# `QmailAdmin`

This `QmailAdmin` distribution puts together Inter7's last `qmailadmin-1.2.16` with the following features:

* `qmailadmin-skin`, which provides a new responsive skin to the control panel. It modifies everything under the html dir and many `.c` files in order to adjust the html embedded into the source files. Added a style sheet in the *images* folder and a couple of png files for the `qmail` logo.
* A patch to call `cracklib` in order to check for the password strenght. This should avoid unsafe accounts created by domain administrators such as *test 123456*. This patch enhanses the original patch by Inter7.
* A patch (original author unknown) which gets `qmailadmin` to have authentication failures logged. This makes possible to ban malicious IPs via fail2ban. It is required to create the log file */var/log/qma-auth.log* initially and assign write priviledges to `apache`.
* `ezmlm-idx 7` compatibility patch (author unknown), which restores the compatibility with `ezmlm-idx-7` (thanks to J.D. Trolinger for the advice).
* a fix to the catchall account
* another fix to autorespond.c to correct the way `.qmail` files are modified

The patches applied can be found in the contrib directory.

## Support
To ask for support and find more informations have a look to the following page: https://notes.sagredo.eu/en/qmail-notes-185/qmailadmin-23.html
