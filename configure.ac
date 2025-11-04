dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([qmailadmin],[1.2.25])
AC_CONFIG_SRCDIR([qmailadmin.c])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

AC_DEFINE_UNQUOTED(QA_PACKAGE,"$PACKAGE_NAME", Package name)
AC_DEFINE_UNQUOTED(QA_VERSION,"$PACKAGE_VERSION", Package version)
AC_SUBST(QA_PACKAGE)
AC_SUBST(QA_VERSION)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_SEARCH_LIBS([strerror],[cposix])

dnl Checks for libraries.
AC_CHECK_LIB(crypt,crypt)
AC_CHECK_LIB(shadow,crypt)
AC_CHECK_LIB(crack,FascistCheck)
#AC_CHECK_LIB(m,floor)
#AC_CHECK_LIB(nsl, gethostbyaddr)
#AC_CHECK_LIB(socket, getsockname)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
#AC_HEADER_STDBOOL
AC_TYPE_UID_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
#AC_FUNC_FORK
#AC_FUNC_LSTAT
#AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC
#AC_FUNC_STAT
AC_CHECK_FUNCS([dup2 memset mkdir strcasecmp strchr strdup strncasecmp strrchr strstr])

# commented out as it prevents CFLAGS to be overwritten
#if test "$GCC" = yes ; then
#  CFLAGS="-Wall"
#fi

CGIBINTARGETS=qmailadmin
AC_SUBST(CGIBINTARGETS)

dnl ===============================================================
dnl  Check for vpopmail headers and version >= 5.6.6
dnl ===============================================================

vpopmaildir=""
for f in ~vpopmail
do
    if test -d $f
    then
        vpopmaildir=$f
    fi
done

AC_ARG_ENABLE(vpopmaildir, [  --enable-vpopmaildir=DIR  Home directory of vpopmail.],
    vpopmaildir="$enableval",
    [
        if test "$vpopmaildir" = ""
        then
            AC_MSG_ERROR([Unable to find your vpopmaildir, specify --enable-vpopmaildir.])
        fi
    ] )
AC_SUBST(vpopmaildir)

auth_incs=`cat $vpopmaildir/etc/inc_deps`
AC_SUBST(auth_incs)
auth_libs=`cat $vpopmaildir/etc/lib_deps`
AC_SUBST(auth_libs)

CPPFLAGS="$CPPFLAGS -I$vpopmaildir/include"
LDFLAGS="$LDFLAGS -L$vpopmaildir/lib"

AC_CHECK_HEADERS([vpopmail.h],
    [have_vpopmail_h=yes],
    [AC_MSG_ERROR([vpopmail.h not found in $vpopmaildir/include])])

AC_MSG_CHECKING([for vpopmail version >= 5.6.6])

AC_RUN_IFELSE(
  [AC_LANG_PROGRAM(
     [[#include <config.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <string.h>]],
     [[
       const char *ver = VERSION;
       int major=0, minor=0, patch=0;
       if (sscanf(ver, "%d.%d.%d", &major, &minor, &patch) != 3) {
         printf("no, failed in parsing vpopmail version\n");
         return 1;
       }
       if (major < 5 || (major == 5 && minor < 6) || (major == 5 && minor == 6 && patch < 6)) {
         printf("no, found %s\n", ver);
         return 1;
       }
       printf("yes, found %s\n", ver);
       return 0;
     ]]
  )],
  [],
  [AC_MSG_ERROR([vpopmail version is too old, need >= 5.6.6])]
)

dnl ===============================================================

htmldir=""
for f in /home/httpd/html /usr/local/apache/htdocs /usr/local/www/data /var/www/html
do
    if test -d $f
    then
        htmldir=$f
    fi
done
AC_ARG_ENABLE(htmldir, [  --enable-htmldir=DIR      HTTP server's base html dir.],
    htmldir="$enableval",
    [
        if test "$htmldir" = ""
        then
            AC_MSG_ERROR([Unable to find your html base directory, specify --enable-htmldir.])
        fi
    ] )
AC_SUBST(htmldir)


cgibindir=""

for f in /home/httpd /usr/local/apache /usr/local/www /var/lib/httpd /var/www /usr/local/www $htmldir
do
    if test -d $f/cgi-bin
    then
        cgibindir=$f/cgi-bin
    fi
done

AC_ARG_ENABLE(cgibindir, [  --enable-cgibindir=DIR    HTTP server's cgi-bin directory.],
    cgibindir="$enableval",
    [
        if test "$cgibindir" = ""
        then
            AC_MSG_ERROR([Unable to find your cgi-bin directory, specify --enable-cgibindir.])
        fi
    ] )
AC_SUBST(cgibindir)

imageurl="/images/qmailadmin"
AC_ARG_ENABLE(imageurl, [  --enable-imageurl=URL     URL to image files [/images/qmailadmin].],
    imageurl="$enableval",
    [
        if test "$imageurl" = ""
        then
            AC_MSG_ERROR([Unable to find your html url, specify --enable-imageurl.])
        fi
    ] )
AC_SUBST(imageurl)
AC_DEFINE_UNQUOTED(IMAGEURL, "$imageurl","")

imagedir=""
for f in /home/httpd/html /usr/local/apache/htdocs /usr/local/www/data /var/www/html $htmldir
do
    if test -d $f
    then
        imagedir=$f$imageurl
    fi
done
AC_ARG_ENABLE(imagedir, [  --enable-imagedir=DIR     Where to put the image files [DocumentRoot/images/qmailadmin].],
    imagedir="$enableval",
    [
        if test "$imagedir" = ""
        then
            AC_MSG_ERROR([Unable to find your html image directory, specify --enable-imagedir.])
        fi
    ] )
AC_SUBST(imagedir)

htmllibdir="/usr/local/share/qmailadmin"

AC_ARG_ENABLE(htmllibdir, [  --enable-htmllibdir=DIR   qmailadmin HTML library directory.],
    htmllibdir="$enableval",
    [
        if test "$htmllibdir" = ""
        then
            AC_MSG_ERROR([Unable to find your htmllibdir directory, specify --enable-htmllibdir.])
        fi
    ] )
AC_SUBST(htmllibdir,$htmllibdir)
AC_DEFINE_UNQUOTED(HTMLLIBDIR, "$htmllibdir","")


qmaildir=""
for f in /var/qmail
do
    if test -d $f
    then
        qmaildir=$f
    fi
done

AC_ARG_ENABLE(qmaildir, [  --enable-qmaildir=DIR     directory where qmail is installed.],
    qmaildir="$enableval",
    [
        if test "$qmaildir" = ""
        then
            AC_MSG_ERROR([Unable to find your qmaildir directory, specify --enable-qmaildir.])
        fi
    ] )
AC_DEFINE_UNQUOTED(QMAILDIR,"$qmaildir","")

ezmlmdir=""
for f in /usr/local/bin/ezmlm /usr/bin /usr/sbin /usr/local/bin
do
    if test -f $f/ezmlm-make
    then
        ezmlmdir=$f
    fi
done

AC_ARG_ENABLE(ezmlmdir, [  --enable-ezmlmdir=PATH    Path to ezmlm binaries or =n to disable.],
    ezmlmdir="$enableval",
    [
        if test "$ezmlmdir" = ""
        then
            AC_MSG_ERROR([Unable to find your ezmlmdir user, specify --enable-ezmlmdir.])
        fi
    ] )
AC_DEFINE_UNQUOTED(EZMLMDIR,"$ezmlmdir","")

cgipath="/cgi-bin/qmailadmin"

AC_ARG_ENABLE(cgipath, [  --enable-cgipath=PATH     URL path for cgi [/cgi-bin/qmailadmin].],
    cgipath="$enableval",
    [
        if test "$cgipath" = ""
        then
            AC_MSG_ERROR([Unable to find your cgipath directory, specify --enable-cgipath.])
        fi
    ] )
AC_DEFINE_UNQUOTED(CGIPATH,"$cgipath","")

autorespond_path=""
for f in /usr/local/bin /usr/bin /usr/sbin /usr/local/bin /var/qmail/bin
do
    if test -f $f/autorespond
    then
        autorespond_path=$f
    fi
done
AC_ARG_ENABLE(autoresponder-path, [  --enable-autoresponder-path=PATH   Path to autoresponder directory.],
AC_ARG_ENABLE(autoresponder-bin, [], autorespond_path="$enableval", [])
    autorespond_path="$enableval",
    [
        if test "$autorespond_path" = ""
        then
            AC_MSG_ERROR([Unable to find your autorespond program directory, specify --enable-autoresponder-path.])
        fi
    ] )
AC_DEFINE_UNQUOTED(AUTORESPOND_PATH,"$autorespond_path","")

qmailautoresponder=no
AC_ARG_ENABLE(qmail-autoresponder, [  --enable-qmail-autoresponder  Replace autorespond by qmail autoresponder.],
    qmailautoresponder="$enableval",)
case $qmailautoresponder in
1*|y*|Y*)
	qmailautoresponder=yes
        AC_DEFINE_UNQUOTED(USE_QMAIL_AUTORESPONDER,1,"")
        ;;
*)
        qmailautoresponder=no
        ;;
esac

vpopuser="vpopmail"
AC_ARG_ENABLE(vpopuser, [  --enable-vpopuser=USER        User vpopmail was installed as [vpopmail].],
    vpopuser="$enableval",
    [
        if test "$vpopuser" = ""
        then
            AC_MSG_ERROR([Unable to find your vpopuser user, specify --enable-vpopuser.])
        fi
    ] )
AC_DEFINE_UNQUOTED(VPOPUSER,"$vpopuser","")
AC_SUBST(vpopuser)
   
vpopgroup="vchkpw"
AC_ARG_ENABLE(vpopgroup, [  --enable-vpopgroup=GROUP      Group vpopmail was installed as [vchkpw].],
    vpopgroup="$enableval",
    [
        if test "$vpopgroup" = ""
        then
            AC_MSG_ERROR([Unable to find your vpopgroup group, specify --enable-vpopgroup.])
        fi
    ] )
AC_DEFINE_UNQUOTED(VPOPGROUP,"$vpopgroup","")
AC_SUBST(vpopgroup)

mysql=yes
AC_ARG_ENABLE(ezmlm-mysql, [  --disable-ezmlm-mysql         Disable MySQL options for mailing lists.],
    mysql="$enableval",)
case $mysql in
1*|y*|Y*)
	mysql=yes
        AC_DEFINE_UNQUOTED(ENABLE_MYSQL,1,"")
        ;;
*)
        mysql=no
        ;;
esac

maxusersperpage=15
AC_ARG_ENABLE(maxusersperpage, [  --enable-maxusersperpage=#    Number of users to display on a page [15].],
    maxusersperpage="$enableval",)
AC_DEFINE_UNQUOTED(MAXUSERSPERPAGE,$maxusersperpage,"")

maxaliasesperpage=25
AC_ARG_ENABLE(maxaliasesperpage, [  --enable-maxaliasesperpage=#  Number of aliases to display on a page [25].],
    maxaliasesperpage="$enableval",)
AC_DEFINE_UNQUOTED(MAXALIASESPERPAGE,$maxaliasesperpage,"")

NO_CACHE=no
AC_ARG_ENABLE(no-cache, [  --enable-no-cache   Prohibit caching via http headers.],
    NO_CACHE="$enableval",)

case $NO_CACHE in
1*|y*|Y*)
	NO_CACHE=1
        AC_DEFINE_UNQUOTED(NO_CACHE,$NO_CACHE,"")
        echo "checking whether we should prohibit caching... yes"
        ;;
*)
        echo "checking whether we should prohibit caching... no"
        ;;
esac

IPAUTH=yes
AC_ARG_ENABLE(ipauth, [  --disable-ipauth    Disable IP address checks (NAT/proxy systems may not use the same IP for the whole session).],
    IPAUTH="$enableval",)

case $IPAUTH in
1*|y*|Y*)
	IPAUTH=1
        AC_DEFINE_UNQUOTED(IPAUTH,$IPAUTH,"")
        echo "checking whether to require ip authentication... yes"
        ;;
*)
        echo "checking whether to require ip authentication... no"
        ;;
esac

USER_INDEX=yes
AC_ARG_ENABLE(user-index, [  --disable-user-index   Don't display user index on show users page.],
    USER_INDEX="$enableval",)

case $USER_INDEX in
1*|y*|Y*)
	USER_INDEX=1
        AC_DEFINE_UNQUOTED(USER_INDEX,$USER_INDEX,"")
        echo "checking whether to display user index... yes"
        ;;
*)
        echo "checking whether to display user index... no"
        ;;
esac

AC_ARG_ENABLE(modify-quota, [  --enable-modify-quota  Allow domain admin (postmaster) to modify user quotas.],
    MODIFY_QUOTA="$enableval",)

case $MODIFY_QUOTA in
1*|y*|Y*)
	MODIFY_QUOTA=yes
        AC_DEFINE_UNQUOTED(MODIFY_QUOTA,1,"")
        echo "checking whether to allow admins to modify user quotas... yes"
        ;;
*)
        MODIFY_QUOTA=no
        echo "checking whether to allow admins to modify user quotas... no"
        ;;
esac

AC_ARG_ENABLE(domain-autofill, [  --enable-domain-autofill  Autofill the domain on login page based on the hostname that accesses qmailadmin.],
   DOMAIN_AUTOFILL="$enableval",)

case $DOMAIN_AUTOFILL in
1*|y*|Y*)
      DOMAIN_AUTOFILL=yes
      AC_DEFINE_UNQUOTED(DOMAIN_AUTOFILL,1,"")
      echo "checking whether to autofill domain name on login page... yes"
      ;;
*)
      DOMAIN_AUTOFILL=no
      echo "checking whether to autofill domain name on login page... no"
      ;;
esac


CATCHALL_ENABLED=yes
AC_ARG_ENABLE(catchall, [  --disable-catchall  Disable the use of catchall accounts.],
   CATCHALL_ENABLED="$enableval",)

case $CATCHALL_ENABLED in
1*|y*|Y*)
      CATCHALL_ENABLED=yes
      AC_DEFINE_UNQUOTED(CATCHALL_ENABLED,1,"")
      echo "checking whether to enable catchall accounts... yes"
      ;;
*)
      CATCHALL_ENABLED=no
      echo "checking whether to enable catchall accounts... no"
      ;;
esac


TRIVIAL_PASSWORD_ENABLED=yes
AC_ARG_ENABLE(trivial-password, [  --disable-trivial-password  Disable the use of trivial password.],
   TRIVIAL_PASSWORD_ENABLED="$enableval",)

case $TRIVIAL_PASSWORD_ENABLED in
1*|y*|Y*)
      TRIVIAL_PASSWORD_ENABLED=yes
      AC_DEFINE_UNQUOTED(TRIVIAL_PASSWORD_ENABLED,1,"")
      echo "checking whether to enable trivial password... yes"
      ;;
*)
      TRIVIAL_PASSWORD_ENABLED=no
      echo "checking whether to enable trivial password... no"
      ;;
esac

AC_ARG_ENABLE(spamcmd-needs-email, [  --enable-spamcmd-needs-email  Append the user's email to the end of the spam command.],
    SPAM_NEED_EMAIL="$enableval",)

case $SPAM_NEED_EMAIL in
1*|y*|Y*)
      SPAM_NEED_EMAIL=yes
      AC_DEFINE_UNQUOTED(MODIFY_SPAM_NEED_EMAIL,1,"")
      echo "checking whether to append the email to the end of the spam command... yes"
      ;;
*)
      SPAM_NEED_EMAIL=no
      echo "checking whether to append the email to the end of the spam command... no"
      ;;
esac

AC_ARG_ENABLE(modify-spam, [  --enable-modify-spam  Allow users to enable/disable spam checking.],
    MODIFY_SPAM="$enableval",)

case $MODIFY_SPAM in
1*|y*|Y*)
      MODIFY_SPAM=yes
      AC_DEFINE_UNQUOTED(MODIFY_SPAM,1,"")
      echo "checking whether to allow spam check enable/disable... yes"
      ;;
*)
      MODIFY_SPAM=no
      echo "checking whether to allow spam check enable/disable... no"
      ;;
esac

spam_command="|preline /usr/local/bin/maildrop /etc/mailfilter"
AC_ARG_ENABLE(spam_command, [  --enable-spam-command=CMD     Remember to use '|preline' at the beginning, [|preline /usr/local/bin/maildrop /etc/mailfilter].],
    spam_command="$enableval",
)
AC_DEFINE_UNQUOTED(SPAM_COMMAND, "$spam_command","")
AC_SUBST(SPAM_COMMAND)

AC_ARG_ENABLE(cracklib, [  --enable-cracklib=DIR      Path to cracklib dictionary directory.], cracklib="$enableval", cracklib="no")

case $cracklib in
0*|n*|N*)
     # --enable-cracklib=no|n|N|0
     cracklib="disabled"
     echo "checking whether to use password checking... no"
     ;;
1*|y*|Y*)
     # --enable-cracklib=yes|y|Y|1
     cracklib=""
     for f in /usr/share/cracklib /usr/local/share/cracklib /usr/share/cracklib_dict /usr/share/cracklib_dict /usr/lib/cracklib /usr/lib/cracklib_dict
     do
       if test -d "$f"
       then
         cracklib="${f}/pw_dict"
         echo "checking whether to use password checking... yes"
         break
       fi
     done

     if test -z $cracklib
     then
       AC_MSG_ERROR([Unable to find your cracklib dictionary directory, specify --enable-cracklib=DIR])
     fi

     AC_DEFINE_UNQUOTED(CRACKLIB,"$cracklib","")
     ;;
*)
     # --enable-cracklib with no argument
     if test -z "$cracklib"
     then
       for f in /usr/share/cracklib /usr/local/share/cracklib /usr/share/cracklib_dict /usr/share/cracklib_dict /usr/lib/cracklib /usr/lib/cracklib_dict
       do
         if test -d "$f"
         then
           cracklib="${f}/pw_dict"
           break
         fi
       done
     else
       # test dictionary dir --enable-cracklib=DIR
       if test -d "$cracklib"
       then
         cracklib="${cracklib}/pw_dict"

       # test dictionary path --enable-cracklib=PATH
       else
         if test -f "${cracklib}.hwm"
         then
           cracklib="$cracklib"
         else
           # cracklib not found
           cracklib=""
         fi
       fi
     fi

     # test if anything has been found
     if test -z $cracklib
     then
       AC_MSG_ERROR([Unable to find your cracklib dictionary directory, specify --enable-cracklib=DIR])
     fi

     echo "checking whether to use password checking... yes"
     AC_DEFINE_UNQUOTED(CRACKLIB,"$cracklib","")

     ;;
esac

HELP=no
AC_ARG_ENABLE(help, [  --enable-help  Display help links on login page.],
    HELP="$enableval",)

case $HELP in
1*|y*|Y*)
        enable_help="yes"
        AC_SUBST(enable_help)
        AC_DEFINE_UNQUOTED(HELP,1,"")
        echo "checking whether to show help pages... yes"
        ;;
*)
        enable_help="no"
        echo "checking whether to show help pages... no"
        ;;
esac


EZMLMIDX=no
if test -f $ezmlmdir/ezmlm-moderate
then
	EZMLMIDX=yes
fi

case $EZMLMIDX in
1*|y*|Y*)
                EZMLMIDX=1
                AC_DEFINE_UNQUOTED(EZMLMIDX,$EZMLMIDX,"")
		echo "checking for ezmlm-idx... yes"
		ezmlmidx="yes"
        ;;
*)
		echo "checking for ezmlm-idx... no"
		ezmlmidx="no"
        ;;
esac

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo ""
echo "          $PACKAGE_NAME $PACKAGE_VERSION"
echo "             Current settings"
echo "---------------------------------------------------------------"
echo "        cgi-bin dir = "$cgibindir""
echo "           html dir = "$htmldir""
echo "          image dir = "$imagedir""
echo "          image URL = "$imageurl""
echo "       template dir = "$htmllibdir""
echo "          qmail dir = "$qmaildir""
echo "       vpopmail dir = "$vpopmaildir""
echo "    autorespond dir = "$autorespond_path""
echo "          ezmlm dir = "$ezmlmdir""
echo "          ezmlm idx = "$ezmlmidx""
echo "    mysql for ezmlm = "$mysql""
echo "               help = "$enable_help""
echo "       modify quota = "$MODIFY_QUOTA""
echo "    domain autofill = "$DOMAIN_AUTOFILL""
echo "  catchall accounts = "$CATCHALL_ENABLED""
echo "  trivial passwords = "$TRIVIAL_PASSWORD_ENABLED""
echo "  modify spam check = "$MODIFY_SPAM""
if test "$MODIFY_SPAM" = "yes"
then
echo "       spam command = "$spam_command""
fi
echo "cracklib dictionary = "$cracklib""
if test "$qmailautoresponder" != "no"
then
echo "      autoresponder = qmail autoresponder"
fi
