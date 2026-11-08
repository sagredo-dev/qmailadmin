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
 * A login creates a random session id (256 bits) and a separate random
 * request token (128 bits). The id is sent only as a cookie:
 *
 *   __Host-qmailadmin=<id>; Path=/; Secure; HttpOnly; SameSite=Strict
 *
 * The __Host- prefix makes the browser refuse the cookie unless it is Secure,
 * has Path=/ and no Domain, so it cannot be set or overwritten by another
 * host or over plain http. The request token goes into every link and form
 * (##& in the templates, cgiurl()) as tok=, and every /com/ request must
 * carry both: the cookie identifies the browser, the token shows that the
 * request comes from a page qmailadmin generated (CSRF).
 *
 * Both are stored in <user's Maildir>/qmailadmin-session.qw (mode 0600,
 * replaced at every login, removed at logout). A session ends 2 hours after
 * login or after 30 minutes without a request.
 */

#define SESSION_ID_BYTES  32
#define SESSION_TOK_BYTES 16
#define SESSION_MAX_AGE   7200
#define SESSION_IDLE      1800

char SessionToken[2 * SESSION_TOK_BYTES + 1];
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

static void session_path(char *path, size_t size, const struct vqpasswd *pw)
{
  snprintf(path, size, "%s/" MAILDIR "/qmailadmin-session.qw", pw->pw_dir);
}

/* the session id from the Cookie header, or NULL */
static const char *cookie_session_id(void)
{
  static char id[2 * SESSION_ID_BYTES + 1];
  const char *p = getenv("HTTP_COOKIE");
  const size_t namelen = strlen(SESSION_COOKIE "=");
  size_t len;

  while (p != NULL && *p != '\0') {
    while (*p == ' ' || *p == ';') p++;
    if (strncmp(p, SESSION_COOKIE "=", namelen) == 0) {
      p += namelen;
      len = strcspn(p, "; ");
      if (len != sizeof(id) - 1) return NULL;
      memcpy(id, p, len);
      id[len] = '\0';
      return is_lower_hex(id, len) ? id : NULL;
    }
    p = strchr(p, ';');
  }
  return NULL;
}

/* Read the session file of pw into buf (first line). Returns an open
 * descriptor, so the caller can update the file time, or -1. Refuses
 * symlinks and anything but a regular file.
 */
static int session_read(const struct vqpasswd *pw, char *buf, size_t size,
                        struct stat *st)
{
  char path[MAX_BUFF];
  ssize_t r;
  int fd;

  session_path(path, sizeof(path), pw);
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

/* Start a new session for pw: replaces any earlier session of this user and
 * prepares the Set-Cookie header. Returns 0 on success.
 */
int session_create(const struct vqpasswd *pw, const char *ip_addr,
                   const char *returntext, const char *returnhttp)
{
  char id[2 * SESSION_ID_BYTES + 1];
  char path[MAX_BUFF], tmppath[MAX_BUFF];
  char line[MAX_BIG_BUFF];
  size_t len;
  int fd;

  if (random_hex(id, SESSION_ID_BYTES) != 0 ||
      random_hex(SessionToken, SESSION_TOK_BYTES) != 0) return -1;

  /* removes earlier sessions (and the files of older qmailadmin versions) */
  snprintf(path, sizeof(path), "%s/" MAILDIR, pw->pw_dir);
  del_id_files(path);

  session_path(path, sizeof(path), pw);
  if (snprintf(tmppath, sizeof(tmppath), "%s.tmp", path) >= (int)sizeof(tmppath))
    return -1;
  snprinth(line, sizeof(line),
           "sid=%s&tok=%s&created=%lu&ip_addr=%C&returntext=%C&returnhttp=%C\n",
           id, SessionToken, (unsigned long)time(NULL), ip_addr, returntext,
           returnhttp);
  len = strlen(line);
  if (len == 0 || line[len - 1] != '\n') return -1;  /* truncated */

  unlink(tmppath);
  fd = open(tmppath, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600);
  if (fd < 0) return -1;
  if (write(fd, line, len) != (ssize_t)len) {
    close(fd);
    unlink(tmppath);
    return -1;
  }
  if (close(fd) != 0 || rename(tmppath, path) != 0) {
    unlink(tmppath);
    return -1;
  }

  snprintf(session_cookie_header, sizeof(session_cookie_header),
           "Set-Cookie: " SESSION_COOKIE "=%s; Path=/; Secure; HttpOnly; "
           "SameSite=Strict\n", id);
  memset(id, 0, sizeof(id));
  return 0;
}

/* End the session of pw and expire the cookie. */
void session_destroy(const struct vqpasswd *pw)
{
  char path[MAX_BUFF];

  if (pw != NULL) {
    session_path(path, sizeof(path), pw);
    unlink(path);
  }
  memset(SessionToken, 0, sizeof(SessionToken));
  snprintf(session_cookie_header, sizeof(session_cookie_header),
           "Set-Cookie: " SESSION_COOKIE "=; Path=/; Secure; HttpOnly; "
           "SameSite=Strict; Max-Age=0\n");
}

/* Check the session of pw for this request: cookie, request token (tok= in
 * TmpCGI), client address (with IPAUTH) and age. Returns 0 when valid,
 * -1 when invalid and -2 when expired.
 */
int session_check(const struct vqpasswd *pw, const char *ip_addr)
{
  char line[MAX_BIG_BUFF];
  char value[MAX_BUFF];
  char tok[2 * SESSION_TOK_BYTES + 2];
  const char *id;
  struct stat st;
  time_t now, created;
  int fd, ok;

  if (pw == NULL || (id = cookie_session_id()) == NULL) return -1;
  if (GetValue(TmpCGI, tok, "tok=", sizeof(tok)) != 0) return -1;
  if ((fd = session_read(pw, line, sizeof(line), &st)) < 0) return -1;

  GetValue(line, value, "sid=", sizeof(value));
  ok = is_lower_hex(value, 2 * SESSION_ID_BYTES) && secret_equal(id, value);
  GetValue(line, value, "tok=", sizeof(value));
  ok = ok && is_lower_hex(value, 2 * SESSION_TOK_BYTES) &&
       secret_equal(tok, value);
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

/* Value of key ("returntext=", ...) saved in the session of pw, or "" */
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
