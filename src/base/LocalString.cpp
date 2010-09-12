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

void LocalString::add(const QString& aValue, const QString& aLangCode)
{
		theStringList.insert(aLangCode, aValue);
}

void LocalString::clear()
{
	theStringList.clear();
}

QString LocalString::english() const
{
	if (theStringList.count()==0)
		return "";
	else
		return theStringList.find("").value();
}

void
LocalString::fillFromDOM(
		const QDomNode& aNode,
		const QString& aTagString,
		const QString& )
{
	QDomElement myE = aNode.firstChildElement(aTagString);
	while (myE.isNull()==false)
	{
		theStringList.insert(myE.attribute("lang",""), myE.text());
		myE = myE.nextSiblingElement(aTagString);
	}
}

QString LocalString::result() const
{
	QString myReturn;
	QString myLang;

	LocalStringList::const_iterator i = theStringList.begin();
	while (i != theStringList.end())
	{
		QString aLangCode = i.key();
		QString aValue = i.value();

		// we follow these rules:
		// 1) if nothing set and we find a <title> without a language - let's
		//    use it
		// 2) if we find a title with the corresponding language - let's use
		//    it instead of rule 1
		// 3) if we find a title with a full corresponding "language_country",
		//    let's use that one instead of 1 or 2

		// rule 1
		if (aLangCode.isEmpty() && myLang.isEmpty())
			myReturn = aValue;

		// rule 2
		if (myLang.size()<3 && aLangCode.left(2) == the5Char.left(2))
		{
			// but not if we already have "nl", are looking for "nl_NL" and we now get "nl_BE"
			if ( !(aLangCode.size()==5 && myLang.size()==2) )
			{
				myReturn = aValue;
				myLang = aLangCode.left(2);
			}
		}

		// rule 3
		if (myLang.size()<3 && aLangCode == the5Char)
		{
			myReturn = aValue;
			myLang = aLangCode;
		}

		++i;
	}
	return myReturn;
}
