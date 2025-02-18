/* 
 * $Id: user.c,v 1.11.2.21 2009-05-02 19:13:29 tomcollins Exp $
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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <vpopmail_config.h>
#include "vpopmail.h"
#include "pwstr.h"
/* undef some macros that get redefined in config.h below */
#undef PACKAGE
#undef VERSION
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef QMAILDIR
#include "alias.h"
#include "cgi.h"
#include "config.h"
#include "limits.h"
#include "printh.h"
#include "qmailadmin.h"
#include "qmailadminx.h"
#include "show.h"
#include "template.h"
#include "user.h"
#include "util.h"
#include "vauth.h"
/* cracklib patch */
#ifdef CRACKLIB
#	include <crack.h>
#endif
/* end cracklib patch */

#define HOOKS 1

#ifdef HOOKS
/* note that as of December 2003, only the first three hooks are 
implemented */
#define HOOK_ADDUSER     "adduser"
#define HOOK_DELUSER     "deluser"
#define HOOK_MODUSER     "moduser"
#define HOOK_ADDMAILLIST "addmaillist"
#define HOOK_DELMAILLIST "delmaillist"
#define HOOK_MODMAILLIST "modmaillist"
#define HOOK_LISTADDUSER "addlistuser"
#define HOOK_LISTDELUSER "dellistuser"
#endif

void show_users(char *Username, char *Domain, time_t Mytime)
{
  if (MaxPopAccounts == 0) return;
  send_template("show_users.html");
}


int show_user_lines(char *user, char *dom, time_t mytime, char *dir)
{
 int  i,j,k,startnumber,moreusers = 1;
 FILE *fs;
 struct vqpasswd *pw;
 int totalpages;
 int bounced;
 int colspan = 7;
 int allowdelete;
 char qconvert[11];
 int bars;

  if (MaxPopAccounts == 0) return 0;

  /* Get the default catchall box name */
  if ((fs=fopen(".qmail-default","r")) == NULL) {
    /* report error opening .qmail-default and exit */
    printf ("<tr><td colspan=\"%i\">%s .qmail-default</td></tr>\n", 
      colspan, html_text[144]);
    vclose();
    exit(0);
  }

  fgets(TmpBuf, sizeof(TmpBuf), fs);
  fclose(fs);

  if (*SearchUser) {
    pw = vauth_getall(dom,1,1);
    for (k=0; pw!=NULL; k++) {
      if ((!SearchUser[1] && *pw->pw_name >= *SearchUser) ||
          ((!strcmp(SearchUser, pw->pw_name)))) {
        break;
      }

      pw = vauth_getall(dom,0,0);
    }

    if (k == 0) strcpy (Pagenumber, "1");
    else sprintf(Pagenumber, "%d", (k/MAXUSERSPERPAGE)+1);
  }

  /* Determine number of pages */
  pw = vauth_getall(dom,1,1);
  for (k=0; pw!=NULL; k++) pw = vauth_getall(dom, 0, 0);

  if (k == 0) totalpages = 1;
  else totalpages = ((k/MAXUSERSPERPAGE)+1);

  /* End determine number of pages */
  if (atoi(Pagenumber)==0) *Pagenumber='1';

  if ( strstr(TmpBuf, " bounce-no-mailbox\n") != NULL ) {
    bounced = 1;
  } else if ( strstr(TmpBuf, "@") != NULL ) {
    bounced = 0;
    /* check for local user to forward to */
    if (strstr(TmpBuf, dom) != NULL) {
      i = strlen(TmpBuf); --i; TmpBuf[i] = 0; /* take off newline */
      for(;TmpBuf[i]!=' ';--i);
      for(j=0,++i;TmpBuf[i]!=0 && TmpBuf[i]!='@';++j,++i) 
         TmpBuf3[j] = TmpBuf[i];
      TmpBuf3[j]=0;
    }
  } else {
    /* Maildir type catchall */
    bounced = 0;
    i = strlen(TmpBuf); --i; TmpBuf[i] = 0; /* take off newline */
    for(;TmpBuf[i]!='/';--i);
    for(j=0,++i;TmpBuf[i]!=0;++j,++i) TmpBuf3[j] = TmpBuf[i];
    TmpBuf3[j]=0;
  }

  startnumber = MAXUSERSPERPAGE * (atoi(Pagenumber) - 1);

  /*
   * check to see if there are any users to list, 
   * otherwise repeat previous page
   *  
   */
  pw = vauth_getall(dom,1,1);
  if ( AdminType==DOMAIN_ADMIN ||
       (AdminType==USER_ADMIN && strcmp(pw->pw_name,Username)==0)){

    for (k = 0; k < startnumber; ++k) { 
      pw = vauth_getall(dom,0,0); 
    }
  }

  if (pw == NULL) {
    printf ("<tr><td colspan=\"%i\">%s</td></tr>\n", 
      colspan, html_text[131]);
      moreusers = 0;
    } else {
      char path[256];
      while ((pw != NULL) && ((k < MAXUSERSPERPAGE + startnumber) ||  
              (AdminType!=DOMAIN_ADMIN || AdminType!=DOMAIN_ADMIN || 
              (AdminType==USER_ADMIN && strcmp(pw->pw_name,Username)==0)))) {
        if (AdminType==DOMAIN_ADMIN || 
            (AdminType==USER_ADMIN && strcmp(pw->pw_name,Username)==0)) {
          storage_t diskquota = 0;
          storage_t maxmsg = 0;

          /* display account name and user name */
          printf ("<tr>\n");
          printh ("<td>%H</td>\n", pw->pw_name);
          printh ("<td>%H</td>\n", pw->pw_gecos);

          /* display user's quota */
	  snprintf(path, sizeof(path), "%s/" MAILDIR, pw->pw_dir);
          readuserquota(path, &diskquota, &maxmsg);
          printf ("<td>%-2.2lf&nbsp;/&nbsp;</td>\n", ((double)diskquota)/1048576.0);  /* Convert to MB */
          if (strncmp(pw->pw_shell, "NOQUOTA", 2) != 0) {
              if(quota_to_megabytes(qconvert, pw->pw_shell)) {
                  printf ("<td>(BAD)</td>\n");
              }
              else { printf ("<td>%s</td>\n", qconvert); }
          }
          else { printf ("<td>%s</td>", html_text[229]); }

          /* display button to modify user */
          printf ("<td>");
          printh ("<a href=\"%s&moduser=%C\">", cgiurl("moduser"), pw->pw_name);
          printf ("<span class=\"material-icons\">create</span></a>");
          printf ("</td>\n");

          /* if the user has admin privileges and pw->pw_name is not 
           * the user or postmaster, allow deleting 
           */
          if (AdminType==DOMAIN_ADMIN && 
               strcmp(pw->pw_name, Username) != 0 &&
               strcmp(pw->pw_name, "postmaster") != 0) {
            allowdelete = 1;

          /* else, don't allow deleting */
          } else {
            allowdelete = 0;
          }

          /* display trashcan for delete, or nothing if delete not allowed */
          printf ("<td>");
          if (allowdelete) {
            printh ("<a href=\"%s&deluser=%C\">", cgiurl("deluser"), pw->pw_name);
            printf ("<span class=\"material-icons\">delete_forever</span></a>");
          } else {
            /* printf ("<span class=\"material-icons\">flash_off</span>"); */
          }
          printf ("</td>\n");

          /* display button in the 'set catchall' column */
          printf ("<td>");
          if (bounced==0 && strncmp(pw->pw_name,TmpBuf3,sizeof(TmpBuf3)) == 0) {
            printf ("<span class=\"material-icons\">radio_button_checked</span></a>");
#ifdef CATCHALL_ENABLED
          } else if (AdminType==DOMAIN_ADMIN) {
            printh ("<a href=\"%s&deluser=%C&page=%s\">",
              cgiurl("setdefault"), pw->pw_name, Pagenumber);
            printf ("<span class=\"material-icons\">radio_button_unchecked</span></a>");
#endif
          } else {
            printf ("<span class=\"material-icons\">flash_off</span>");
          }
          printf ("</td>\n");
          printf ("</tr>\n");
        }        
        pw = vauth_getall(dom,0,0);
        ++k;
      }
    }

    if (AdminType == DOMAIN_ADMIN) {
      print_user_index ("showusers", colspan, user, dom, mytime);

      printf ("<tr>\n");
      printf ("<td class=\"text-center\" colspan=\"%i\">", colspan);
//      printf ("[&nbsp;");
      bars = 0;
#ifdef USER_INDEX
      /* only display "previous page" if pagenumber > 1 */
      if (atoi(Pagenumber) > 1) {
        printh ("<a href=\"%s&page=%d\">%s</a>", cgiurl ("showusers"),
          atoi(Pagenumber)-1 ? atoi(Pagenumber)-1 : atoi(Pagenumber), 
          html_text[135]);
        bars = 1;
      }

      if (moreusers && atoi(Pagenumber) < totalpages) {
        if (bars) printf ("&nbsp;|&nbsp;");
        printh ("<a href=\"%s&page=%d\">%s</a>",
          cgiurl("showusers"), atoi(Pagenumber)+1, html_text[137]);
        bars = 1;
      }
#endif
#ifdef CATCHALL_ENABLED
      if (bars) printf ("&nbsp;|&nbsp;");
      printh ("<a href=\"%s\">%s</a>", cgiurl ("deleteall"), html_text[235]);
      printf ("&nbsp;|&nbsp;");
      printh ("<a href=\"%s\">%s</a>", cgiurl ("bounceall"), html_text[134]);
      printf ("&nbsp;|&nbsp;");
      printh ("<a href=\"%s\">%s</a>", cgiurl("setremotecatchall"), html_text[206]);
#endif
//      printf ("&nbsp;]");
      printf ("</td>\n</tr>\n");
  }
  return 0;
}

void adduser()
{
  count_users();
  load_limits();

  if ( AdminType!=DOMAIN_ADMIN ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }

  if ( MaxPopAccounts != -1 && CurPopAccounts >= MaxPopAccounts ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s %d\n", html_text[199],
      MaxPopAccounts);
    show_menu(Username, Domain, Mytime);
    vclose();
    exit(0);
  }

  send_template( "add_user.html" );

}

void moduser()
{
  if (!( AdminType==DOMAIN_ADMIN ||
        (AdminType==USER_ADMIN && strcmp(ActionUser,Username)==0))){
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }
  send_template( "mod_user.html" );
} 

void addusernow()
{
#ifdef CRACKLIB
 const char *tmpstr;
#endif
 int cnt=0, num;
 char *c_num;
 char **mailingListNames;
 char *tmp;
 char *email;
#ifdef MODIFY_QUOTA
 char qconvert[11];
#endif
 int pid;
 int error;
 struct vqpasswd *mypw;
#ifdef MODIFY_SPAM
 char spamvalue[50];
 static char NewBuf[156];
 FILE *fs;
#endif

  c_num = malloc(MAX_BUFF);
  email = malloc(128);
  tmp = malloc(MAX_BUFF);

  count_users();
  load_limits();

  if ( AdminType!=DOMAIN_ADMIN ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }

  if ( MaxPopAccounts != -1 && CurPopAccounts >= MaxPopAccounts ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s %d\n", html_text[199],
      MaxPopAccounts);
    show_menu(Username, Domain, Mytime);
    vclose();
    exit(0);
  }
 
  GetValue(TmpCGI,Newu, "newu=", sizeof(Newu));

  if ( fixup_local_name(Newu) ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s %s\n", html_text[148], Newu);
    adduser();
    vclose();
    exit(0);
  } 

  if ( check_local_user(Newu) ) {
    snprinth (StatusMessage, sizeof(StatusMessage), "%s %H\n", html_text[175], Newu);
    adduser();
    vclose();
    exit(0);
  } 
  // Coded added by jhopper
#ifdef MODIFY_QUOTA
  GetValue(TmpCGI, Quota, "quota=", sizeof(Quota));
#endif
  GetValue(TmpCGI,Password1, "password1=", sizeof(Password1));
  GetValue(TmpCGI,Password2, "password2=", sizeof(Password2));
  if ( strcmp( Password1, Password2 ) != 0 ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[200]);
    adduser();
    vclose();
    exit(0);
  }

#ifndef TRIVIAL_PASSWORD_ENABLED
  if ( strstr(Newu,Password1)!=NULL) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[320]);
    adduser();
    vclose();
    exit(0);
  }
#endif
/* cracklib patch */
#ifdef CRACKLIB
    if ((tmpstr = FascistCheck(Password1, CRACKLIB)) != NULL ) {
       sprintf(StatusMessage, "Bad password - %s\n", tmpstr);
       adduser();
       vclose();
       exit(0);
    }
#endif
/* end cracklib */

#ifndef ENABLE_LEARN_PASSWORDS
  if ( strlen(Password1) <= 0 ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[234]);
    adduser();
    vclose();
    exit(0);
  }
#endif

  snprintf (email, MAX_BIG_BUFF, "%s@%s", Newu, Domain);

  GetValue(TmpCGI,Gecos, "gecos=", sizeof(Gecos));
  if ( strlen( Gecos ) == 0 ) {
    strcpy(Gecos, Newu);
  }

  GetValue(TmpCGI, c_num, "number_of_mailinglist=", MAX_BUFF);
  num = atoi(c_num);
  if (num > 0) {
    if (!(mailingListNames = malloc(sizeof(char *) * num))) {
      snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[201]);
      vclose();
      exit(0);
    } else {
      for(cnt = 0; cnt < num; cnt++) {
        if(!(mailingListNames[cnt] = malloc(MAX_BUFF))) {
          snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[201]);
          vclose();
          exit(0);
        }
      }

      for(cnt = 0; cnt < num; cnt++) {
        sprintf(tmp, "subscribe%d=", cnt);
        error = GetValue(TmpCGI, mailingListNames[cnt], tmp, MAX_BUFF);
        if( error != -1 ) {
          pid=fork();

          if (pid==0) {
            sprintf(TmpBuf1, "%s/ezmlm-sub", EZMLMDIR);
            sprintf(TmpBuf2, "%s/%s", RealDir, mailingListNames[cnt]);
            execl(TmpBuf1, "ezmlm-sub", TmpBuf2, email, NULL);
            exit(127);
          } else {
            wait(&pid);
          }
        } 
      }
    }
  }

  /* add the user then get the vpopmail password structure */
  int i;
  if ( (i = vadduser( Newu, Domain, Password1, Gecos, USE_POP )) == 0 && 
#ifdef MYSQL_REPLICATION
    !sleep(2) &&
#endif
    (mypw = vauth_getpw( Newu, Domain )) != NULL ) {

    /* vadduser() in vpopmail 5.3.29 and later sets the default
     * quota, so we only need to change it if the user enters
     * something in the Quota field.
     */

#ifdef MODIFY_QUOTA
    if (strcmp (Quota, "NOQUOTA") == 0) {
      vsetuserquota (Newu, Domain, "NOQUOTA");
    } else if ( Quota[0] != 0 ) {
      if(quota_to_bytes(qconvert, Quota)) { 
        snprintf (StatusMessage, sizeof(StatusMessage), html_text[314]);
      } else {
        vsetuserquota (Newu, Domain, qconvert);
      }
    }
#endif

#ifdef MODIFY_SPAM
    GetValue(TmpCGI, spamvalue, "spamcheck=", sizeof(spamvalue));
    if(strcmp(spamvalue, "on") == 0) {
       snprintf(NewBuf, sizeof(NewBuf), "%s/.qmail", mypw->pw_dir);
       fs = fopen(NewBuf, "w+");
#ifdef MODIFY_SPAM_NEED_EMAIL
       fprintf(fs, "%s %s\n", SPAM_COMMAND, email);
#else  
       fprintf(fs, "%s\n", SPAM_COMMAND);
#endif
       fclose(fs);
    }
#endif

    /* report success */
    snprinth (StatusMessage, sizeof(StatusMessage), "%s %H@%H (%H) %s",
      html_text[2], Newu, Domain, Gecos,
      html_text[119]);
  }

  /* pwd strength check */
  else if (((i <= -69) && (i >= -74)) && (pw_strength_policy() != NULL)) {
      snprinth (StatusMessage, sizeof(StatusMessage),
      "%s %H@%H %s. %s",
      html_text[2], Newu, Domain, html_text[120], pw_strength_policy());
  }
  else {
      /* otherwise, report error */
      snprinth (StatusMessage, sizeof(StatusMessage), "%s %H@%H (%H) %s",
      html_text[2], Newu, Domain, Gecos, html_text[120]);
  }

  call_hooks(HOOK_ADDUSER, Newu, Domain, Password1, Gecos);

  /* After we add the user, show the user page
   * people like to visually verify the results
   */
  show_users(Username, Domain, Mytime);

}

int call_hooks(char *hook_type, char *p1, char *p2, char *p3, char *p4)
{
 FILE *fs = NULL;
 int pid;
 char *hooks_path;
 char *cmd = NULL;
 char *tmpstr;
 // int error;

  hooks_path = malloc(MAX_BUFF);

  /* first look in directory for domain */
  sprintf(hooks_path, "%s/.qmailadmin-hooks", RealDir);
  if((fs = fopen(hooks_path, "r")) == NULL) {
    /* then try ~vpopmail/etc */
    sprintf(hooks_path, "%s/etc/.qmailadmin-hooks", VPOPMAILDIR);
    if((fs = fopen(hooks_path, "r")) == NULL) {
      return (0);
    }
  }

  while(fgets(TmpBuf, sizeof(TmpBuf), fs) != NULL) {
    if ( (*TmpBuf == '#') || (*TmpBuf == '\0')) continue;
    tmpstr = strtok(TmpBuf, " :\t\n");
    if (tmpstr == NULL) continue;

    if ( strcmp(tmpstr, hook_type) == 0) {
      tmpstr = strtok(NULL, " :\t\n");

      if ((tmpstr == NULL) || (*tmpstr == '\0')) continue;

      cmd = strdup(tmpstr);        

      break;
    }
  }

  fclose(fs);

  if (cmd == NULL) return 0;   /* don't have a hook for this type */
    
  pid = fork();

  if (pid == 0) {
    /* Second param to execl should actually be just the program name,
       without the path information.  Add a pointer to point into cmd
       at the start of the program name only.    BUG 2003-12 */
    // error = execl(cmd, cmd, p1, p2, p3, p4, NULL);
    printf ("Error %d %s \"%s\", %s, %s, %s, %s, %s\n",
      errno, html_text[202], cmd, hook_type, p1, p2, p3, p4);
    /* if (error == -1) return (-1); */
    exit(127);
  } else {
    wait(&pid);
  }

  return (0);
}

void deluser()
{
  send_template( "del_user_confirm.html" );
}

void delusergo()
{
 static char forward[200] = "";
 static char forwardto[200] = "";
     
  if ( AdminType!=DOMAIN_ADMIN ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }

  vdeluser( ActionUser, Domain );

  snprinth (StatusMessage, sizeof(StatusMessage), "%H %s", ActionUser, html_text[141]);

  /* Start create forward when delete - 
   * Code added by Eugene Teo 6 June 2000 
   * Modified by Jeff Hedlund (jeff.hedlund@matrixsi.com) 4 June 2003 
   */

  GetValue(TmpCGI,forward, "forward=", sizeof(forward));
  if (strcmp(forward, "on") == 0) {
    GetValue(TmpCGI, forwardto, "forwardto=", sizeof(forwardto));    
    if(adddotqmail_shared(ActionUser, forwardto, -1)!=0) {
       snprintf (StatusMessage, sizeof(StatusMessage), html_text[315], forwardto);
    }
  } 

  call_hooks(HOOK_DELUSER, ActionUser, Domain, forwardto, "");
  show_users(Username, Domain, Mytime);
}

void count_users()
{
 struct vqpasswd *pw;

  CurPopAccounts = 0;
  pw = vauth_getall(Domain,1,0);
  while(pw!=NULL){
    ++CurPopAccounts;
    pw = vauth_getall(Domain,0,0);
  }
}

void setremotecatchall() 
{
  send_template("setremotecatchall.html");
}

void set_qmaildefault(char *opt)
{
 FILE *fs;

  if ( (fs = fopen(".qmail-default", "w")) == NULL ) {
    printf ("%s %s<br>\n", html_text[144], ".qmail-default");
  } else {
    fprintf(fs,"| %s/bin/vdelivermail '' %s\n", VPOPMAILDIR, opt);
    fclose(fs);
  }
  show_users(Username, Domain, Mytime);
  vclose();
  exit(0);
}

void setremotecatchallnow() 
{
  char *fwdaddr;

  GetValue(TmpCGI,Newu, "newu=", sizeof(Newu));

  if (check_email_addr(Newu) ) {
    snprinth (StatusMessage, sizeof(StatusMessage), "%s %H\n", html_text[148], Newu);
    setremotecatchall();
    exit(0);
  }
  if (*Newu == '@') {
    /* forward all mail to external domain */
    fwdaddr = malloc (strlen(Newu) + 4 + 1);
    if (fwdaddr != NULL) {
      sprintf (fwdaddr, "$EXT%s", Newu);
      set_qmaildefault (fwdaddr);
      free (fwdaddr);
    }
  } else {
    set_qmaildefault (Newu);
  }
}

void bounceall()
{
  set_qmaildefault ("bounce-no-mailbox");
}

void deleteall()
{
  set_qmaildefault ("delete");
}

int get_catchall()
{
 int i,j;
 FILE *fs;

  /* Get the default catchall box name */
  if ((fs=fopen(".qmail-default","r")) == NULL) {
    printf ("<tr><td colspan=\"5\">%s %s</td></tr>\n", 
      html_text[144], ".qmail-default");
    vclose();
    exit(0);
  }
  fgets( TmpBuf, sizeof(TmpBuf), fs);
  fclose(fs);

  if (strstr(TmpBuf, " bounce-no-mailbox\n") != NULL) {
    printf ("%s", html_text[130]);

  } else if (strstr(TmpBuf, " delete\n") != NULL) {
    printf ("%s", html_text[236]);

  } else if ( strstr(TmpBuf, "@") != NULL ) {
    i=strlen(TmpBuf);
    for(;TmpBuf[i-1]!=' ';--i);
    if (strncmp (&TmpBuf[i], "$EXT@", 5) == 0) {
      /* forward to an entire domain */
      printh ("%s user%H", html_text[62], &TmpBuf[i+4]);
    } else {
      printh ("%s %H", html_text[62], &TmpBuf[i]);
    }
  } else {
    i = strlen(TmpBuf) - 1;
    for(;TmpBuf[i]!='/';--i);
    for(++i,j=0;TmpBuf[i]!=0;++j,++i) TmpBuf2[j] = TmpBuf[i];
    TmpBuf2[j--] = '\0';

    /* take off newline */
    i = strlen(TmpBuf2); --i; TmpBuf2[i] = 0;/* take off newline */
    printh ("%s %H", html_text[62], TmpBuf2);
  }
  return 0;
}

int makevacation (FILE *d, char *dir)
{
 char subject[80];
 FILE *f;
 char fn[156];
  
  GetValue(TmpCGI, subject, "vsubject=", sizeof(subject));

  /* if no subject, error */
  if ( *subject == '\0' ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[216]);
    return 1;
  }
    
  /* make the vacation directory */
  snprintf (fn, sizeof(fn), "%s/vacation", dir);
  mkdir (fn, 0750);

#ifdef USE_QMAIL_AUTORESPONDER
  fprintf (d, "| %s/qmail-autoresponder %s/vacation\n",
    AUTORESPOND_PATH, dir);

  /* set up the message file */
  snprintf(fn, sizeof(fn), "%s/vacation/message.txt", dir);
  GetValue(TmpCGI, Message, "vmessage=", sizeof(Message));
#else
  fprintf (d, "| %s/autorespond 86400 3 %s/vacation/message %s/vacation\n",
    AUTORESPOND_PATH, dir, dir );
      
  /* set up the message file */
  snprintf(fn, sizeof(fn), "%s/vacation/message", dir);
  GetValue(TmpCGI, Message, "vmessage=", sizeof(Message));
#endif

  if ( (f = fopen(fn, "w")) == NULL ) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s %s\n", html_text[150], fn);
    return 1;
  }
  fprintf(f, "From: %s@%s\n", ActionUser, Domain);
  fprintf(f, "Subject: %s\n\n%s", subject, Message);
  fclose(f);

  return 0;
}

void modusergo()
{
 char *tmpstr;
#ifdef CRACKLIB
 const char *tmpstr2;
#endif
 struct vqpasswd *vpw=NULL;
 static char box[500];
 char cforward[50];
 static char NewBuf[156];
 char dotqmailfn[156];
#ifdef MODIFY_QUOTA
 char *quotaptr;
 char qconvert[11];
#endif
 int count;
 FILE *fs;
 int spam_check = 0;
 int vacation = 0;
 int saveacopy = 0;
 int emptydotqmail;
 char *olddotqmail = NULL;
 char *dotqmailline;
 struct stat sb;
 int err;
 int ret_code;

 const char *flagfields[] = { "zeroflag=", "oneflag=", "twoflag=", "threeflag=" };
 const gid_t gidflags[] = { V_USER0, V_USER1, V_USER2, V_USER3 };
 gid_t orig_gid;
 int i;

  if (!( AdminType==DOMAIN_ADMIN ||
         (AdminType==USER_ADMIN && strcmp(ActionUser,Username)==0))){
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }

  if (strlen(Password1)>0 && strlen(Password2)>0 ) {
    if ( strcmp( Password1, Password2 ) != 0 ) {
      snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[200]);
      moduser();
      vclose();
      exit(0);
    }
#ifndef TRIVIAL_PASSWORD_ENABLED    
     if ( strstr(ActionUser,Password1)!=NULL) {
       snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[320]);
       moduser();
       vclose();
       exit(0);
     }
#endif
/* cracklib patch */
#ifdef CRACKLIB
    if ((tmpstr2 = FascistCheck(Password1, CRACKLIB)) != NULL ) {
       sprintf(StatusMessage, "Bad password - %s\n", tmpstr2);
       moduser();
       vclose();
       exit(0);
    }
#endif
/* end cracklib */
    if ( (ret_code = vpasswd( ActionUser, Domain, Password1, USE_POP)) != VA_SUCCESS ) {
      /* pwd strength check */
      if (((ret_code <= -69) && (ret_code >= -74)) && (pw_strength_policy() != NULL)) {
        snprinth (StatusMessage, sizeof(StatusMessage), "%s %H@%H %s. %s",
        html_text[2], ActionUser, Domain, html_text[120], pw_strength_policy());
      }
      /* end pwd strength */
      else
      snprintf (StatusMessage, sizeof(StatusMessage), "%s (%s)", html_text[140], verror(ret_code));
      moduser();
      vclose();
      exit(0);
    }
    else {
//      snprinth (StatusMessage, sizeof(StatusMessage), "%s %H@%H.", html_text[139], ActionUser, Domain );
      strcpy (StatusMessage, html_text[139]);
    }
  }

#ifdef MODIFY_QUOTA
  /* strings used: 307 = "Invalid Quota", 308 = "Quota set to unlimited",
   * 309 = "Quota set to %s bytes"
   */
  if (AdminType == DOMAIN_ADMIN) {
    GetValue(TmpCGI, Quota, "quota=", sizeof(Quota));
    vpw = vauth_getpw(ActionUser, Domain); 
    if ((strlen(Quota) == 0) || (strcmp (vpw->pw_shell, Quota) == 0)) {
      /* Blank or no change, do nothing */
    } else if (strncmp(Quota, "NOQUOTA", 2)==0) {
      if (vsetuserquota( ActionUser, Domain, Quota )) {
        snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[307]);  /* invalid quota */
      } else {
        snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[308]);
      }
    } else if (atoi(Quota)) {
      quotaptr = Quota;
      if (quota_to_bytes(qconvert, quotaptr)) {
        snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[307]);
      } else if(strcmp(qconvert, vpw->pw_shell)==0) {
        /* unchanged, do nothing */
      } else if(vsetuserquota( ActionUser, Domain, qconvert )) {
        snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[307]);
      } else { 
        snprintf (StatusMessage, sizeof(StatusMessage), html_text[309], qconvert);
      }
    } else {
      snprintf (StatusMessage, sizeof(StatusMessage)*2, "%s", html_text[307]);
    }
  }
#endif

  GetValue(TmpCGI,Gecos, "gecos=", sizeof(Gecos));

  vpw = vauth_getpw (ActionUser, Domain);

  /* check for the V_USERx flags and set accordingly */
  /* new code by Tom Collins <tom@tomlogic.com>, Dec 2004 */
  /* replaces code by James Raftery <james@now.ie>, 12 Dec. 2002 */

  orig_gid = vpw->pw_gid;
  for (i = 0; i < 4; i++) {
    GetValue (TmpCGI, box, (char *) flagfields[i], sizeof (box));
    if (strcmp (box, "on") == 0)
      vpw->pw_gid |= gidflags[i];
    else if (strcmp (box, "off") == 0)
      vpw->pw_gid &= ~gidflags[i];
  }

  /* we're trying to cut down on unnecessary updates to the password entry
   * we accomplish this by only updating if the pw_gid or gecos changed
   */

  if ((*Gecos != '\0') && (strcmp (Gecos, vpw->pw_gecos) != 0)) {
    vpw->pw_gecos = Gecos;
    vauth_setpw(vpw, Domain);  
  } else if (vpw->pw_gid != orig_gid) vauth_setpw (vpw, Domain);

  /* get value of the spam filter box */
  GetValue(TmpCGI, box, "spamcheck=", sizeof(box));
  if ( strcmp(box, "on") == 0 ) spam_check = 1;

  /* get the value of the vacation checkbox */
  GetValue(TmpCGI, box, "vacation=", sizeof(box));
  if ( strcmp(box, "on") == 0 ) vacation = 1;

  /* if they want to save a copy */
  GetValue(TmpCGI, box, "fsaved=", sizeof(box));
  if ( strcmp(box,"on") == 0 ) saveacopy = 1;

  /* get the value of the cforward radio button */
  GetValue(TmpCGI, cforward, "cforward=", sizeof(cforward));
  if ( strcmp(cforward, "vacation") == 0 ) vacation = 1;

  /* open old .qmail file if it exists and load it into memory */
  snprintf (dotqmailfn, sizeof(dotqmailfn), "%s/.qmail", vpw->pw_dir);
  err = stat (dotqmailfn, &sb);
  if (err == 0) {
    olddotqmail = malloc (sb.st_size);
    if (olddotqmail != NULL) {
      fs = fopen (dotqmailfn, "r");
      if (fs != NULL) {
        fread (olddotqmail, sb.st_size, 1, fs);
        fclose (fs);
      }
    }
  }
  
  fs = fopen (dotqmailfn, "w");

/**************************************************************************
  R. Puzzanghera 06/02/2024
  defaultdelivery patch

  If DEFAULT_DELIVERY is defined in vpopmail, do not allow installations
  of any delivery agent in user's .qmail if it already contains a copy of
  control/defaultdelivery. In fact, when an unknown (for qmailadmin)
  delivery agent is already defined there, and qmailadmin adds another one,
  users will get two messages for each delivery in their mailbox.
  Of course in this case the autoresponder is out of question, but users
  can create their out of office message by means of a plugin installed
  in their webmail.
 **************************************************************************/
#ifdef DEFAULT_DELIVERY
  FILE *file;
  int found = 0;
  char ln[MAX_BUFF], file_buf[MAX_BUFF];

  // open control/defaultdelivery
  snprintf(file_buf, sizeof(file_buf), "%s/control/defaultdelivery", QMAILDIR);
  file = fopen(file_buf, "r");
  if( file == NULL ) exit(127);

  // write out on ln the 1st line
  fgets(ln, sizeof(ln), file);
  fclose(file);
#endif
/* end patch */

  /* Scan through old .qmail and write out any unrecognized program delivery
   * lines to the new .qmail file.
   */
  emptydotqmail = 1;
  if (olddotqmail != NULL) {
    dotqmailline = strtok (olddotqmail, "\n");
    while (dotqmailline) {
#ifdef USE_QMAIL_AUTORESPONDER
      if ( (*dotqmailline == '|') &&
          (strstr (dotqmailline, "/true delete") == NULL) &&
          (strstr (dotqmailline, "/qmail-autoresponder ") == NULL) &&
          (strstr (dotqmailline, SPAM_COMMAND) == NULL) ) {
#else
      if ( (*dotqmailline == '|') &&
          (strstr (dotqmailline, "/true delete") == NULL) &&
          (strstr (dotqmailline, "/autorespond ") == NULL) &&
          (strstr (dotqmailline, SPAM_COMMAND) == NULL) ) {	
#endif
        fprintf (fs, "%s\n", dotqmailline);
        emptydotqmail = 0;
/* defaultdelivery patch */
#ifdef DEFAULT_DELIVERY
        if ( !found && strcmp(ln,dotqmailline) != 0 ) found = 1;
#endif
/* end patch */
      }
      dotqmailline = strtok (NULL, "\n");
    }
    free (olddotqmail);
  }

  /* Decide on what to write to the new .qmail file after any old program
   * delivery lines are written.
   */

  err = 0;
  
  /* note that we consider a .qmail file with just Maildir delivery to be empty
   * since it can be removed.
   */
   
  /* if they want to forward */
  if (strcmp (cforward, "forward") == 0 ) {

    /* get the value of the foward */
    GetValue(TmpCGI, box, "nforward=", sizeof(box));

    /* If nothing was entered, error */
    if ( box[0] == 0 ) {
      snprintf (StatusMessage, sizeof(StatusMessage), "%s\n", html_text[215]);
      err = 1;
    } else {

      tmpstr = strtok(box," ,;\n");

      /* tmpstr points to first non-token */

      count=0;
      while( tmpstr != NULL && count < MAX_FORWARD_PER_USER) {
        if ((*tmpstr != '|') && (*tmpstr != '/')) {
          if (strchr (tmpstr, '@') == NULL) {
            fprintf(fs, "&%s@%s\n", tmpstr, Domain);
          } else {
            fprintf(fs, "&%s\n", tmpstr);
          }
          emptydotqmail = 0;
          ++count;
        }
        tmpstr = strtok(NULL," ,;\n");
      }

    }
  }

  if ( (strcmp (cforward, "forward") != 0) || saveacopy ) {

    if (strcmp (cforward, "blackhole") == 0) {
      fprintf (fs, "# delete\n");
      emptydotqmail = 0;
    } else if (spam_check == 1) {
#ifdef MODIFY_SPAM_NEED_EMAIL
       fprintf(fs, "%s %s@%s\n", SPAM_COMMAND, ActionUser, Domain);
#else
      fprintf (fs, "%s\n", SPAM_COMMAND);
#endif
      emptydotqmail = 0;
    }

/* defaultdelivery patch */
#ifdef DEFAULT_DELIVERY
    else if ( !found ) {
      fprintf (fs, "%s/" MAILDIR "/\n", vpw->pw_dir);
    }
#else
    else {
      fprintf (fs, "%s/" MAILDIR "/\n", vpw->pw_dir);
      /* this isn't enough to consider the .qmail file non-empty */
    }
#endif
/* end patch */
  }

  if (vacation) {
    err = makevacation (fs, vpw->pw_dir);
    emptydotqmail = 0;
  } else {
    /* delete old vacation directory */
    snprintf (NewBuf, sizeof(NewBuf), "%s/vacation", vpw->pw_dir);
    vdelfiles (NewBuf);
  }

  fclose (fs);
  if (emptydotqmail) unlink (dotqmailfn);

  if (err) {
    moduser();
    vclose();
    exit(0);
  }
  
  call_hooks(HOOK_MODUSER, ActionUser, Domain, Password1, Gecos);
  moduser();
}


/* display ##i0 - ##i9 macros */
void parse_users_dotqmail (char newchar)
{
  static struct vqpasswd *vpw = NULL;
  static FILE *fs1=NULL; /* for the .qmail file */
  static FILE *fs2=NULL; /* for the vacation message file */
  int i, j;
  char fn[500];
  char linebuf[256];
  int inheader;
  
  static unsigned int dotqmail_flags = 0;
#define DOTQMAIL_STANDARD	(1<<0)
#define DOTQMAIL_FORWARD	(1<<1)
#define DOTQMAIL_SAVECOPY	(1<<3)
#define DOTQMAIL_VACATION	(1<<4)
#define DOTQMAIL_BLACKHOLE	(1<<8)
#define DOTQMAIL_SPAMCHECK	(1<<9)
#define DOTQMAIL_OTHERPGM	(1<<14)


  if (vpw == NULL) vpw = vauth_getpw(ActionUser, Domain);
  if (vpw == NULL) return;
  
  if (fs1 == NULL) {
    
    snprintf (fn, sizeof(fn), "%s/.qmail", vpw->pw_dir);
    fs1 = fopen (fn, "r");
    if (fs1 == NULL) {
      /* no .qmail file, standard delivery */
      dotqmail_flags = DOTQMAIL_STANDARD;
    } else {
      while (fgets (linebuf, sizeof(linebuf), fs1) != NULL) {
        i = strlen (linebuf);
        /* strip trailing newline if any */
        if (i && (linebuf[i-1] == '\n')) linebuf[i-1] = '\0';

        switch (*linebuf) {
          case '\0':	/* blank line, ignore */
            break;
            
          case '.':
          case '/':	/* maildir delivery */
            /* see if it's the user's maildir */
            if (1)
              dotqmail_flags |= DOTQMAIL_SAVECOPY;
            break;
            
          case '|':	/* program delivery */
            /* in older versions of QmailAdmin, we used "|/bin/true delete"
             * for blackhole accounts.  Since the path to true may vary,
             * just check for the end of the string
             */
            if (strstr (linebuf, "/true delete") != NULL)
              dotqmail_flags |= DOTQMAIL_BLACKHOLE;

#ifdef USE_QMAIL_AUTORESPONDER
			else if (strstr (linebuf, "/qmail-autoresponder ") != NULL) {
              dotqmail_flags |= DOTQMAIL_VACATION;
              snprintf (fn, sizeof(fn), "%s/vacation/message.txt", vpw->pw_dir);
              fs2 = fopen (fn, "r");
            }
#else
            else if (strstr (linebuf, "/autorespond ") != NULL) {
              dotqmail_flags |= DOTQMAIL_VACATION;
              snprintf (fn, sizeof(fn), "%s/vacation/message", vpw->pw_dir);
              fs2 = fopen (fn, "r");
            }
#endif
            else if (strstr (linebuf, SPAM_COMMAND) != NULL )
              dotqmail_flags |= DOTQMAIL_SPAMCHECK;
            
            else /* unrecognized program delivery, set a flag so we don't blackhole */
              dotqmail_flags |= DOTQMAIL_OTHERPGM;
                				
            break;
            
          case '#':	/* comment */
            /* ignore unless it's our 'blackhole' comment */
            if (strcmp (linebuf, "# delete") == 0)
              dotqmail_flags |= DOTQMAIL_BLACKHOLE;
            break;

          default:	/* email address delivery */
            dotqmail_flags |= DOTQMAIL_FORWARD;
            
        }
      }

      /* if other flags were set, in addition to blackhole, clear blackhole flag */
      if ( dotqmail_flags & (DOTQMAIL_FORWARD | DOTQMAIL_SAVECOPY) )
        dotqmail_flags &= ~DOTQMAIL_BLACKHOLE;
      
      /* if no flags were set (.qmail file without delivery), it's a blackhole */	
      if (dotqmail_flags == 0)
        dotqmail_flags = DOTQMAIL_BLACKHOLE;

      /* clear OTHERPGM flag, as it tells us nothing at this point */
      dotqmail_flags &= ~DOTQMAIL_OTHERPGM;
      
      /* if forward and save-a-copy are set, it will actually set the spam flag */
      if ((dotqmail_flags & DOTQMAIL_FORWARD) && (dotqmail_flags & DOTQMAIL_SPAMCHECK))
        dotqmail_flags |= DOTQMAIL_SAVECOPY;

      /* if forward is not set, clear save-a-copy */
      if (! (dotqmail_flags & DOTQMAIL_FORWARD))
        dotqmail_flags &= ~DOTQMAIL_SAVECOPY;

    }
    
    /* if deleted and forward aren't set, */        
    /* default to standard delivery */
    if (! (dotqmail_flags & (DOTQMAIL_BLACKHOLE | DOTQMAIL_FORWARD)) )
      dotqmail_flags |= DOTQMAIL_STANDARD;	
  }  

  switch (newchar) {
    case '0':	/* standard delivery checkbox */
    case '1':	/* forward delivery checkbox */
    case '3':	/* save-a-copy checkbox */
    case '4':	/* vacation checkbox */
    case '8':	/* blackhole checkbox */
    case '9':	/* spam check checkbox */
      if (dotqmail_flags & (1 << (newchar - '0'))) printf ("checked ");
      break;
      
    case '2':	/* forwarding addresses */
      if (fs1 != NULL) {
        rewind (fs1);
        j = 0;
        while (fgets (linebuf, sizeof(linebuf), fs1) != NULL) {
          i = strlen (linebuf);
          /* strip trailing newline if any */
          if (i && (linebuf[i-1] == '\n')) linebuf[i-1] = '\0';
          switch (*linebuf) {
            case '\0':	/* blank line */
            case '/':	/* maildir delivery */
            case '|':	/* program delivery */
            case '#':	/* comment */
              /* ignore */
              break;
  
            default:	/* email address delivery */
              /* print address, skipping over '&' if necessary */
              if (j++) printf (", ");
              printh ("%H", &linebuf[(*linebuf == '&' ? 1 : 0)], "\n");
          }
        }
      }
      break;
      
    case '5':	/* vacation subject */
      if (fs2 != NULL) {
        rewind (fs2);

        /* scan headers for Subject */
        while (fgets (linebuf, sizeof(linebuf), fs2) != NULL) {
          if (*linebuf == '\n') break;
          if (strncasecmp (linebuf, "Subject: ", 9) == 0)
            printh ("%H", &linebuf[9]);
        }
      }
      break;
      
    case '6':	/* vacation message */
      if (fs2 != NULL) {
        rewind (fs2);

        /* read from file, skipping headers (look for first blank line) */
        inheader = 1;
        while (fgets (linebuf, sizeof(linebuf), fs2) != NULL) {
          if (!inheader) printh ("%H", linebuf);
          if (*linebuf == '\n') inheader = 0;
        }
      }
      break;
      
    case '7':	/* gecos (real name) */
      printh ("%H", vpw->pw_gecos);
      break;
  }
  
}

