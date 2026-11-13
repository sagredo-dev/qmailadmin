/* 
 * $Id: auth.c,v 1.3.2.7 2009-05-02 17:51:40 tomcollins Exp $
 * Copyright (C) 1999-2004 Inter7 Internet Technologies, Inc. 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <vpopmail.h>
#include <vauth.h>
/* undef some macros that get redefined below */
#undef PACKAGE
#undef VERSION
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef QMAILDIR
#undef EZMLMDIR
#include "config.h"
#include "qmailadmin.h"
#include "qmailadminx.h"
#include "cgi.h"
#include "show.h"
#include "util.h"
#include "printh.h"
#include "auth.h"

//extern char *crypt();

/*
 * Sessions
 *
 * A login creates a session: a random handle (128 bits), a random secret
 * (256 bits) and a separate random request token (128 bits). The handle and
 * the secret are sent only as a cookie:
 *
 *   __Host-qmailadmin=<handle>.<secret>; Path=/; Secure; HttpOnly; SameSite=Strict
 *
 * The __Host- prefix makes the browser refuse the cookie unless it is Secure,
 * has Path=/ and no Domain, so it cannot be set or overwritten by another
 * host or over plain http. The request token goes into every link and form
 * (##& in the templates, cgiurl()) as tok=, and every /com/ request must
 * carry both: the cookie identifies the browser, the token shows that the
 * request comes from a page qmailadmin generated (CSRF).
 *
 * Each session is a file in the account's directory,
 * <pw_dir>/qmailadmin-session.<handle>.qw (mode 0600), holding the secret,
 * the token and the login time. An account can have several sessions at
 * once. A session ends 2 hours after login, after 30 minutes without a
 * request, at logout, or when the account's password is changed (the session
 * that changed it stays). A login removes the account's session files that
 * have been idle for longer than that, and the .qw files older versions kept
 * in the Maildir.
 */

#define SESSION_HANDLE_BYTES 16
#define SESSION_ID_BYTES  32
#define SESSION_TOK_BYTES 16
#define SESSION_MAX_AGE   7200
#define SESSION_IDLE      1800
#define SESSION_PREFIX    "qmailadmin-session."
#define SESSION_SUFFIX    ".qw"

char SessionToken[2 * SESSION_TOK_BYTES + 1];
static char SessionHandle[2 * SESSION_HANDLE_BYTES + 1];
static char session_cookie_header[256];

int session_https(void)
{
  const char *https = getenv("HTTPS");

  return (https != NULL && strcasecmp(https, "on") == 0);
}

/* the Set-Cookie header line to send with this response, or "" */
const char *session_cookie(void)
{
  return session_cookie_header;
}

/* fill hex (2 * nbytes + 1 bytes) with nbytes random bytes in hex */
static int random_hex(char *hex, size_t nbytes)
{
  unsigned char buf[64];
  size_t got = 0, i;
  ssize_t r;
  int fd;

  if (nbytes > sizeof(buf)) return -1;
  if ((fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC)) < 0) return -1;
  while (got < nbytes) {
    r = read(fd, buf + got, nbytes - got);
    if (r < 0 && errno == EINTR) continue;
    if (r <= 0) {
      close(fd);
      return -1;
    }
    got += r;
  }
  close(fd);
  for (i = 0; i < nbytes; i++) sprintf(hex + 2 * i, "%02x", buf[i]);
  memset(buf, 0, sizeof(buf));
  return 0;
}

/* compare two secrets without leaking where they differ */
static int secret_equal(const char *a, const char *b)
{
  size_t len = strlen(a), i;
  unsigned char diff = 0;

  if (len == 0 || len != strlen(b)) return 0;
  for (i = 0; i < len; i++) diff |= (unsigned char)a[i] ^ (unsigned char)b[i];
  return diff == 0;
}

static int is_lower_hex(const char *s, size_t len)
{
  return strlen(s) == len && strspn(s, "0123456789abcdef") == len;
}

/* path of the session file with this handle */
static int session_path(char *path, size_t size, const char *dir,
                        const char *handle)
{
  return snprintf(path, size, "%s/" SESSION_PREFIX "%s" SESSION_SUFFIX, dir,
                  handle) < (int)size ? 0 : -1;
}

/* the handle of a session file name, or NULL */
static const char *session_file_handle(const char *name)
{
  static char handle[2 * SESSION_HANDLE_BYTES + 1];
  const size_t plen = strlen(SESSION_PREFIX), hlen = sizeof(handle) - 1;

  if (strncmp(name, SESSION_PREFIX, plen) != 0 ||
      strlen(name) != plen + hlen + strlen(SESSION_SUFFIX) ||
      strcmp(name + plen + hlen, SESSION_SUFFIX) != 0) return NULL;
  memcpy(handle, name + plen, hlen);
  handle[hlen] = '\0';
  return is_lower_hex(handle, hlen) ? handle : NULL;
}

/* Read the handle and the secret from the Cookie header into SessionHandle
 * and id. Returns 0, or -1 when there is no valid session cookie.
 */
static int cookie_session(char *id)
{
  const char *p = getenv("HTTP_COOKIE");
  const size_t namelen = strlen(SESSION_COOKIE "=");
  const size_t hlen = 2 * SESSION_HANDLE_BYTES, idlen = 2 * SESSION_ID_BYTES;
  size_t len;

  while (p != NULL && *p != '\0') {
    while (*p == ' ' || *p == ';') p++;
    if (strncmp(p, SESSION_COOKIE "=", namelen) == 0) {
      p += namelen;
      len = strcspn(p, "; ");
      if (len != hlen + 1 + idlen || p[hlen] != '.') return -1;
      memcpy(SessionHandle, p, hlen);
      SessionHandle[hlen] = '\0';
      memcpy(id, p + hlen + 1, idlen);
      id[idlen] = '\0';
      if (is_lower_hex(SessionHandle, hlen) && is_lower_hex(id, idlen))
        return 0;
      SessionHandle[0] = '\0';
      return -1;
    }
    p = strchr(p, ';');
  }
  return -1;
}

/* Read the file of the current session (SessionHandle) of pw into buf (first
 * line). Returns an open descriptor, so the caller can update the file time,
 * or -1. Refuses symlinks and anything but a regular file.
 */
static int session_read(const struct vqpasswd *pw, char *buf, size_t size,
                        struct stat *st)
{
  char path[MAX_BUFF];
  ssize_t r;
  int fd;

  if (SessionHandle[0] == '\0' ||
      session_path(path, sizeof(path), pw->pw_dir, SessionHandle) != 0)
    return -1;
  if ((fd = open(path, O_RDONLY | O_NOFOLLOW | O_CLOEXEC)) < 0) return -1;
  if (fstat(fd, st) < 0 || !S_ISREG(st->st_mode) || st->st_size <= 0 ||
      (size_t)st->st_size >= size) {
    close(fd);
    return -1;
  }
  if ((r = read(fd, buf, size - 1)) != st->st_size) {
    close(fd);
    return -1;
  }
  buf[r] = '\0';
  buf[strcspn(buf, "\n")] = '\0';
  return fd;
}

/* Remove session files in dir: all of them (all != 0) or those idle for
 * longer than SESSION_IDLE, except the one with handle keep (may be NULL).
 */
static void session_remove(const char *dir, int all, const char *keep)
{
  char path[MAX_BUFF];
  const char *handle;
  struct dirent *de;
  struct stat st;
  time_t now = time(NULL);
  DIR *d;

  if ((d = opendir(dir)) == NULL) return;
  while ((de = readdir(d)) != NULL) {
    if ((handle = session_file_handle(de->d_name)) == NULL) continue;
    if (keep != NULL && strcmp(handle, keep) == 0) continue;
    if (session_path(path, sizeof(path), dir, handle) != 0) continue;
    if (!all && lstat(path, &st) == 0 && S_ISREG(st.st_mode) &&
        now - st.st_mtime <= SESSION_IDLE) continue;
    unlink(path);
  }
  closedir(d);
}

/* Start a new session for pw and prepare the Set-Cookie header. Other
 * sessions of the account stay. Returns 0 on success.
 */
int session_create(const struct vqpasswd *pw, const char *ip_addr,
                   const char *returntext, const char *returnhttp)
{
  char id[2 * SESSION_ID_BYTES + 1];
  char path[MAX_BUFF];
  char line[MAX_BIG_BUFF];
  size_t len;
  int fd;

  if (random_hex(SessionHandle, SESSION_HANDLE_BYTES) != 0 ||
      random_hex(id, SESSION_ID_BYTES) != 0 ||
      random_hex(SessionToken, SESSION_TOK_BYTES) != 0) return -1;

  /* expired sessions of this account, and the files of older qmailadmin
   * versions in the Maildir */
  session_remove(pw->pw_dir, 0, NULL);
  snprintf(path, sizeof(path), "%s/" MAILDIR, pw->pw_dir);
  del_id_files(path);

  if (session_path(path, sizeof(path), pw->pw_dir, SessionHandle) != 0)
    return -1;
  snprinth(line, sizeof(line),
           "sid=%s&tok=%s&created=%lu&ip_addr=%C&returntext=%C&returnhttp=%C\n",
           id, SessionToken, (unsigned long)time(NULL), ip_addr, returntext,
           returnhttp);
  len = strlen(line);
  if (len == 0 || line[len - 1] != '\n') return -1;  /* truncated */

  /* a new name, so no other login or request can see it half written */
  fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600);
  if (fd < 0) return -1;
  if (write(fd, line, len) != (ssize_t)len || close(fd) != 0) {
    unlink(path);
    return -1;
  }

  snprintf(session_cookie_header, sizeof(session_cookie_header),
           "Set-Cookie: " SESSION_COOKIE "=%s.%s; Path=/; Secure; HttpOnly; "
           "SameSite=Strict\n", SessionHandle, id);
  memset(id, 0, sizeof(id));
  memset(line, 0, sizeof(line));
  return 0;
}

/* End the current session of pw and expire the cookie. */
void session_destroy(const struct vqpasswd *pw)
{
  char path[MAX_BUFF];

  if (pw != NULL && SessionHandle[0] != '\0' &&
      session_path(path, sizeof(path), pw->pw_dir, SessionHandle) == 0)
    unlink(path);
  SessionHandle[0] = '\0';
  memset(SessionToken, 0, sizeof(SessionToken));
  snprintf(session_cookie_header, sizeof(session_cookie_header),
           "Set-Cookie: " SESSION_COOKIE "=; Path=/; Secure; HttpOnly; "
           "SameSite=Strict; Max-Age=0\n");
}

/* End the sessions of user@domain after a password change, except the
 * current session when it belongs to that account.
 */
void session_end_others(const char *user, const char *domain)
{
  struct vqpasswd *pw;
  char dir[MAX_BUFF];
  int own;

  if ((pw = vauth_getpw((char *)user, (char *)domain)) == NULL) return;
  snprintf(dir, sizeof(dir), "%s", pw->pw_dir);
  own = SessionHandle[0] != '\0' && strcmp(user, Username) == 0 &&
        strcmp(domain, Domain) == 0;
  session_remove(dir, 1, own ? SessionHandle : NULL);
}

/* Check the session of pw for this request: cookie, request token (tok= in
 * TmpCGI), client address (with IPAUTH) and age. Returns 0 when valid,
 * -1 when invalid and -2 when expired.
 */
int session_check(const struct vqpasswd *pw, const char *ip_addr)
{
  char line[MAX_BIG_BUFF];
  char value[MAX_BUFF];
  char id[2 * SESSION_ID_BYTES + 1];
  char tok[2 * SESSION_TOK_BYTES + 2];
  struct stat st;
  time_t now, created;
  int fd, ok;

  if (pw == NULL || cookie_session(id) != 0) return -1;
  if (GetValue(TmpCGI, tok, "tok=", sizeof(tok)) != 0) return -1;
  if ((fd = session_read(pw, line, sizeof(line), &st)) < 0) return -1;

  GetValue(line, value, "sid=", sizeof(value));
  ok = is_lower_hex(value, 2 * SESSION_ID_BYTES) && secret_equal(id, value);
  GetValue(line, value, "tok=", sizeof(value));
  ok = ok && is_lower_hex(value, 2 * SESSION_TOK_BYTES) &&
       secret_equal(tok, value);
  memset(id, 0, sizeof(id));
  if (!ok) {
    close(fd);
    return -1;
  }
  strcpy(SessionToken, value);

#ifdef IPAUTH
  GetValue(line, value, "ip_addr=", sizeof(value));
  if (strcmp(ip_addr, value) != 0) {
    close(fd);
    return -1;
  }
#endif

  GetValue(line, value, "created=", sizeof(value));
  created = (time_t)strtoul(value, NULL, 10);
  now = time(NULL);
  if (created == 0 || now - created > SESSION_MAX_AGE ||
      now - st.st_mtime > SESSION_IDLE) {
    close(fd);
    session_destroy(pw);
    return -2;
  }

  /* the file time records the last request, for the idle limit */
  futimens(fd, NULL);
  close(fd);
  return 0;
}

/* Value of key ("returntext=", ...) saved in the current session of pw,
 * or "" */
char *session_value(const struct vqpasswd *pw, const char *key)
{
  static char value[MAX_BUFF];
  char line[MAX_BIG_BUFF];
  struct stat st;
  int fd;

  value[0] = '\0';
  if (pw == NULL || (fd = session_read(pw, line, sizeof(line), &st)) < 0)
    return value;
  close(fd);
  if (GetValue(line, value, (char *)key, sizeof(value)) != 0) value[0] = '\0';
  return value;
}

void set_admin_type()
{
 struct vqpasswd *vpw=NULL;

  vpw = vauth_getpw(Username, Domain);
  AdminType = NO_ADMIN;
  if ( vpw != NULL && strlen(Domain) > 0 ) {
    if ( strcmp(Username,"postmaster")==0 ) {
      AdminType = DOMAIN_ADMIN;
#ifdef VQPASSWD_HAS_PW_FLAGS
    } else if ( vpw->pw_flags & QA_ADMIN ) {
#else
    } else if ( vpw->pw_gid & QA_ADMIN ) {
#endif
      AdminType = DOMAIN_ADMIN;
    } else {
      AdminType = USER_ADMIN;
    }
  }
}
