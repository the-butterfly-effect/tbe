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

LevelList::LevelList(const QString& aBaseDir, const QString &aFileName)
		: theBaseLevelsDir(aBaseDir)
{
	DEBUG5ENTRY;
	theNr = 0;
	currentText.clear();

	if (!theBaseLevelsDir.endsWith("/"))
		theBaseLevelsDir+="/";

	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);

	QFile file(theBaseLevelsDir + aFileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		// TODO: MOVE THIS AWAY
		// note that critical won't return...
		Popup::Critical(QObject::tr("Level parser:\n"
						"Cannot read the level descriptions in '%1':\n%2.")
						.arg(theBaseLevelsDir+aFileName).arg(file.errorString()));
		exit(3);
	}

	QXmlInputSource xmlInputSource(&file);
	if (!reader.parse(xmlInputSource))
	{
		// TODO: fouteboel
		printf("FOUTEBOEL\n");
		exit(684);
	}
}

int LevelList::findNameInList(const QString &aName)
{
	if (theMetaList.isEmpty())
		return -1;
	for (int i=0; i< theMetaList.size(); i++)
	{
		if (theMetaList.at(i).theFileName == aName)
			return i;
	}
	return -1;
}


QString LevelList::getFirstLevel()
{
	if (theMetaList.isEmpty())
		return "";
	else
		return theMetaList.first().theFileName;
}

LevelList::LevelMetaInfo LevelList::getLevelMetaInfo(QString aName)
{
	int i = findNameInList(aName);
	if (i==-1)
	{
		// empty LMI
		LevelMetaInfo myLMI;
		return myLMI;
	}
	return theMetaList.at(i);
}

QString LevelList::getNextLevel(QString aName)
{
	int i = findNameInList(aName);
	if (i==-1)
		return "";
	if (i+1 == theMetaList.size())
		return "";
	return theMetaList.at(i+1).theFileName;
}

QString LevelList::getNextToPlayLevel()
{

}



bool LevelList::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
	DEBUG5ENTRY;
	if (qName == "level")
	{
		// remove any starting/trailing whitespace and add the path name
		currentText = theBaseLevelsDir + currentText.trimmed();

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
		theMetaList.push_back(myLevelInfo);
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
	DEBUG5ENTRY;
	if (qName == "title")
		theTitle.add(currentText.trimmed(), theAttrs.value("lang"));

	if (qName == "description")
		theDescription.add(currentText.trimmed(), theAttrs.value("lang"));

	// no need to parse everything beyond the levelinfo
	if (qName == "levelinfo")
		return false;

	currentText = "";
	return true;
}


bool LevelList::FastLevelParser::startElement(const QString &, const QString &,
					   const QString &qName, const QXmlAttributes &attributes)
{
	DEBUG5ENTRY;
	// this potentially is an expensive copy operation, let's reduce it...
	if (qName == "title" || qName == "description")
	{
		theAttrs = attributes;
	}
	return true;
}
