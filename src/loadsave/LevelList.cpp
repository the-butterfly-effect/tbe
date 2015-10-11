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

#include "LevelList.h"
#include "tbe_global.h"
#include "tbe_paths.h"

#include <qsettings.h>

// TODO: get rid of!
#include "Popup.h"

LevelList::LevelList(const QString& aFileName )
{
	DEBUG1ENTRY;
	theNr = 0;
	currentText.clear();

	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);

	QFile file(aFileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		// TODO: MOVE THIS AWAY
		// note that critical won't return...
		Popup::Critical(QObject::tr("Level parser:\n"
						"Cannot read the level descriptions in '%1':\n%2.")
						.arg(aFileName).arg(file.errorString()));
		exit(3);
	}

//	QXmlInputSource xmlInputSource(&file);
//	if (reader.parse(xmlInputSource))
//	{
//		// find the first item that not has "done" in the NR_COLUMN field
//		QTreeWidgetItemIterator it(m_ui->theTreeWidget);
//		while( *it )
//		{
//			QString myLineStatus = (*it)->text(NR_COLUMN);
//			if ( myLineStatus.contains(QRegExp("[0-9]")) )
//			{
//				m_ui->theTreeWidget->setCurrentItem(*it);
//				(*it)->setSelected(true);
//				break;
//			}
//			++it;
//		}
//		return true;
//	}
//	return false;
}

bool LevelList::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
	DEBUG5ENTRY;
	if (qName == "level")
	{
		// remove any starting/trailing whitespace and add the path name
		currentText = LEVELS_DIRECTORY + "/" + currentText.trimmed();

		LevelMetaInfo myLevelInfo;
		myLevelInfo.theFileName = currentText;

		// read Level title and Level description from the level file
		FastLevelParser myParser;
		QXmlSimpleReader reader;
		reader.setContentHandler(&myParser);
		reader.setErrorHandler(&myParser);
		QFile file(currentText);
		if (!file.open(QFile::ReadOnly | QFile::Text))
			goto RETURN;

		QXmlInputSource xmlInputSource(&file);
		reader.parse(xmlInputSource);
		if (myParser.theTitle.isEmpty()==false)
			myLevelInfo.theTitle = myParser.theTitle;
		if (myParser.theDescription.isEmpty()==false)
			myLevelInfo.theDescription = myParser.theDescription;

		QSettings mySettings;
		QString myLevelStatus = mySettings.value("completed/"+currentText).toString();
		if (myLevelStatus.isEmpty()==false)
		{
			if (myLevelStatus=="done")
				myLevelInfo.theStatus = LevelMetaInfo::COMPLETED;
			else if (myLevelStatus=="skipped")
				myLevelInfo.theStatus = LevelMetaInfo::SKIPPED;
			else
				myLevelInfo.theStatus = LevelMetaInfo::FRESH;
		}
	}
RETURN:
	currentText.clear();
	return true;
}

bool LevelList::fatalError(const QXmlParseException &exception)
{
	DEBUG1ENTRY;
	// TODO: get rid of!
	Popup::Critical(QObject::tr("LevelList:\nParse error at line %1, column %2:\n%3")
					.arg(exception.lineNumber())
					.arg(exception.columnNumber())
					.arg(exception.message()));
	return false;
}


// ###################################################

bool LevelList::FastLevelParser::endElement(const QString & /* namespaceURI */,
											const QString & /* localName */,
											const QString &qName)
{
	DEBUG1ENTRY;
	if (qName == "title")
		theTitle.add(currentText.trimmed(), theAttrs.value("lang"));

	if (qName == "description")
		theDescription.add(currentText.trimmed(), theAttrs.value("lang"));

	// no need to parse everything beyond the levelinfo
	if (qName == "levelinfo")
		return false;

	printf("'%s': ", ASCII(theTitle.all()));
	printf("'%s'\n", ASCII(theDescription.all()));

	currentText = "";
	return true;
}


bool LevelList::FastLevelParser::startElement(const QString &, const QString &,
					   const QString &qName, const QXmlAttributes &attributes)
{
	DEBUG1ENTRY;
	// this potentially is an expensive copy operation, let's reduce it...
	if (qName == "title" || qName == "description")
	{
		theAttrs = attributes;
	}
	return true;
}
