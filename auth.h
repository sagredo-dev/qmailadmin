/*
 * $Id: auth.h,v 1.1.2.1 2004-11-20 01:10:41 tomcollins Exp $
 */

/* name of the session cookies, followed by the session handle; the __Host-
 * prefix requires https */
#define SESSION_COOKIE "__Host-qmailadmin-"

extern char SessionToken[];

int session_https(void);
const char *session_cookie(void);
int session_create(const struct vqpasswd *pw, const char *ip_addr,
                   const char *returntext, const char *returnhttp);
int session_check(const struct vqpasswd *pw, const char *ip_addr);
void session_destroy(const struct vqpasswd *pw);
void session_end_others(const char *user, const char *domain);
char *session_value(const struct vqpasswd *pw, const char *key);
void set_admin_type();

