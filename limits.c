/* 
 * $Id: limits.c,v 1.2.2.1 2004-11-20 01:10:41 tomcollins Exp $
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

#include "config.h"
/* undef some macros that get redefined below */
#undef PACKAGE
#undef VERSION
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef QMAILDIR
#include "qmailadmin.h"
#include "qmailadminx.h"
#include "limits.h"

void load_limits()
{
  vget_limits(Domain, &Limits);
  MaxPopAccounts = Limits.maxpopaccounts;
  MaxAliases = Limits.maxaliases;
  MaxForwards = Limits.maxforwards;
  MaxAutoResponders = Limits.maxautoresponders;
  MaxMailingLists = Limits.maxmailinglists;

  DisablePOP = Limits.disable_pop;
  DisableIMAP = Limits.disable_imap;
  DisableDialup = Limits.disable_dialup;
  DisablePasswordChanging = Limits.disable_passwordchanging;
  DisableWebmail = Limits.disable_webmail;
  DisableRelay = Limits.disable_relay;
}
