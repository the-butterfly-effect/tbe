/* The Butterfly Effect
 * This file copyright (C) 2009,2015  Klaas van Gend
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

#ifndef LevelList_H
#define LevelList_H

#include "LocalString.h"
#include <QtCore/QCoreApplication>
#include <QtXml/QXmlDefaultHandler>

/// this class contains a list of all levels
class LevelList : private QXmlDefaultHandler
{
public:
	/// constructor
	LevelList(const QString &aFileName);

	QString errorString() const
	{ return errorStr; }

private:
	friend class QXmlSimpleReader;
	/// implemented from QXmlDefaultHandler
	bool endElement(const QString &namespaceURI, const QString &localName,
					const QString &qName);
	/// implemented from QXmlDefaultHandler
	bool characters(const QString &aStr)
	{ currentText += aStr; return true; }

	/// implemented from QXmlDefaultHandler
	bool fatalError(const QXmlParseException &exception);

private:
	int		theNr;
	QString currentText;
	QString errorStr;

	/// Describes the meta-info for a level in the game
	struct LevelMetaInfo
	{
		enum Status
		{
			FRESH,
			SKIPPED,
			COMPLETED
		};

		Status		theStatus;
		QString     theFileName;
		LocalString theTitle;
		LocalString theDescription;
	};

	typedef QList<LevelMetaInfo> MetaInfoList;
	MetaInfoList theMetaList;


	/** this class is used after the above LevelList class to quickly retrieve
	  * title and description from a level (in the right locale)
	  */
	class FastLevelParser : public QXmlDefaultHandler
	{
	public:
		/// empty constructor - Note that the file name is specified on a different call
		FastLevelParser(void) {};

		/// implemented from QXmlDefaultHandler
		virtual bool endElement(const QString &namespaceURI, const QString &localName,
						const QString &qName);

		/// implemented from QXmlDefaultHandler
		virtual bool characters(const QString &aStr)
		{ currentText += aStr; return true; }


		virtual bool startElement(const QString &namespaceURI, const QString &localName,
					   const QString &qName, const QXmlAttributes &attributes);

		QString errorString() const
		{ return errorStr; }

		LocalString theTitle;
		LocalString theDescription;

		QXmlAttributes theAttrs;

	private:
		QString currentText;
		QString errorStr;

	}; // end-of FastLevelParser
};

#endif // LevelList_H
