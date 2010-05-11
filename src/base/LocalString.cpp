/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "LocalString.h"

#include <QLocale>

// ###################################################

LocalString::LocalString(void)
{
	QLocale mySysLocale = QLocale::system();
	the5Char = mySysLocale.name();
}

void LocalString::check(const QString& aValue, const QString& aLangCode)
{
	// rule 1
	if (aLangCode.isEmpty() && theLang.isEmpty())
		theString = aValue;

	// rule 2
	if (theLang.size()<3 && aLangCode.left(2) == the5Char.left(2))
	{
		// but not if we already have "nl", are looking for "nl_NL" and we now get "nl_BE"
		if ( !(aLangCode.size()==5 && theLang.size()==2) )
		{
			theString = aValue;
			theLang = aLangCode.left(2);
		}
	}

	// rule 3
	if (theLang.size()<3 && aLangCode == the5Char)
	{
		theString = aValue;
		theLang = aLangCode;
	}
}

void
LocalString::fillFromDOM(
		const QDomNode& aNode,
		const QString& aTagString,
		const QString& aDefault)
{
	if (aDefault.isEmpty()==false)
		check(aDefault, "");

	QDomElement myE = aNode.firstChildElement(aTagString);
	while (myE.isNull()==false)
	{
		check(myE.text(), myE.attribute("lang",""));
		myE = myE.nextSiblingElement(aTagString);
	}
	printf("return: '%s'\n", ASCII(result()));
}

