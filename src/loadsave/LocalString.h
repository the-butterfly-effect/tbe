/* The Butterfly Effect
 * This file copyright (C) 2010,2013  Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef LOCALSTRING_H
#define LOCALSTRING_H

#include <QtCore/QString>
#include <QtXml/QDomNode>
#include <QtCore/QMap>

/// this QString-on-steroids keeps copies of all localized strings
/// but will only return the string in the currently set locale
class LocalString
{
public:
    LocalString();

    /** adds string aValue and its locale to the list
      *
      *  Note: you'll probably use multiple calls to check() if you are using the SAX model
      *  @param aValue   string to be copied if one of the above rules matches
      *  @param aLangCode  the locale code - either empty, or "nl" or "nl_BE" format
      */
    void add(const QString& aValue, const QString& aLangCode);

    /** read within the DomNode Dom to find all tags with a lang="lang"
      * and find the string which fits the current locale best
      * example:
      *         <toolboxitem count="9" name="Bowling Pin">
      *				<name lang="nl">Kegel</name>
      *				<name lang="pt_BR">###</name>
      *             <object type="Bowling Pin" />
      *			</toolboxitem>
      * will find "###" if lang is "pt_BR" and "Bowling Pin" if lang is "de_AT".
      * see class LocalString for more explanation on the matching algorithms
      *
      *  Note: you'll probably use one call to fillFromDom() if you are using the DOM model
      * @param aNode		the DOM Node that we're analysing (in the above example 'toolboxitem')
      * @param aTagString	the tag to look for (in the above example 'name')
      * @param aDefault		the value for the default locale (in the above example 'Bowling Pin')
      *                     (if there is a <name> without a lang tag, that one
      *						will be used as default, so you won't need to
      *						supply it as a programmer)
      */
    void	fillFromDOM(const QDomNode& aNode,
                        const QString& aTagString, const QString& aDefault="");

    /** serialize all names except the default to aParentElement
      * @param aParentElement QDomElement to serialize to
      */
    void    serializeTo(QDomElement& aParentElement) const;

    bool isEmpty(void)
    { return theStringList.isEmpty(); }

    /// returns a string will all entries
    QString all() const;

    /**
      *  we follow these rules:
      *    1) if nothing set and we find a <title> without a language - let's use it
      *    2) if we find a title with the corresponding language - let's use it instead of rule 1
      *    3) if we find a title with a full corresponding "language_country", let's use that one instead of 1 or 2
      */
    QString result() const;

    void clear();

    QString english() const;

    bool operator== (const LocalString& anOtherOne) const
    { return this->english() == anOtherOne.english();	}

    bool operator< (const LocalString& anOtherOne) const
    { return this->english() < anOtherOne.english();	}


private:
    QString the5Char;

    /// contains all strings read
    typedef QMap<QString,QString> LocalStringList;
    LocalStringList theStringList;

    friend class TestLocalString;

    // TODO/FIXME: This one shouldn't be necessary :-(
    friend class Level;
};

#endif // LOCALSTRING_H
