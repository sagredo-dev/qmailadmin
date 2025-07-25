# ChangeLog

## 1.2.24
Roberto Puzzanghera

  - definition of CFLAGS in configure.ac prevented CFLAGS to be overwritten
  - cracklib check improved in configure.ac
  - char *tmpstr in qmailadmin.c now under #ifdef CRACKLIB to avoid compilation warnings  
    in case CRACKlib is not defined
  - qmailadmin now shows an error message in case the change pwd failed due to the pwd  
    strength feature in vpopmail
  - dropped zombie function modusernow() from user.c
  - fix: pwd changed with success messages was printed twice
  - bug fix on pwd strength check
  - pwd strength output now is always shown

## 1.2.23
Nathanaël Semhoun 

  - Added support for qmail-autoresponder
  - Fixed load lang not retrieved

## 1.2.22
Roberto Puzzanghera

  - owner no longer required in autorespond
  - template.c code optimization

## 1.2.21
Roberto Puzzanghera

  - Bug fix in mailinglist.c which was preventing the display of the subscribers to the list

## 1.2.20
Roberto Puzzanghera

  - Bug fix in mailinglist.c which was preventing the creation of the m/l

## 1.2.19 
Roberto Puzzanghera

  - if DEFAULT_DELIVERY is defined in vpopmail, do not allow installations of any delivery
    agent in user's .qmail if it already contains a copy of control/defaultdelivery.
    With respect to the previous version, the program recognizes if the control/defaultdelivery
    line has been copied to the user's mailbox. If not, qmailadmin installs the delivery
    agent when enabling the copy&forward.

## 1.2.18
Roberto Puzzanghera

  - If DEFAULT_DELIVERY is defined in vpopmail, do not allow user's .qmail modifications.
    In fact, when an unknown (for qmailadmin) delivery agent is already defined there, and
    qmailadmin adds another one, users will get two messages for each delivery in their
    mailbox.
  - undef macros that get redefined in config.h
  - solved all gcc compilation warnings
  - configure.in moved to configure.ac
  - configure.ac modernization
  - dropped autotools warnings

## 1.2.17
Roberto Puzzanghera

  - Added qmailadmin-skin patch, which provides a new responsive skin to the control panel.
    It modifies everything under the html dir and many .c files in order to adjust the html
    embedded into the source files. Added a style sheet in the "images" folder and a couple
    of png files for the qmail logo.
  - Added a patch to call cracklib in order to check for the password strenght. This should
    avoid unsafe accounts created by domain administrators such as "test 123456". This
    patch is based on an original work by Inter7.
  - A patch gets qmailadmin to have authentication failures logged. This makes possible to
    ban malicious IPs via fail2ban. It is required to create the log file
    /var/log/qma-auth.log initially and assign write priviledges to apache.
  - ezmlm-idx 7 compatibility patch (author unknown), which restores the compatibility with
    ezmlm-idx-7 (thanks to J.D. Trolinger for the advice).
  - a fix to the catchall account (thanks to Luca Franceschini).
  - a fix to autorespond.c to correct the way .qmail files are modified.

New patches can be found in the "contrib" folder.

More info [here](https://www.sagredo.eu/en/qmail-notes-185/qmailadmin-23.html)

-----------------------------------------

Numbers in square brackets ([]) indicate tracker item on SourceForge with
patch or information related to the entry.

1.2.14-1.2.16

    Matt Brookings
    - Removed mailbox image
	- Updated quota code to work with vpopmail v5.4.33
	- Fixed some compiler warnings
	- Updated default compiler flags
	- Updated package version string
	- Fixed some format string warnings
	- Reserved language template file codes 400 through 450 for custom autoresponder headers
	- Added code to support custom autoresponder headers per language

1.2.13 - released 2-May-09

	Simone Lazzaris
	- Add --disable-catchall and --disable-trivial-password options
	  to configure scripts.

	Tom Collins
	- qmailadmin.c: check return value of setuid() and setgid() and 
	  exit if either fail (security issue reported by Miguel Lourenco).
	- template.c: fix for potential XSS (cross-site scripting) hole
	  in the login page (reported by Michael Middleton).
	- mailinglist.c: Increase size of tmp buffer used to delete 
	  orphan .qmail files when removing a mailing list (reported by 
	  William Fuller).
	- auth.c: Fix compiler warning.  Variable ip_value only used if 
	  IPAUTH is defined.
	- util.c: minor performance improvement (use strchr instead of
	  strstr) in open_lang().
	- command.c: disabled catchall commands unless enabled

	Chaz Owensby
	- Report disk usage to non-admin user on their Modify User page.

	Tomas Kuliavas
	- Updated Lithuanian translation. [1913361]

	Dvorkin Dmitry
	- Updated Russian translation (now in utf8).

	Ismail YENIGUL
	- qmailadmin.c: check return value of open_lang() in load_lang()
	  and exit on failure.

1.2.12 - released 13-Nov-07

	John Simpson
	- mailinglist.c: call vpopmail's onchange onchange handler when
	  adding or removing a mailing list.

	Jorgen Moth
	- user.c: Make sure number_of_mailinglists > 0 before malloc() in 
	  addusernow(). [1647157]

	Tom Collins
	- Use function to generate cgi URLs.
	- Remove unused AliasType variable.
	- Use email address instead of maildir for catchall.
	- Move HTML out of alias.c and into html.h as move toward more
	  flexible HTML templates.
	- When deleting a subscriber, convert '=' to '@' if no '@'. [1651370]
	- Slight change to Jorgen Moth's patch.
	- mailinglist.c: make sure name of list to delete is valid.

1.2.11 - released 29-Dec-06

	Brandon
	- Add missing semi-colons to &nbsp; tags in user.c. [1537587]

	Andreas Tolfsen
	- Added Norwegian translation for strings 316 and 317.

	Juergen Kendzorra
	- Add strings 316 & 317 to German translation, plus fix some
	  grammatical errors.

	Nicolas Croiset
	- Add strings 316 & 317, fix string 267 in French translation.

	Oden Eriksson
	- Add string 316 to Swedish translation.

	Dave Steinberg
	- Add --enable-spamcmd-needs-email option to configure to have 
	  QmailAdmin include the user's email address as the last 
	  parameter to --enable-spam-command.

	Tom Collins
	- Create autoresponder message before creating its .qmail file.
	- Deliver copy before autoresponding to ensure delivery.
	- On Modify User page, add current domain to forwarding address
	  if it doesn't already include a domain. [1508971, 1545664]
	- Don't try to add list owner as moderator of new unmoderated list.

1.2.10 - released 20-Feb-06

	Peter Pentchev
	- Update Bulgarian translation. [1421770]
	- qmailadmin.c: Fix potential buffer overflow.
	- alias.c: missing semicolon when using VALIAS, eliminate unused
	  variable warnings when using VALIAS.

	Tom Collins
	- mailinglist.c: Fix problem with enable/disable ezmlm-cgi access
	  to archives.
	- alias.c: Add index and user search to the aliases page.  When
	  deleting an alias, return to the page that alias would have been
	  on.  This feature was sponsored by Marko Lerota. [894823, 877895]
	- qmailadmin.h: undef MAX_BUFF if previously defined (too small) by 
	  vpopmail.h.  Include vpopmail.h before qmailadmin.h so we can
	  always redefine MAX_BUFF.

1.2.9 - released 1-Sep-05

	Luis Gonzalo Constantini Von Rickel
	- util.c: free sort_list in sort_cleanup(). [1182347]

	HA van Tilborg
	- Added Dutch translations for strings 316 and 317. [1255845]

	Kimmo Hedman [by way of Bill Shupp]
	- Update Finnish translations of strings 114 & 137, add 316.

	Tom Collins
	- qmailadmin.c: fix problem parsing language files that could
	  result in SIGSEGV. [1242317]
	- qmailadmin.c: disable ability to delete postmaster account
	  from Quick Links on main menu. [1255845]

1.2.8 - released 4-Jul-05

	Tom Collins
	- Added single page for changing password. [775208]
	- Switch to improved method of handling strings in language files,
	  resulting in reduced disk access and faster rendering of pages.
	- mailinglists: Update settings screen to show that archive 
	  retrieval limitations don't apply to ezmlm-cgi.
	- mailinglist.c: Fix compile error with non-idx ezmlm installs.
	- lang/en: More descriptive entry for "Standard" email delivery on 
	  Modify User page.

	L4nce1ot/honeymak
	- mailinglist.c: Fix Tom's fix for non-idx ezmlm installs. [1123809]

	sunezapa
	- Fix entry 303 ("Delete Mail" instead of "None") and 205
	  ("Standard (ingen videresendelse)" instead of "Standard")
	  in Danish translation. [1152997]

	Martin Dimov
	- Added Czech translation.

1.2.7 - released 23-Jan-05

	Tomas Kuliavas
	- Added Lithuanian translation.

	John Peacock
	- template.c: Fix typo that broke autoresponder subject display.

	Tom Collins
	- Add Quick Links section to main menu.  Admins can now enter an
	  email address and go directly to the modify or delete page,
	  regardless of address type (mailbox, forward, list, etc.).
	  Thanks to PinkRoccade Public Sector for sponsoring this feature.
	- Modify User: For clarity, change "Deleted" to "All mail deleted"
	  and "Vacation" to "Send Vacation Auto-Repsonse".
	- template.c: Add macro for current language (##~) and document
	  in html/README.  [818883]
	- user.c: Fix bug from 1.2.6 related to creating .qmail files when
	  one didn't already exist.
	- INSTALL: Add note about running `libtoolize --force` on x86_64.
	- Update config.guess and config.sub scripts.
	- Include charset in HTTP Content-Type header. [1098891]

1.2.6 - released 24-Dec-04

	Tom Collins
	- user.c: fix bug in handling .qmail files with blank lines.
	- user.c: new catchall option, forward unknown users to remote
	  domain (e.g., x -> x@dom.com, y -> y@dom.com, etc.)  To use,
	  enter "@<domain>" for remote address catchall. [910410]
	- util.c: fix check_email_addr() (previously accepted invalid 
	  punctuation in domain part of address).
	- user.c: modusergo() function that writes out user's .qmail file
	  now keeps unrecognized program delivery lines. [994319]
	- Change 'Vacation' option to checkbox on 'Modify User' page.  Can
	  now have vacation response with forward (and delete). [800254]

1.2.5 - released 1-Dec-04

	Tom Collins
	- Remove unnecessary name attributes in SUBMIT buttons. [869292]
	- When adding a mailing list, automatically subscribe the
	  list owner and make them a moderator/remote admin. [892489]
	- user.c: deliver mail to Maildir before sending vacation message
	  in order to prevent autorespond from losing messages. [777612]
	- Move .qmail parsing code from template.c to user.c.  Rewrite
	  for more flexibility and to allow new format for vacation.
	- configure.in: Compile with all warnings on.
	- Add header files: alias.h auth.h autorespond.h cgi.h command.h 
	  dotqmail.h forward.h limits.h mailinglist.h show.h template.h 
	  user.h util.h.
	- Modify .c files to use header files, fix function prototypes,
	  definitions and calls for consistent parameter passing and
	  return values.
	- Modify code to eliminate all compiler warnings.

1.2.4 - released 15-Nov-04

	Tom Collins
	- Modify contrib/alias2forward.pl to work with '/Maildir' or
	  '/.maildir' directory names.
	- Add #define to qmailadmin.h for globally setting Maildir directory
	  name (defaults to "/Maildir" but Gentoo can use ".maildir").
	- Better detect .qmail-alias files that are tied to mailing lists.
	  (Aliases that end in "-owner" but aren't tied to ezmlm lists 
	  will now display properly.)
	- Add printh.c, new routines for generating HTML-safe and CGI-safe
	  strings.
	- Convert sprintf calls to snprintf to avoid buffer overflow.
	- Changes to almost all .c and .html files to make use of printh
	  routines.  QmailAdmin should now properly handle email addresses
	  that contain special characters (like '+' and '&'), including
	  domain admin addresses.  It now also escapes user-supplied text
	  to avoid possible HTML-insertion and cross site scripting attacks.

1.2.3 - released 7-Oct-04

	Ken Jones
	- Remove check for C++ compiler in configure.in.

	Rick Widmer
	- Updated INSTALL documentation (from 2004-05-07).

	Charlie Garrison
	- Fix invalid HTML in mod_autorespond.html.

	Maurice Snellen
	- Fix lang/nl entry 152 (was opposite of English original).

	Tom Collins
	- Before considering an alias/forward to be a 'blackhole', make
	  sure it's only a single line.
	- Ignore .qmail-alias files if 'alias' ends in '-owner'.  We assume
	  it's for a mailing list and isn't an alias.  Note that this only
	  affects systems that don't store aliases in the valias table and
	  reverts to pre-1.2.2 behavior. [992132]
	- Fix error handling when attempting to set catchall (if
	  vauth_getpw() failed, we'd end up with an empty .qmail-default).

1.2.2 - released 25-Jun-04

	Oden Eriksson
	- Added "lazy snap to" javascripts to have the cursor jump to the
	  most interesting input field. The idea was stolen from Mr. Sam's
	  sqwebmail package. [897088]

	Zhao Xin
	- Add Traditional Chinese (zh-cn) translation.	

	Tom Collins
	- Allow user to create a 'blackhole' address by entering "#" as 
	  the forwarding address when creating a new alias.
	- Display forwards even if they're all blackholes. [941083]
	- Fixed bug where unrecognized .qmail-alias entries could throw
	  QmailAdmin into an infinite loop (Modify Alias). [941082]
	- Display non-ezmlm aliases with "-owner" in the name.
	- Display aliases with "-default" in the name.
	- Use "# delete" instead of "|/bin/true delete" for blackholed
	  accounts.
	- Remove --enable-true-path from configure (no longer used).

1.2.1 - released 10-Feb-04

	Tom Collins
	- Add note to INSTALL about requiring vpopmail 5.4.0 or later.
	- Store aliases and autoresponders in valias table if vpopmail was
	  compiled with --enable-valias.  Development of this feature was
	  sponsored by ACIS Pty Ltd <http://www.acis.com.au/>. [774819]
	* If you are upgrading from a previous version of QmailAdmin and
	  used the --enable-valias option when building vpopmail, be sure
	  to download vpopmail 5.4.1 or later and use the dotqmail2valias
	  program to convert .qmail-alias files to valias table entries.

1.2.0 - released 28-Jan-04

	Tom Collins
	- Changes to Makefile to avoid copying CVS directories on install.
	  Fixes previous attempt that wasn't compatible with all platforms.
	- Update all mentions of --with-htmllibdir to --enable-htmllibdir.
	- undef some macros defined in vpopmail_config.h and later defined
	  in QmailAdmin's config.h (PACKAGE_*). [883245]
	- Fix bad quota when adding user with default (blank) quota field.

1.2.0-rc3 - released 19-Jan-04

	Tom Collins
	- Minor formatting changes to configure.in for consistency.
	- Make function buffers static when returning pointers to them.
	- More updates to build files.
	- Properly format autoresponder names with '.' in them. [872721]
	- Remove old code and redundant quota setting code from user.c.
	- Switch to using new pw_flags field in auth.c.
	- Set permissions on html templates and lang files to 644 (not 755).

	Jussi Siponen
	- Add Finnish translation. [879032]

1.2.0-rc2 - released 1-Jan-04

	Tom Collins
	- Added comment and added minor fix to ##z macro in template.c.
	- Fix Makefile.am to use correct owner/group/perm for 
	  install-strip. [864310]
	- Change html/header to use ##D instead of ##H in title.

	Rick Widmer
	- Remove fclose(NULL) call in alias.c.
	- Add html/README with information on template macros. [867305]

1.2.0-rc1 - released 21-Dec-03

	Michael Bowe
	- Update to configure.in for libtools 2.57 compatability. [858201]

	Tom Collins
	- Use AM_INSTALL_PROGRAM_FLAGS instead of chown/chgrp/chmod in
	  Makefile. [827196]

	Andrea Riela (reviewed by Davide Guinchi)
	- Updates to Italian translation.

1.2.0-pre1 - released 9-Dec-03

	Tom Collins
	- More updates related to build process. [826594]
	- Fix HTML formatting on display subscribers page.
	- Convert HTTP_ACCEPT_LANGUAGE entries to lowercase, rename pt-BR
	  to pt-br. [852963]
	- Major cleanup of hooks code (fix call to execl, allow blank
	  lines, don't require entries for all hooks, pass proper 
	  parameters to hook program, update documentation). [855707]
	- Use VPOPMAIL_UMASK instead of hardcoded 0077.
	- Better lstat() error messages in template.c.

1.0.29 - released 20-Nov-03

	Tom Collins
	- Totally fix enable-htmllibdir option in configure.in. [812535]
	- Change from seteuid()/setegid() to setuid()/setgid(). [834008,
	  830695].  This code only gets executed if the cgi is suid root.
	  Note that we can use these calls since we don't need to get
	  root privileges back at any point.

	Nikolai V. Ivanyushin
	- Fix codepage name from koi-8r to koi8-r. [827792]
	- Updated Russian translation. [831398]

	Nicolas Croiset		- Updated French translation.

1.0.28 - released 17-Oct-03

	Oden Eriksson		- Updated Swedish translation.
	Tohru Kobayashi		- Updated Japanese translation.
	Martin Dalum		- Updated Danish translation.
	Nicolas Croiset		- Updated French translation.
	Martin Mosny		- Updated Slovak translation.
	Maurice Snellen		- Updated Dutch translation.
	Jeremiah Tieman (JT)	- Updated Spanish translation.
	Vyacheslav Shishakov	- Updated Russian translation.
	Fabio Erri		- Updated Italian translation.
	Julian Neytchev		- New Bulgarian translation.
	Dorneles Tremea		- Updated Brazilian Portuguese translation.
	Franz Stancl		- Updated German translation.
	Maciej Budzynski	- Updated Polish translation.
	Omer Faruk Sen		- Updated Turkish translation.
	HONTI Miklos		- Updated Hungarian translation.
	Andreas Tolfsen		- Updated Norwegian translation.

	Tom Collins
	- Properly handle mailing list prefixes (strip square brackets).
	- Update configure.in to try htmldir as starting point for 
	  cgibindir and imagedir.
	- Update configure.in to use imageurl when building imagedir.
	- Update configure.in to work without acconfig.h; rm acconfig.h.
	- Update missing script with version from automake 1.6.
	- Fix enable-htmllibdir option in configure.in. [812535]
	- Check result of getenv() so we don't strdup(NULL). [800326]
	- Add 'Reply-To' to headerremove when setting Reply-To. [810670]
	- Add depcomp script, now required by automake/autoconf?

1.0.27 - released 9-Sep-03

	Oden Eriksson		- Updated Swedish translation.

	Tom Collins
	- Added tracker item numbers to CHANGELOG for all closed trackers.
	- Fixed Makefile.am to include README.hooks in distribution.
        - Updated code that parses HTTP_ACCEPT_LANGUAGE to compare
	  q values when deciding on best available language. [787601]
	- Fixed bug in modify quota code. [791587]
	- Allow user to set Reply-To header on lists. [771723]
	- Moved error strings for modify quota to language files.
	- Updated some hardcoded strings to pull from langauge files.
	- Reorganized modify quota code for readability.
	- Updated configure.in for readability.
	- Moved language files from 'html' to 'lang' directory.  Note
	  that QMAILADMIN_TEMPLATEDIR and HTMLLIBDIR should point to
	  /usr/local/share/qmailadmin (or equivalent) instead of the
	  html directory in that directory.  Updated configure.in and
	  Makefile.am to reflect that change.
	- When creating user, blank quota will now use default quota.
	  Use "NOQUOTA" for unlimited. [787716] [802662]
	- Update add_user.html to say "NOQUOTA for unlimited". [802662]
	- Allow blank password on user creation if vpopmail was compiled
	  with learn-passwords enabled. [801677]
	- Make add_mailinglist templates as similar as possible, in
	  anticipation of eventually replacing them with a single file.
	- Various formatting cleanup in add_user template.
	- Update show_counts() in util.c to pull from language files.
	- In user.c, use strcmp instead of strncmp when making sure both 
	  passwords entered are identical.

1.0.26 - released 6-Aug-03

	Omer Faruk Sen
	- Change formatting of Turkish language file to iso-8859-9.

	Martin Mosny
	- Updated Slovak translation.

	Oden Eriksson
	- Added README.hooks. [784037] [778499]

	Tom Collins
	- Attempt to properly handle IMAP mailboxes included in Maildir
	  delivery lines in .qmail-alias files. [779697]
	- Change configure option mysql to ezmlm-mysql. [781041]
	- Fix default for --enable-ezmlm-mysql to be ON. [781041]
	- Updated README.hooks and hook-related code in user.c.
	- Changed configure options --enable-autorespond-bin and 
	  --enable-true-bin to --enable-autorespond-path and 
	  --enable-true-path to avoid confusion.  They both specify
	  the directory containing the binary, not the binary itself.

1.0.25 - released 24-Jul-03

	******* Corrected security flaw introduced in 1.0.13 ********
	In QmailAdmin 1.0.13 through 1.0.24, it was possible for any
	user to configure their account (on the "Modify User" screen)
	to forward their email to any program on the server.

	Since the program would run as the vpopmail user, this was a
	very bad thing.  The 1.0.25 release corrects this problem,
	but will remove existing program delivery lines (other than
	autoresponder and spam command) from a user's .qmail file if
	they click the "Modify User" button on the "Modify User" screen.

	This is a temporary fix; we plan to improve the code that
	alters a user's .qmail file to allow existing program delivery
	lines to remain unchanged.
	*************************************************************

	Tom Collins
	+ Security fix described above.
	+ Remove count_aliases() and CurAliases (now that aliases are
	  considered forwards).
	+ Fix count_forwards() to correctly ignore ezmlm-idx list files
	  and count all forwards (both email address and Maildir format).
	  [772529]
	+ When reading email address of list owner, strip leading & if 
	  present.
	+ Added alias2forward.pl in contrib directory to convert from
	  Maildir-style aliases to address-based forwards.  See contrib/
	  README.alias2forward for more details. [753683]
	+ Added --disable-mysql option to configure to disable MySQL 
	  options on add/modify mailing list (defaults to enabled).
	  Added ##tm tag (currently unused) to show block when MySQL
	  is enabled. [771726]
	+ Increase size of alias_user in alias.c to MAX_BUFF to avoid
	  buffer overflow on long lines in .qmail-user files. [776976]

	Nicolas Croiset
	+ Corrections to French translation.

	Andreas Tolfsen
	+ Updated Norwegian translation.

	Omer Faruk Sen
	+ Updated Turkish translation.

	Franz Stancl
	+ Added 6 missing German translations.

1.0.24 - released 16-Jul-03

	Michael Bowe
	+ Remove reference to "Aliases" in del_forward.html. [770008]

	Yuu Andou
	+ updated Japanese translation

	Tom Collins
	+ Converted tabs to spaces in .html files to make editing easier.
	+ Fixed mailinglist.c to work with non-idx build of ezmlm again.
	  [765760]
	+ Cleaned up ezmlm_make in mailinglist.c.  Limit SQL options to 64
	  characters each to prevent buffer overflow in args to ezmlm-make.
	+ Changes to mod_user.html to make it more readable and easier to
	  follow, especially with non-English translations. [762368]
	+ Configure process searches for 'true' binary in /bin, /usr/bin
	  and /usr/local/bin.  Added -enable-true-bin option to override.
	  (QmailAdmin uses 'true' for blackhole accounts until vpopmail
	  supports "#" in .qmail accounts.) [766572]
	+ Various changes to configure.in (displays more information in
	  "current settings" at end of configure process).
	+ Removed add_alias.html, del_alias_confirm.html, & show_alias.html
        + Change display of forwards for program delivery ("|") and
	  blackhole/delete ("#").  Now displays in italics, shows "deleted"
	  for blackhole and program name (without path) including command-
	  line parameters for program delivery. [771390]
	+ Cleanup Makefile.am to fix make dist and make distcheck.

1.0.23 - released 25-Jun-03

	Tom Collins
	+ Fixed get_html_text (broken during fgets cleanup).
	+ Fixed minor bug in get_catchall (user.c).

1.0.22 - released 25-Jun-03

	Tom Collins
	+ Fixed permissions on language files. [754268]
	+ Cleaned up code that determines whether user can delete an account.
	+ Converted unnecessary use of strncmp to strcmp in command.c.
	+ snprintf, GetValue, memset, fgets, strncmp, strncpy cleanup
	  [755708]
	+ Removed unneccessary memset calls before GetValue calls. [755708]
	+ Modify Makefile.am to chmod -R 755 /usr/local/share/qmailadmin
	  on install.  Also, don't create en-us language file.
	+ Cleanup in show_login_help.html.
	+ Changed qmailadmin URL in template.c to SourceForge.
	+ Cleaned up login .png files to fix bottom edge.
	+ Merged show_login_help.html and show_login.html with ##th tag.
	+ Modified install procedure in configure.in (set permissions,
	  remove login_help code, other fixes).
	+ Now uses /bin/true instead of ~vpopmail/bin/vdelivermail on
	  "blackhole" accounts (modifies James Raftery's patch in 1.0.19).

	  WARNING: Blackhole accounts set up in previous versions of
	  qmailadmin can cause infinite mail loops.  Please fix any
	  accounts using the blackhole feature by replacing the call to
	  vdelivermail with "|/bin/true delete".  To find such accounts,
	  run:
		grep vdelivermail `find ~vpopmail/domains -name .qmail`

	  Manually edit the .qmail files, or run qmailadmin 1.0.22,
	  modify the account, and click the "Modify User" button to
	  switch it to /bin/true from vdelivermail.

	Michael Bowe
	+ Cleaned up status response when adding a new forward. [753694]
	+ Minor changes to column headers on add/mod/show forward. [753694]

1.0.21 - released 06-Jun-03   

	Michael Bowe
	+ Show number used and maxmimums for autoresponders, forwards
	  and mailing lists.

	Ryan O'Neill
	+ Don't show mailing list options if configured with
	  --disable-ezmlmdir or --enable-ezmlmdir=no.

	Tom Collins
	+ Don't show mailing lists on "Add Email account" page
          if ezmlm is disabled (see above) or there aren't any lists.
	+ Change header formatting to be consistent, including Michael
          Bowe's changes.
	+ Established SourceForge project 'qmailadmin' to coordinate
	  development efforts.

	Oden Eriksson
	+ updated Swedish translation

	Jeremiah (JT) Tieman
	+ updated Spanish translation

	Maurice Snellen
	+ updated Dutch translation

	Martin Mosny and Peter Hudec
	+ new Slovak translation

	HONTI Miklos
	+ updated Hungarian translation

	Jeff Hedlund
	Forwards/Aliases:
	+ Was not actually checking for existing users when adding a new
	  forward
	+ Removed alias count checks, just checking for forward counts
	+ Remove AliasType code
	+ adddotqmailnow and moddotqmailnow have some shared code in a new 
	  function, adddotqmail_shared() 
	+ Changed the modify forward page to have a better table layout
	+ Changed modify forward page to show same interface as add forward;
	  less confusion
	+ Adding a forward to a local user now checks for an actual account
	  or an existing forward
	+ Listing forwards removes local domain if a local user.  If the
	  local user does not exist (actual or forward), it lists the
	  user@domain in red to jump out as an invalid forward 
	  (compares domain part of alias (after @) for an exact match
	  with the Domain logged into)
	+ Some changes to html/en to better reflect when dealing with forwards
	+ Delete user now uses the shared code of adddotqmail_shared()

	Quota:
	+ For administrator, Quota field populates on mod user.
	+ For user, Quota is displayed on mod user.

	Session values:
	+ Modified the .qw file to hold a string of variable value
	  combinations that GetValue() can read.  (eg,
	  "ip_addr=[ip]&another_var=[whatever]", etc)
	+ Added two session values that may be a little specific to me: 
	  returntext and returnhttp.  These values, when both are given,
	  will replace the Logout text with the text in returntext, and
	  the link for logout to the link in returnhttp.  I am going to
	  be working on a very simple SquirrelMail plugin that will send
	  the user to qmailadmin, and I wanted a easy way to  let them
	  return to squirrelmail instead of logging out.

	Catchall:
	+ Catchall continues to use the Maildir for local users, but if
	  the file contains a local email address, the qmailadmin UI will 
	  show that account as the catchall.
     
	Login page changes:
	+ Added configuration option --enable-domain-autofill (default no)
	  to fill the domain based on the accessed domain (compares the 
	  HTTP_HOST to the virtualdomains to get the correct domain)
	+ If username and dom are set in the var line (as they are when
	  logging out), those fill the username and domain
	+ If username is not filled, defaults to postmaster (as it used to)
	+ If a failed login attempt is made, the username and domain are
	  filled the same as they were when the login attempt was made

	Spam Detection:
	+ Added --enable-modify-spam configuration directive to turn on
	  this option (default OFF)
	+ Added --enable-spam-command to define the command to be used
	  in the users .qmail file (default: "|preline /usr/local/bin/maildrop 
	  /etc/mailfilter")
	+ Updated INSTALL (for spam instruction and domain autofill
	  instruction)
	+ Add user page now has the option to set spam detection
	+ When Save Copy AND detect spam are checked, spam is detected
	  on local (only)

	General cleanup:
	+ Added a new, special tag in the templates: ##tX where X is a letter.  
	  This tag could help clean some of the templates up:  It allows 
	  optional output in the templates.
	+ Added 3 tags for ##t:  ##tq is for MODIFY_QUOTA, ##ta for
	   DOMAIN_ADMIN, ##tu for USER_ADMIN
	+ Merged mod_user_*.html to mod_user.html using ##tq tag
	+ Merged add_user_*.html to add_user.html using ##tq tag
	+ Removed "Email Accounts" and "Main Menu" links from USER_ADMIN
	  mod_user page. (because they are not necessary, there's no more
	  functionality for a user on the main menu or the "Email Accounts"
	  listing)
	+ Fixed configuration files so autoconf, automake, etc. work.
	+ Updated TODO (removed the stuff this fixes or is outdated)

1.0.20
05/26/03    Tom Collins
          - Fix bug when enabling MySQL support on mailing list.  
            Previously fixed, but patch never made it into distribution.

1.0.19
05/26/03    Tom Collins
          - Removed references to --enable-maxpopusers, etc. from
            build scripts and INSTALL document.  Maximums are now
            handled through vlimits code in vpopmail.

            James Raftery
          - Better matching of 'delete' and 'bounce' settings for catchall
          - Addition of deleted/blackholed mail handling option (with 
            Vacation and Forward) on POP/IMAP accounts.

            Gerco Dries
          - lstat error message in send_template_now of template.c

            Maciej Budzynski
          - Updated Polish translation.

            Davide Giunchi
          - Updated Italian translation (just a few lines).

            Dorneles Tremea
          - Updated Brazilian Portuguese translation (just a few lines).

            Jesper Have
          - Complete Danish translation and a message to Danish users,
            "Hvis du finder nogen fejl i den send dem til webmaster@jdh.dk"

            Jeroen van Raalte
          - New (incomplete) Dutch translation.

            Jerome Mollier-Pierret
          - Updated French translation.

            Vyacheslav Shishakov
          - Updated Russian translation.

1.0.18
05/21/03    James Raftery
          - User Flags on Modify User pages are now commented out in the
            HTML by default.

            Tom Collins
          - Added common sorting code for Mail Robots, Mailing Lists,
            and List Subscribers.
          - Clean up interface & sort mailing lists on "Add User" page.
          - Returns to Mail Robot List after deleting a Mail Robot.
          - Correctly shows mailing list containing '.' on add user page.
          - Removed language strings related to old mailing list
            interface (019-038, 040-052).
          - Removed dupe 012/013 and fixed wrapped lines in html/it.
          - Added missing strings to da language file.
          - Fixed digest list checking error from 1.0.16.
          - Changed mailing list layout to better match other pages.
          - Properly displays list owner when modifying ML containing '.'.

1.0.17
04/28/03    Tom Collins
          - updated menus in page footers to include Logout link, and a link
            up to the previous level of the menu tree.

          - qmailadmin now tries the primary language (e.g., de) if there
            isn't a file for the sublanguage (e.g., de-at). [787707]

1.0.16
04/18/03    Tom Collins
          - when regular users log in they go directly to the modify user page
        
          - Clean up error reporting when changing a user's password.  
            The verror should only be reported if the change failed,
            instead of when it works, and there's no need to display the 
            email address -- it's right there on the modify user page.

          - On the list of mailing lists page, it will only show 
            digest-related buttons for a list if it has a digest.

            Franz Stancl
          - New German translation 

            James Raftery
          - enables qmailadmin view and modify the user mailbox 
            flags: V_USER0, V_USER1

1.0.15
03/26/03    Dorneles Trem
          - Updated Brazilian translation

03/24/04    Brian Kolaci
          - updates to to quota code

1.0.14
03/22/03    Ken Jones
          - Lost the ability to GET for logins, it's back in

	Switch to using vlimits code from vpopmail.  This release
	requires vpopmail 5.3.19 or later.

1.0.13
03/18/03    Tom Collins
          - Makefile.am fixes (@imagedir@ and @QA_PACKAGE@)
          - Added ##Z to represent imagedir in .html files
          - Updated main_menu and show_login[_help] to use ##Z
          - Remove check for valid address format when deleting from lists
          - Fix check for "Archive" (option i) on non-moderated lists
          - Updates to util.c regarding quotas

            Matt Simerson
          - Fix javascript typo in mod_user_[no]quota

03/03/03    Fabio Erri
          - New Italian translation

02/27/03    Joshua Megerman
          - more robust handling of .qmail file syntax. Lines like:
            |/usr/local/bin/maildrop ./.mailfilter would be mangled into 
            two lines:
            &|/usr/local/bin/maildrop
            &./.mailfilter
            This patch correctly handles the above case and is backwardly 
            compatible

1.0.12
02/27/03    Tom Collins
          - *Major* overhaul of add/modify mailing list code and interface.
          - New entries to en lanuage file for mailing list interface

            Ken Jones
          - update all language files to include English text for the
            new mailing list entries.

1.0.11
02/26/03    Dorneles Tremea
          - Removed files 'Makefile.in.orig' and 'Makefile.in.rej'
          - Added item 260 to enable 'Routing' translation, on both
            'mod user noquota.html' and 'mod user quota.html' files.
          - Updated Brazilian Portuguese translation file.

            Ken Jones
          - new --enable-imagedir=[dir] configure option to control what
            directory to place the image files in. 
            Default is DocumentRoot/images/qmailadmin

          - new --enable-imageurl=url configure option to control the URL
            to access the images.
            Default is http://machine/images/qmailadmin

1.0.10
02/25/03    Tom Collins
          - Replace the generic red buttons used everywhere with special
            buttons for "delete" and "modify".
   
          - Replace the buttons in the user list for setting the CatchAll
            account with a set of radio buttons.  This is a slight improvement,
            but could use more work, specifically in how remote and bounce 
            CatchAll are displayed.
   
          - Move delete and modify buttons to the left side of row information,
            especially in rows of varying length that are left align.
            Note that trash.png is a modified version of an icon from KDE and
            modify.png is an icon from GAIM (both GPL products, so re-use of
            icons should be OK). 

          - Honti Miki 
            Updated Hungarian translation

          - Nicolas Croiset
            Updated French translation

          - Michael Bowe
            configure.in syntax error for maxaliasesperpage fixed
 
02/10/03    Matt Simerson
          - changes to login page for admin guide links
          - changes to mod user page for cleaner configuration

          - missing trailing / on Users Guide and Admin Guide help links
          - add Users Guide, Admin Guide and Help Index into language files

1.0.9
01/25.03    Oden Eriksson
          - Remove un-used code, show_locals(), show_rcpthosts(), 
            show_virtualdomains(), show_assign(). These functions
            were a start at making qmailadmin be able to administer
            qmail control files. Instead of putting them in qmailadmin,
            they were moved to the vqadmin package.

            Ken Jones
          - add dependencies in Makefile.am so all files get recompiled
            if configure is run again. All .o files depend on config.h
          - --enable-help wasn't correctly setting up the show_login.html page


01/24/03  - Makefile.am had 8 spaces instead of a tab 

            Brian Kolaci
          - make qmailadmin posix complient
            remove the fts_ functions 
            implemented scandir() and bkscandir() functions
            fixed memory leak in alias functions that used scandir

            Oden Eriksson
          - Makefile.am: use DESTDIR variable 

1.0.8
01/21/03  - on Modify Mailing list page with idx installed, Should display
            prefix and not Quota.

            Oden Eriksson, Deserve IT com
          - 1.0.8 Swedish Translation.
            Svenskar bör korrekturläsa denna och/eller ge synpunkter.

          - solved the alphasort problem by just using strcasecmp
            function for two dir entry parameters. Wonder if scandir
            is supported on HP/Solaris/BSD?

01/19/03  - New help links from www dot cfes dot ca

1.0.7
01/18/03  - moduser  logic syntax for displaying quota modification or
            not was not using the correct file names. There are only two
            file names, mod_user_quota.html and mod_user_noquota.html.
            Everyone gets noquota except when configured with 
            --enable-modify-quota=y AND the user (usually postmaster)
            who has
             if(AdminType==DOMAIN_ADMIN)send_template("mod_user_quota.html");

1.0.7
01/14/03    Michael Bowe mbowe at pipeline dot com dot au
          - Make the menu layout more consistent with other parts
            of qmailadmin. ie put the email address on the left and
            the action button on the right 
          - In the show subscribers menu, the column headings were
            the reverse of the data. ie the action button was in the
            address column, and the address was in the action column.
          - Add mailing list screen showed Quota instead of Prefix, 
            added line to language files
          - Show accounts screen displayed Total/Moderators instead 
            of Total Accounts. Added line to language files

          - Clean up of MODIFY_QUOTA #ifdefs
 
01/13/03    Nicolas Croiset nicolas dot croiset at radio dash campus dot org
          - New French translation

            unknown contributor
          - New Dutch translation

            Alex Krohn alex at gossamer dash threads dot com
          - new secure session code
          - alternate quota patch
          - help file templates

          - add new configure option --enable-user-modify-quota default is No
            setting --enable-user-modify-quota=y will allow the postmaster
            or other site admin account to set a users quota.
          - new dependency rules in Makefile.am to remake all the files
            if config.h changes (re-run config and make will recompile all
            the code, otherwise, it might not pick up the changes)


01/10/03    Michael Bowe mbowe at pipeline dot com dot au
          - configure script contains a typo for the maxaliasesperpage
            configure command
          - page navigation hyperlinks at the bottom of show forwards/aliases
            screen always display "next page" even when there are no more
            users to display
          - sort the userlist using the "sort_it=1" option in vpopmail api
            call to vauth_getall()

            Tim Janes t dot janes at 2020speech dot com
          - Adjustment so that entries on the forward/aliases screen are
            show in alphabetical order

            Unknown contributor
          - changes to the login page

            Unknown contributor
          - changes to the mailing list prefix setup

          - remove "nothing" printf on modify user if nothing is changed

          - add support for "disable_forward" and "disable_vacation" in
            .qmailadmin-limits file. Those will disable a user from
            being able to "save and forward" or to set a vacation,
            respectively.

01/09/03
          - update login code to use a GET to login like:
            https://host/cgi-bin/qmailadmin?username=postmaster&
            domain=yourdomain&password=your_postmaster_password

1.0.6

1.0.5
09/01/02    John Doshier john at inter7 dot com
          - Increased buffer size so that vacation messages would work
            for users with long usernames.  
          - Added polish translation.

06/11/02    Bill Shupp hostmaster at shupp dot org
          - Added add/delete/show digest user support for idx lists
          - removed extraneous patch file

05/31/02    Loren Jordan ljordan at nw3c dot org
          - user.c aesthetic fix. next_pages ran into select_catchall_deleted

1.0.4
03/25/02    Darren Fraser darrenf at jammicron dot com
          - fixes soft links when a mailing list is changed. Now the
            softlinks can get unlinked successfully.

          - cleaned up indentation

03/22/02  - add --enable-ipauth=yes (default). If you have clients behind
            a NAT/proxie/whatever who's IP address changes during the 
            session then you need to use --enable-ipauth=no

03/18/02  - cleaned up ChangeLog 

            Antoni Nata tonix at interazioni dot it
          - updated Italian translation

03/15/02  - typo in en file "New Mailing Lists" should be "New Mailing List"

03/14/02    Tom Collins tom at tomlogic dot com
          - use malloc to allocate 64k blocks of memory for reading in address
            list instead of a large block of fixed length strings
          - more efficient use of memory -- doesn't allocate 96 bytes for 
            each address
          - will read addresses up to 255 characters long
          - will handle up to 100,000 addresses as long as they fit in
            200*65536 bytes of memory (note that a web browser would probably
            crash before displaying that many addresses)
          - case-insensitive sort of email addresses
          - uses qsort instead of the bubble(?) sort in v1.0.2 (should be
            significantly faster for large mailing lists)
          - removed spacestring and checked2.
          - combined a lot of the functions that worked with
            subscribers/moderators with a single function that could handle
            either.  It eliminates a lot of duplicate code, and ensures that a
            change to one happens to the other.
          - As a result,  fixed a bug or two in how qmailadmin behaved.  
            For example, it looks like entering a bad address on the 
            "add moderator" form would display the error on the 
            "add subscriber" form.
          - also made some changes per a post by Chris Hardie 
            (11:13 AM -0500 2/28/02):

            Yeak Nai Siew yeak at md dot com dot my
          - clean up of mod_user to support forwarding AND vacation.
          - clean up form field returned by template.c so the \n character
            is not part of the field data. 

03/12/02  - use malloc instead of stack space for large character arrays
          - mailinglist.c: make sublist and pa arrays globals instead of
            allocating space on the stack
          - change sublist and pa arrays to char not unsigned char
          - util.c: check_local_user stating for username not needed.
          - new --enable-user-index option. Disable display of user indexes
            in case using Internet Explorer 5.50 which is brain dead. 
          - mailinglist.c: remove non-portable C++ style comments //

1.0.3   
02/28/02    Rich Roth rich at on-the-net dot com
          - fixed annoying typo in FAQ file on QMAILADMIN_TEMPLATEDIR
02/23/02    Albert Weichselbraun albert at atnet dot at
          - typo in German translation fixed

02/22/02    Tom Collins tom at tomlogic dot com
          - Change ReQuest to Request in en, hu, tr

02/18/02    "Lars Lerager Hansen" lars at interachtung dot com
          - new danish translation

02/14/02  - spelling error of Vacation in qmailadmin en file fixed

            Mike Horwath drechsau at Geeks dot ORG
          - if using mysql replication, after adding a user sleep for
            2 seconds before calling vauth_getpw, so the replication 
            has a chance to propogate

02/11/02  - Do not allow the language variable to be used to look at
            other files on the machine.
          - Do not allow the PATH_INFO varilable be used to look at
            other files on the machine.

            "Nicolas CROISET" ncroiset at vdldiffusion dot com
          - addition to french translation

            "Oden Eriksson" oden dot eriksson at kvikkjokk dot net
          - Updated swedish translation

1.0.2
02/05/02    "Davide Giunchi" davidegiunchi at libero dot it
          - after adding a new user, display the user page
          - after adding a new alias/forward display the alias/forward page
          - after adding a new autoresponder display the autoresponder page

          - after adding a new mailing list subscriber go back to the
            add mailing list subscriber page.
          - display qmailadmin and vpopmail versions at bottom of
            main page with links back to home pages
          - Change behavior of displaying forwards/aliases so that 
            people can display programs
          - do not display progams that contain the names ezmlm or
            autorespond
          - add link to mod alias/forward to go back to displaying 
            alias/forwards page

02/03/02    "Nicolas Croiset (VDL)" ncroiset at vdldiffusion dot com>
          - updated French translation
  
            "Pablo Murillo" info at killer dot com dot ar
          - updated Espanol translation

02/02/02  - add support for deleting catchall email
          - remove "Refresh Page" from Email Accounts menu for space

            "Jens Georg" jens dot georg at instant-networks dot de
          - update german translation

01/23/02  "Davide Giunchi" davidegiunchi at libero dot it
          - Display error message if no "object" is created and
            click on display "object" on main menu.
 
01/10/02  - copy en file to en-us on make install

01/08/02  "Czako Krisztian" slapic at linux dot co dot hu
          - Hungarian translation

01/05/02  "Davide Giunchi" davidegiunchi at libero dot it
          - email accounts can be added without a password, fixed

1.0.1
01/03/02  - configure checks for html dir in /var/www/html (redhat default)

12/24/01  - updated limits/quota parsing code

12/20/01  - added new step to INSTALL file to tell people to
            add a new domain before trying to log into qmailadmin. 

12/12/01  - added ";" character to token characters for parsing
            HTTP_ACCEPT_LANGUAGE string.

1.0
11/19/01  "Einar Bordewich" einar at bordewich dot net
          - Updated norwegian dictionary

          Oden Eriksson oden.eriksson at kvikkjokk dot net
          - Updated swedish dictionary

11/18/01  "Tim Hunter" tim dot hunter at cimx dot com
          - Added align=left to many table data tags to compensate for IE 6
            not adhering to HTML specifications.

          "Bill Shupp" hostmaster at shupp dot org
          - More html enhancments.
          - Changed "Master Account" to "User Account" in english dictionary
          - Changed POP to Email in english dictionary
          - Verified that HTTP_X_FORWARDED_FOR fix works for users behing 
            proxies with this environment set.


11/16/01  "Ken Jones" kbo at inter7 dot com
          - Makefile.am install-exec-local section didn't need the
            test for /etc not being writable (What the hell was that??)
            Now it just sets the ownership/permissions
          - updated the INSTALL file: configure as root

11/15/01  "Bill Shupp" hostmaster at shupp dot org
          - Fixed .qmail naming problems in mailinglist.c for lists 
            containing '.' - Steve Wilson pointed this out.

11/14/01  Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Converted the gif:s to png:s to truly become GPL,
            also make the proper changes in the source and html.
            unisys holds the patent on the gif stuff, check:
            http://www.unisys.com/unisys/lzw/, so just let them
            have it, and say up yours unisys!

11/12/01  Brent B dot Powers powers at b2pi dot com
          - Fixed login bug: Swapped sprintf and show_login() lines in 
            qmailadmin.c

0.91
11/08/01  Bill Shupp hostmaster at shupp dot org
          - Added default_quota option to .qmailadmin-limits, updated INSTALL

0.90
11/01/01  Richard A. Secor rsecor at fcc dot net
          - Change PACKAGE and VERSION variables to
            QA_PACKAGE and QA_VERSION

11/02/01  "Nicolas Croiset" ncroiset at vdldiffusion dot com
          - Update to 062 of fr translation

11/01/01  Bill Shupp hostmaster at shupp dot org
          - If passwords didn't match when adding a POP user, nothing was 
            returned to the browser - fixed.  Thanks to Gerald Villemure for 
            pointing this out.
          - Removed the last of the "heading 2" html tags from StatusMessage 
            sprintf statements.

10/29/01  "Christof Egner" cegner at wireworm dot de
          - Updated German translation


0.89
10/25/01  Bill Shupp hostmaster at shupp dot org
          - Cleanup in mailinglist.c and show_subscribers.html: modified 
            Steve's changes to resemble Einar's cosmetic patches, fix for
            display of odd numbered totals.  Applied Steve's new sorting 
            code to show_moderators_now().
          - Updated FAQ #5

10/24/01  Steve Fulton steve at esoteric dot ca
          - Fixed sort mailing lists code to work with lists > 500 subscribers.
            Should now work with up to 65,000. HTML enhancements too.

10/19/01  Bill Shupp hostmaster at shupp dot org
          - Fix for count_aliases() and count_forwards() counting IDX list
            owners

0.88
10/17/01  Gabriel Ambuehl gabriel_ambuehl at buz dot ch
          - Fix corrupted german translation

10/15/01  "Einar Bordewich" einar at bordewich dot net
          - Update to no translation

10/11/01  "Remo Mattei" remo at italy1 dot com
          - Update to Italian translation

          "Oden Eriksson" oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

0.87
10/10/01  "Bill Shupp" hostmaster at shupp dot org
          - Updated version to 0.87
10/08/01  - Added addclearfield-cdb.sh script to contrib directory.  This
            is for people who have recompiled vpopmail-5.0 to use clear 
            passwords and also use the cdb module.
          - Added FAQ #5 do describe the above issue.
          - Removed the last of the SYSTEM_ADMIN code

10/06/01  Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

          "Einar Bordewich" einar at bordewich dot net
           - Cosmetic updates to show_autorespond.html, show_forwards.html,
             show_mailinglist.html, and mailinglist.c

          "Nicolas Croiset" ncroiset at vdldiffusion dot com
          - Update to fr translation

0.86
10/01/01  "Bill Shupp" hostmaster at shupp dot org
          - One more fix for non-postmaster administrators
09/28/01  - Removed #define QA_ADMIN statement and included vpopmail.h (auth.c)
          - Re-added case 'i7' which got removed around 0.78
          - Fixed case 'B' which got messed up around 0.77
          - Fixed typos in add_mailinglist-idx.html.  
            Ross Davis pointed these out.

0.85
09/26/01  "Bill Shupp" hostmaster at shupp dot org
          - Fix for non-postmaster administrators so they can't delete 
            themselves
          - Put a note in INSTALL about non-postmaster administrators
          - Finished updating mailinglist.c and mod_mailinglist-idx.html to 
            mirror add_mailinglist-idx.html

0.84
09/24/01  "Bill Shupp" hostmaster at shupp dot org
          - Added another missing  vclose() and exit(0) to modusergo().
          - Added newlines to select list in case 'O' of template.c
          - Added experimental support for non-postmaster administrators
            (requires my patch to vpopmail-5.0 at http://shupp.org)
          - Removed old sysadmin.c file that somehow got re-added to the source

          Gabriel Ambuehl gabriel_ambuehl at buz dot ch
          - Updated German translation

          "Einar Bordewich" einar at bordewich dot net
          - Updated Norwegian translation


0.83
09/22/01  "Bill Shupp" hostmaster at shupp dot org
          - Missing "#" in del_users.html
          - Minor html changes to show_user_lines()
          - Fixed errors in mod_autoresponder.html
          - Re-added "no-cache" meta tags to mod_mailinglist-idx.html
          - Fixed html error in mailinglist.c that prevented you from 
            modifying idx lists
          - Stripped some newlines off of elements in case 'F' of 
            template.c, cleaned up html
          - Check for mailing lists,  robots, aliases/forwards before 
            showing them
          - Added dictionary entries 231, 232 and 233
          - Added BUGS and TODO files

          Nicolas Croiset ncroiset at vdldiffusion dot com
          - Typo in fr translation #230

          Gabriel Ambuehl gabriel_ambuehl at buz dot ch
          - Updated German translation

0.82
09/18/01  Antonio Dias accdias at sst dot com dot br
          - added --enable-vpopgroup configure option
          - "make install" now installs with proper uid/gid
            when not using defaults
          - fixed html/*_users.html files and improved HTML generated
            by user.c:show_user_lines()
          - Brazilian Portuguese update
          - HTML updates for main menu and template.c

0.81
09/17/01  Steve Fulton steve at esoteric dot ca
          - Sort mailing list subscribers

          Antonio Dias accdias at sst dot com dot br
          - Lots of HTML updates in templates and .c files
          - Code style modifications
          - New "disabled" gif

          Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

          "Nicolas Croiset" ncroiset at vdldiffusion dot com
          - Update to fr translation
          - Fixed en dictionary #118

0.80
09/14/01  - delete alias/forward was still checking for AliasType. Fixed

0.79
09/10/01
          Steve Fulton steve at esoteric dot ca
          - Add the name of the domain being edited to the main menu screen
          - add total subscriber count to the mailing list subscribers page

          Antonio Dias accdias at sst dot com dot br
          - sync language files with lang/en and to update Brazilian
            Portuguese translation.
          - HTML changes

0.78
09/07/01  - remove unused main_table code/html

          - change main menu to have one "Aliases/Forwards" line
            instead of two lines "Aliases" and "Forwards"

0.77
09/04/01  Christof Egner cegner at wireworm dot de
          - new german translation

08/30/01  - change password result message no longer shows password 

0.76ac3
8/23/01   Bill Shupp" hostmaster at shupp dot org
          - Added missing  vclose() and exit(0) to modusergo().

8/21/01   Bill Shupp hostmaster at shupp dot org
          - Added check_local_user() to addusernow() to prevent duplicates.
          - Updated check_local_user() to use vauth when checking for user.
          - Removed old sysadmin code and sysadmin.c (dead).

0.76ac2
8/20/01   Bill Shupp hostmaster at shupp dot org
          - Fixed "Go user" hang when searching for accounts starting with
            a number: separated Pagenumber from SearchUser (new variable).
            Alfredo Hassaf pointed this out.
          - Added "0-9" to POP Accounts index.
          - More "show users" html tweaks.  "Previous Page" and "Next Page"
            are no longer displayed if they don't go anywhere.

8/19/01   Bill Shupp hostmaster at shupp dot org
          - Updated mod_autorespond.html to mirror add_autorepond.html

8/19/01   Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

0.76ac1
8/18/01   Nicolas Croiset ncroiset at vdldiffusion dot com
          - Several html fixes.
          - Update to fr translation

8/17/01   Bill Shupp hostmaster at shupp dot org
          - Rewrote show_users.html (and user.c's html)
          - added html/colortable so you can change colors of hard-coded html.
            It's a copy of get_html_text(), but for bgcolor tags.
          - Created contrib directory, put my deletegif.psd in there
          - Updated "Total Users" to show the limit as well.

0.76
8/17/01   Bill Shupp hostmaster at shupp dot org
          - Added "Total Users: " to show_users.html Thanks to Steve Fulton
            for the idea.
          - HTML fixes

0.75
8/13/01   Bill Shupp hostmaster at shupp dot org
          - More fprintf->sprintf changes in alias.c
          - Fixes in adddotqmailnow maxalias/forward checks.

1/23/01   DJ (David J Radunz) dradunz at bigpond dot net dot au
          - (Found in archives, never applied) Fix for users behind a proxy by
            adding support for HTTP_X_FORWARDED_FOR environment variable.

0.74
8/13/01   Bill Shupp hostmaster at shupp dot org
          - Merged Ken's 0.73 and mine.  Updated version.
          - Minor changes to mod_user.html
          - modified what's in header.html by default, put back 
            "no-cache" meta tags in mod_mailinglist-idx.html

0.73
08/13/01  Ken Jones kbo at inter7 dot com
          - Integrated in aruba.it modify user page

8/4/01    Bill Shupp hostmaster at shupp dot org
          - Added check for maxusers in addusernow, not just adduser.
            Cached web pages allowed you to exceed your limit.  Discovered 
            by Evren Yurtesen.
          - Added similar checks in adddotqmailnow, addmailinglistnow and 
            addautorespondnow.

8/2/01    Oden Eriksson oden dot eriksson at kvikkjokk dot net
          Missing "border=0" in image tag in mailinglist.c

          Einar Bordewich einar at bordewich dot net
          - Updated Norwegian translation
          - Patches fixing hard-coded text in template.c, mailinglist.c, and
            del_forward.html.

0.72
8/1/01    Bill Shupp hostmaster at shupp dot org
          - added ##N<filename># include hook for use in templates.
            Thanks to Antonio Dias for the idea.
          - Removed trailing "/" from HTMLLIBDIR definition in configure.in
          - A few additions to the FAQ
          - One more change to en dictionary 010

          Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

0.71
8/1/01    Bill Shupp hostmaster at shupp dot org
          - added ##V hook to display PACKAGE, VERSION. This is NOT in 
            templates by default.
          - added html/header.html and html/footer.html.  send_template()
            now sends these with every html file, so you can have global
            headers and footers.  Other hooks (i.e. ##D) can be used in
            these files.  Moved appropriate header and footer html into these
            files


0.70      - increase the version number

0.60pre9
7/31/01   Bill Shupp hostmaster at shupp dot org
          - Added --enable-no-cache={N|y} as configure option
          - Changed references in en dictionary of "pop" to "POP"
          - Modified en dictionary 010 because ##D doesn't get parsed
            in dictionaries
          - Added "no-cache" meta tags to mod_mailinglist-idx.html
          - Added " at ##D" to several html templates to make local 
            names more obvious to users.

7/30/01   Antonio Dias accdias at sst dot com dot br
          - Added "no-cache" code for qmailadmin.c

0.60pre8
7/30/01   Oden Eriksson oden dot eriksson at kvikkjokk dot net
          - Update to Swedish translation

7/30/01   Bill Shupp hostmaster at shupp dot org
          - Added support for default GID of POP accounts via qmailadmin-limits
            to limit available services.  See INSTALL #6 for details.

7/27/01   Nicolas Croiset ncroiset at vdldiffusion dot com
          - Update to fr translation

7/25/01   Ender Hikmet KILICOGLU ender at kilicoglu dot nom dot tr
          - Update to tr translation

0.60pre7
7/21/01   Antonio Dias accdias at sst dot com dot br
          - Added line "227 Digest" in all languague files including "en";
          - Updated Brazilian Portuguese translation file;
          - Updated all html/add_* files; slight look change, and
            tighter html code.

7/20/01   Nicolas Croiset" ncroiset at vdldiffusion dot com
          - Update to fr translation

0.60pre6
7/19/01   Bill Shupp hostmaster at shupp dot org
          - Fixed problems with "ezmlm-make -e" by adding del_list_links();
          - moved "SQL Settings" from mailinglist.c into 099 of all 
            dictionaries (non en need translating)
          - fixed format problem in creating dir/sql file in 
            add_mailinglistnow()
          - Completed mod_mailinglist() code, now current ezmlm-idx values
            are displayed.
          - "Show Mailing Lists" no longer displays moderator functions unless 
            ezmlm-idx is installed.

7/18/01   Antonio Dias accdias at sst dot com dot br
          - adds 000 line to all language files including English;
          - adds missing lines to all language files synchronizing with
            English files (this will help translators);
          - adds message 099 to all language files including English (this
            one was skipped in the en file and all others have followed it);
          - convert a lot of files to Unix format removing CRLF line
            termination (I hate messy-DOS :-));
          - updates Brazilian Portuguese translation.

0.60pre5
7/13/01   Nicolas Croiset" ncroiset at vdldiffusion dot com
          - Update to fr translation

          Bill Shupp hostmaster at shupp dot org
          - modmailinglist() now looks up the current owner
          - fix to show_dotqmail_lines() so that idx list owners
            no longer show up in "show forwards"
          - minor html fixes to modify forwards

0.60pre4
7/12/01   Nicolas Croiset ncroiset at vdldiffusion dot com
          - Updates to en dictionary (010) and fr translation

          Bill Shupp hostmaster at shupp dot org
          - Added mod_mailinglist_idx.html. You can now "click to modify"
            ezmlm-idx lists, but it doesn't get the current values first yet.
            It uses the defaults.  But hey, it works.
          - Many more changes to mailinglist.c and related templates.
          - A few new en dictionary entries for modifying idx mailing lists

0.60pre3
7/12/01   Nicolas Croiset ncroiset at vdldiffusion dot com
          - Update to fr translation

          Bill Shupp hostmaster at shupp dot org
          - Moved "delete subscriber" and "delete moderator" into templates
          - various changes to mailinglist.c
          - freed up dictionary entry 082 (duplicate of 072)
          - moved text into en dictionary from setdefaultaccount()
          - even more fprintf -> sprintf changes
          - Moved <h2></h2> tags from sprintf statements to templates 
            (<h2>##S</h2>)
           
0.60pre2
7/11/01   Antonio Dias accdias at sst dot com dot br
          - modified html/main_menu.html to add support to i18n for some
            messages ("Main Menu" and "Log Out")
          - expanded message buffer in util.c:get_html_text() so some long
            translated messages are shown correctly.
          - updated Brazilian Portuguese translation and English dictionary
          - new "32 pixel wide" lowerright.gif

          Bill Shupp hostmaster at shupp dot org
          - more English dictionary shuffling to accomodate patches

0.60pre1
7/11/01   Bill Shupp hostmaster at shupp dot org
          - Fixes html problem with "CatchAll:" row.  It's now in its
            own table row, above the <th> tags.
          - Moved "show subcribers" and "show moderators" into
            templates.  Also, both "show subscribers" and "show moderators"
            lists now have a delete button next to each name.  
            Pretty handy, huh?
          - A few additions/mods to the en dictionary for the above changes
          - more fprintf -> sprintf changes

0.59
07/06/01  - modify user password was displaying the wrong message.
            reversed the status messages, success/failure, fixed

0.58
07/05/01  - A new rockin' delete gif courtesy of Mr. Bill Shupp

07/04/01  - *LOTS* and I mean *LOTS* of changes by Bill Shupp to
            move all the error/info messages into the html templates
            More user.c text moved into the dictionary and some
            cleanup of old changes.
0.57
07/03/01  - Change name of autorespond to Mail Robot to remove
            confusion over vacation features.

          - Remove redirect menu item for users and only show Modify user
            and user name.

          - Put back in check to not allow a mail robot to be added for
            any email address that already exists as a pop account.

          Nicolas Croiset (VDL) ncroiset at vdldiffusion dot com
          - updates to French translation

07/02/01  Oden Eriksson
          - Updated swedish language file

0.56
07/02/01  Bill Shupp 
          - Fixes mod/delete of autoresponders whose owner address is both 
            local and has a "." in the username.  Marco Leeflang pointed 
            this out.
          - Moves the text of mod_user.html into the en dictionary
          - Deletes extraneous .orig files.

06/28/01  - finished up the vacation message testing, looks good 
          - update the translation files to say "modify user"
            instead of "change password"

0.55
06/28/01  - add new modify user page to provide vacation support, 
            setting a forward or setting a forward and saving a 
            copy. almost completed, need code to put the message
            body in the page if there is a vacation set.           

06/27/01  Bill Shupp hostmaster at shupp dot org
          - fix to user/vacation code
          - addition of setting remote account for catchall

0.54
06/18/01  - add vclose before any exit
          - autoresponders can now be used as vacation 

          Bill Shupp hostmaster at shupp dot org
          - extra space in autoresponder status message

0.53
06/13/01  - Added 204 line in dictionary for Goto User

0.52    
06/13/01  - Updated to work with vpopmail-4.10 
          Stefano Cecconi stefano dot cecconi at technorail dot com
          - new Italian translation

0.45      Bill Shupp
04/24/01  - zero length check in aliases code caused bug. fixed

0.44       vol at inter7 dot com
04/18/01  - security audit and fix for language support  

0.43
Mar 15    Nicolas Croiset
          - fixed syntax error in mod_dotqmail.html, missing an X
          - fix for -5 option in mailing list.c creation
          - additional <BR> lines in main_menu.html
          - changes to French Translation

Mar 15    - missing last parameter in scat in alias.c file

Mar 13    - add AUTOMAKE_OPTIONS to Makefile.am to be more portable

Mar 03    Javier Ribelles Asco or Ultimobyte Espana, S.L sent in 
          - Spanish translation. Thanks Javier!

Feb 28 - Norwegian translation by "Einar Bordewich" einar at bordewich.net

Feb 26 - GetValue in cgi.c modified to remove trailing white space.

Feb 23 - check for zero length user names. 

Jan 31 - remove chdir(TmpBuf1) from main()
         "James" daemus at oregonfast dot net

0.42
Jan 19 - added QMAILADMIN_TEMPLATEDIR as environment variable so virtual
         domains can point to different template directories

Jan 10 - change setreuid/gid to seteuid/gid and getuid to geteuid

       - del user alias confirm and del user confirm had type=image,
	     changed to type=submit

       - delusergo was using character pointers for CGI variables instead
	     of memory. Caused pointer crash. fixed.

0.40

Dec 18 - html template support for ezmlm and ezmlm-idx

       - remove style line from html templates

       - move title into <head> section

       - better mailing list detection code
         "Chris Bolt" <chris at bolt dot cx>

       - new mailing list generation code

       - new 203 message added to language files for deleting a user from
         a mailing list.
         "Chris Bolt" chris at bolt dot cx

       - message changed for successfully deleting a user from a mailing list.
         "Chris Bolt" chris at bolt dot cx

Dec 17 - Better mailing list html
         "Nicolas Croiset (VDL)" ncroiset at vdldiffusion dot com

       - cleaned up the last of the images and html references to images

	   - Brazilian portugese translation
         Daniel Augusto Fernandes daftm at gcsnet dot com dot br

Dec 15   Support - cf-hebergement dot com support at cf-hebergement dot com> 
         and "Nicolas Croiset (VDL)" ncroiset at vdldiffusion dot com
       - French translation by 

       - German translation by Gabriel Ambuehl gabriel_ambuehl at buz dot ch

Dec 14 - update configure script to use vpopmail 4.9.7 new 
         automatic detection of -I and -L -l lines needed
         for linking and including. So cdb or mysql configurations
         do not need tinkering with the qmailadmin Makefile

Dec  4 - upgraded current release to 0.40 

0.39
Dec  4 - trailing white space in .qmail files causes the line to be ignored.
         O'Shaughnessy Evans oevans at acm.org

       - check_email_addr would not correctly parse email addresses if
         the user name had a period in it.
         O'Shaughnessy Evans oevans at acm dot org

Nov 21 - install the binary as vpopmail/vchkpw and not root

       - change --enable-ezmlmidx=y to be the default. Before
         the default was no. Which meant no ezmlm-idx code
         was being used by default.

       - check if running as root, and only attempt to setuid's if
         we are root

       - automatically check if ezmlm-idx is installed and set #define

       - update the ezmlm delete code to remove all .qmail-"list"-*
	     file names.

Nov  1 - one final pass over the ezmlm code, fixed a strcat to strcpy
         Bill Shupp hostmaster at shupp dot org

       - missing two spaces in html code generated in mailinglist.c
         "Francis P. Ling" fpling at pansar dot com.my
         Daniel Augusto Fernandes daftm at gcsnet dot com dot br

Oct 25 - fixed errors in user display page. When clicking on index or
        'next' 'prior' buttons, if there were no more users, qmailadmin
         would die. - Hampton Maxwell - Requires vpopmail-4.9.5 Oct25 

       - when clicking next page in user display, when you get to the
	     last page it no longer displays the 'next' button - Hampton Maxwell
		
Oct 19 - finally got the right code in for ezmlm-idx fix - Roland Irle

       - update Makefile.in to have  at htmllibdir at  instead of hard coded
	     location - calvin at presbychurch dot net

         calvin at presbychurch dot net
       - missing comma on line 61 in configure.in - 

       - removed last vestiage of hard coded paths from qmailadmin.c. Now
         all paths are looked up in /var/qmail/users/cdb

       - missing use of AUTORESPOND_BIN in the mod_auto_respond code, fixed

       - added first japanese translation

       - update the code to include a language path to the images

	   - moved all images to images/qmailadmin/"language"/

Oct 18 - Support for Accept-Language and translations added

Oct 17 - start moving english printfs to qlang.h #defines

Oct 7  - fix to mailing list to put a NULL on the end of the arguments becore
         calling ezmlm-make. Thanks to Nicolas Croiset

Oct 6  - change installation to root with a setreuid() and setregid() in the
         initial code.

       - add support for vpopmail 4.9.5 which allows virtual email
         domains to be created in a /etc/passwd users home dir and
         to be owned by that user. (Allows for file system user
         based quota's to provide domain based email quotas)

0.38   - testing version

0.37   
Aug 25 - allow forwards for existing accounts

0.36   
Aug 24 - remove password from successful new pop account added message.

Aug 23 - fix configure error message when it can't find the autoresponder

Aug 21 - update template.c to close <A href's with a > Thanks to
         Serge Volkov vsv at dubna dot ru

       - change the makefiles to correctly copy the html template
         files to /usr/local/share/qmailadmin/html

       - fixes to the Forwards section (prev,refresh,next) to properly
         process forwards instead of going to alias section. And 
         error checking code around chdir's
         Thanks to Yves De Muyter yves at vlaanderen dot net

0.35
Aug 8  - fix autorespond problem. If you added an autoresponder with out
         filling in the name. Then deleted the autoresponder, it would
         delete the entire domain.

       - modify the login page to display text better.

       - remove excess files from html directory.

       - clean up code indendation. Note: tab stops are set to 4 spaces.

0.34   - new configure arguments, multiples editing, ability for user to 
Aug 1   create own forward, page view for users/aliases/forwards and many 
         random bugfxes

0.31   - mailing list patch to fix ezmlm LeRoy C. Miller III

Jul 11
0.30
Jun 07 - added user.c patch from Eugene at wizoffice dot com. When deleting
         a user it adds the ability to forward all emails to another
         account.

0.26j
May 14  - fixed logic on display of aliases

        - added display of multi-line forwards

0.26i   - courtesy of Michael Boman and crew at Wizoffice dot com ltd

        - ezmlm-idx support (current version has only ezmlm support) including

        - subscribe/unsubscribe email addresses to the moderators list (DIR/mod)

        - when you create users you will have the option to subscribe them 
          to any of the mailinglists that you have in the same domain.

        - hooks to external programs when you add, modify or delete users.
    
0.26h
Apr 04  - before adding or deleting anything, check the authorization

        - Display if bounce-no-mailbox is set

        - prettied up the del alias success message

Mar 30  - allow alias, forwards and autoresponders with same name as pop user

        - fix path to maildir for alias .qmail-user files

0.26g

Mar 29  - fix setting catchall acconut if .qmail-default had bounce-no-mailbox 

Mar 28  - fix small syntax for buffer sizes

0.26f

Feb 29  - update auth.c code to use pw->pw_dir for sub directory users

0.26e

Feb 18 - Modify user screen did not take Gecos field update
         - Didn't set default gecos correctly

0.26

Feb 16 - fixed autorespond.c code for deleting 
           added fix to user.c to check return status of vadduser.

Jan 19 - fixed autorespond.c code to add autorespon to AUTORESPOND_BIN

dec 20 - added library checks for crypt, floor, socket, gethostbyname
         to match vpopmail 3.4.11 

dec 16 - added patch from Peter Green pcg at gospelcom dot net to only
         display users informaion when user logs in

dec 16 - added code to not allow user to delete thier own account

       - removed all snprintf calls, non portable

       - removed need for fgetpwent function

       - replaced all fork/execl calls that were deleting files with system
         function calls to delete them.

       - added --enable-vpopmaildir configure option with default 
	     ../vpopmail-3.4.10

       - fixed bug that didn't remove the *.qw files in the postmaster/Maildir

       - uses vpopmail 3.4.10 libvpopmail.a library. All function code that
         depended on vpasswd files are replaced with vpopmail api functions.

0.25

- new graphics and image layouts

- fixes for mailling list errors

- suse linux patch to remove all popen() and system() calls and use
  fork/execl

- Bug fixes for various files

- removed unused set_compression function


0.24

- update to match vchkpw's move of vpasswd to users/vpasswd

- fixed makefile gnu dependencies, compiles under solaris make now.

- When anything is deleted, a confirmation page pops up to double check.
  this is for autoresponders, mailinglists, users, aliases and forwards. 

- broke the code up into more and smaller c files

0.23

- fixed change user password bug.

- Successfully deletes a user even if they have mail in thier maildir.
  logic error fixed.

- better support for systems that don't have fgetpwent in them.
  Configure automatically detects if it is available or not. 
  If not, it uses a version in qmailadmin.c

0.22

- when adding anything, checks to see if the name is already used as
  a pop user, forward/alias, mailing list or autoresponder. Returns
  error if name already exists 

- removed <HTML> blah blah printf during global init. Those tags are
  now in the template files

- Default value for Pop Account set to "postmaster"

- better checking on local email names and remote email addresses
  no control characters, spaces, non RFC characters.

- forwards and aliases that contain a "." are converted to ":" for
  creating the .qmail file. eg joe.shmoe becomes .qmail-joe:shmoe

- removed compress default settings since it doesn't auto configure
  correctly on all machines

- fixed a few newlines within strings that SCO compiler complained about

- real install documenation

- system level postmaster login

- add/delete virtual domains for system level postmaster

0.21a

- logic errors in tokenizing parts of alias, forward,mailinglist and 
  autoresponder code

0.21

- moved configuration defines to config.h

- removed snprintf.o from the objects. caused everything to die

- put in bounds checking for user/file names when scanning directories

- put back in autoresponder tag in main menu  

- added max popusers, aliases, forwards, autoresponders, mailing lists to
  configure for setting at compile time for system wide defaults

- Checks for lower case force on all email addresses

0.20

- uninstall works too

- added installation of html templates

- added the beginings of the template system

- put local of ezmlm directory into configure

- put path to autorespond program into configure

- put user for vpopmail in configure

- put url path from base to qmailadin in configure /cgi-bin/qmailadmin 

- put base qmail install dir in configure, default is /var/qmail

0.19

- added autoconf support for ./configure ; make ; make install support

- Added support for BSD

- If any of the .qmailadmin-limits values are set to zero, then
  that functionality is disabled. For example: maxmailinglists 0
  will not allow that domain to add mailing lists. Setting it to -1
  means unlimited.

- Added CGI_BIN_DIR define in Makefile

- Added setting default limits in qmailadmin.h. Distritbuted default
  is unlimited. By changing these values the installer can set the
  system wide defaults and hopefully reduce the number of .qmailadmin-limits
  files that need to be installed in each virtual domain directory.

- added makefile dependencies

- added logout button

- added code to count everything and display it

- started adding system/domain/user admin login 

- added snprintf module for solaris 2.5.1 support

0.18

- Added reading of a .qmailadmin-limits file for setting
  max number of pop accounts, aliases, forwards, autoresponders and 
  mailing lists.

- Added check of max number of pop accounts and rejecting with an
  error message if the maximum is reached or exceeded.

- Added check of max number of aliases.

- Added check of max number of forwards.

- Added check of max number of mailing lists.

- Added check of max number of autoresponders.

- Added closedir calls for sections of code that had called opendir. 

- Fixed a bug in add pop account creation of encrypted password .

- First public release on the net.

0.17

- Added modify autoresponder feature.

- added auto detection of client browser compression.
  If the browser supports gzip or x-gzip compression
  then the page is compressed and sent to the browser.

- re-organized code into multiple files based on features
  user.c alias.c forward.c autorespond.c mailinglist.c

- added GPL license.

- wrote first INSTALL doc


0.16

- fixed memory error due to null termination of content length


