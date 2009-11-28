/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#include "Level.h"
#include "World.h"
#include "BaseObjectSerializer.h"
#include "GoalSerializer.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

// Strings identifying elements/nodes in the XML file
// (the ones without static are also used in BaseObjectSerializer.cpp)
//
static const char* theRootNodeString= "tbe-level";
static const char* theLevelInfoString = "levelinfo";
	static const char* theLevelAuthorString       = "author";
	static const char* theLevelDateString         = "date";
	static const char* theLevelDescriptionString  = "description";
	static const char* theLevelLicenseString      = "license";
	static const char* theLevelNameString         = "title";
static const char* theSceneString = "scene";
	static const char* theSceneSizeString  = "scenesize";
	static const char* theViewString       = "view";
	static const char* thePredefinedString = "predefined";
		   const char* theObjectString     = "object";
		   const char* thePropertyString   = "property";
static const char* theToolboxString = "toolbox";
static const char* theGoalsString = "goals";
		   const char* theGoalString = "goal";

const char* theWidthAttributeString     = "width";
const char* theHeightAttributeString    = "height";
const char* theXAttributeString         = "X";
const char* theYAttributeString         = "Y";
const char* theAngleAttributeString     = "angle";
const char* theTypeAttributeString      = "type";
const char* theIDAttributeString        = "ID";

// static (always accessible) data member
// for this file only
static QString theFileName;



// Constructors/Destructors
//  

Level::Level ( ) 
{
    theWorldPtr = new World();
}

Level::~Level ( ) 
{
	DEBUG5("World::~World - clear the ObjectPtrList \n");
    delete theWorldPtr;
    theWorldPtr = NULL;
}

//  
// Methods
//  

void
Level::addBaseObject(QDomElement aParent, const BaseObject& anObjectRef) const
{
	const BaseObjectSerializer* myBOS = anObjectRef.getSerializer();
	if (myBOS!=NULL)
	{
		myBOS->serialize(&aParent);
		delete myBOS;
	}
}

QString
Level::getPathToLevelFile(void)
{
	if (theFileName.isEmpty())
		return ".";
	else
		return QFileInfo(theFileName).absolutePath();
}

QString
Level::getLevelFileName(void)
{
	return theFileName;
}

QString
Level::load(const QString& aFileName)
{
	theFileName = aFileName;

	QString myErrorMessage = tr("Cannot read file '%1'").arg(aFileName);
	QDomDocument myDocument("mydocument");
	
	QDomNode myNode, q;

	// TODO: rename mySceneNode to myActiveNode
	QDomNode mySceneNode;

	QDomElement myElement;
	QDomElement myDocElem;
	QDomNamedNodeMap myNodeMap;
    bool isOK1, isOK2;

    qreal myWidth;
    qreal myHeight;
    
	QFile myFile(aFileName);
	if (!myFile.open(QIODevice::ReadOnly))
	{
		myErrorMessage = tr("Cannot read file %1").arg(aFileName);
		goto not_good;
	}

	if (!myDocument.setContent(&myFile))
	{
		myErrorMessage = tr("Cannot parse file - not valid XML?");
		goto not_good;
	}
	myFile.close();

	myDocElem = myDocument.documentElement();

	//
	// parse the Level Info section
	//
	myErrorMessage = tr("Parsing '%1' section failed: ").arg(theLevelInfoString);
	myNode=myDocElem.firstChildElement(theLevelInfoString);
	if (myNode.isNull())
		goto not_good;
	theLevelName       = myNode.firstChildElement(theLevelNameString).text();
	theLevelAuthor     = myNode.firstChildElement(theLevelAuthorString).text();
	theLevelLicense    = myNode.firstChildElement(theLevelLicenseString).text();
	theLevelDescription= myNode.firstChildElement(theLevelDescriptionString).text();

	if (theLevelName.isEmpty() || theLevelAuthor.isEmpty() || theLevelLicense.isEmpty())
	{
		myErrorMessage += "level, author or license info missing";
		goto not_good;
	}
	DEBUG5("level name:    '%s'\n", ASCII(theLevelName));
	DEBUG5("level author:  '%s'\n", ASCII(theLevelAuthor));
	DEBUG5("level license: '%s'\n", ASCII(theLevelLicense));
	theWorldPtr->theLevelName = theLevelName;

	theToolboxDomNode = myDocElem.firstChildElement("toolbox");

	//
	// parse the Scene section
	//
	mySceneNode=myDocElem.firstChildElement(theSceneString);

	myErrorMessage = tr("Parsing '%1' section failed").arg(theSceneSizeString);
	myNode=mySceneNode.firstChildElement(theSceneSizeString);
	myNodeMap = myNode.attributes();
	myWidth = myNodeMap.namedItem(theWidthAttributeString).nodeValue().toDouble(&isOK1);
	myHeight= myNodeMap.namedItem(theHeightAttributeString).nodeValue().toDouble(&isOK2);
	if (!isOK1 || !isOK2)
	{
		myErrorMessage += tr("scene width or height unspecified");
		goto not_good;
	}
	theWorldPtr->theWorldWidth=myWidth;
	theWorldPtr->theWorldHeight=myHeight;

	// TODO: implement view
	myErrorMessage = tr("Parsing '%1' section failed").arg(theViewString);
	myNode=mySceneNode.firstChildElement(theViewString);
	
	
	myErrorMessage = tr("Parsing '%1' section failed: ").arg(thePredefinedString);
	myNode = mySceneNode.firstChildElement(thePredefinedString);
	for (q=myNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// an object entry has the following layout:
		// <object type="Ramp" X="1.0" Y="0.5" width="2.0"/>
		// of these, type, X and Y are *MANDATORY*
		// optional are:  angle, width, height
	
		// simple sanity checks
		if (q.nodeName() == "#comment")
			continue;
		if (q.nodeName() != theObjectString)
		{
			myErrorMessage += tr("expected a <%1> section, got <%2>").arg(theObjectString).arg(q.nodeName());
			goto not_good;
		}

		BaseObject* myBOPtr = BaseObjectSerializer::createObjectFromDom(q, true);
		if (myBOPtr == NULL)
		{
			myErrorMessage += tr("createObjectFromDom failed");
			goto not_good;
		}
		if (theIsLevelEditor==false)
			myBOPtr->setIsMovable(false);
		theWorldPtr->addObject(myBOPtr);

		if (q==myNode.lastChild())
			break;
	}
	
	//
	// parse the Goal section
	// 
	myErrorMessage = tr("Parsing '%1' section failed: ").arg(theGoalsString);
	mySceneNode=myDocElem.firstChildElement(theGoalsString);
	if (mySceneNode.nodeName()!= theGoalsString)
	{
		myErrorMessage += tr("no <%1> section found!").arg(theGoalsString);
		goto not_good;
	}
	for (q=mySceneNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// a goal entry has the following layout:
		//	<goal type="distance" lessthan="0.3">
		//		 <object ID="Butterfly"/>
		//		 <object ID="Flower"/>
		//	</goal>
		//
		// of these, 'type' is mandatory
		// everything else is optional and depends on the type of goal

		// simple sanity checks
		if (q.nodeName() == "#comment")
			continue;
		if (q.nodeName() != theGoalString)
		{
			myErrorMessage += tr("expected a <%1> section, got <%2>").arg(theGoalString).arg(q.nodeName());
			goto not_good;
		}

		Goal* myGPtr = GoalSerializer::createObjectFromDom(q);
		if (myGPtr == NULL)
		{
			myErrorMessage += tr("createObjectFromDom failed");
			goto not_good;
		}
		if (myGPtr->parseProperties(theWorldPtr)==false)
		{
			myErrorMessage += tr("<%1> properties could not be parsed").arg(theGoalString);
			goto not_good;
		}

		theWorldPtr->addGoal(myGPtr);

		if (q==myNode.lastChild())
			break;
	}

	
	
	// and everything went OK - we're done :-)
	myErrorMessage = "";

	// if goto not_good was called, we get here, too
	// theErrorMessage is set - let's return it!
not_good:
	return myErrorMessage;
}


void
Level::addTextElement(QDomElement aParent, const QString& anElementName, const QString& aText) const
{
	QDomElement myReturn = aParent.ownerDocument().createElement(anElementName);
	QDomText myT = aParent.ownerDocument().createTextNode(aText);
	myReturn.appendChild(myT);
	aParent.appendChild(myReturn);
}


bool Level::save(const QString& aFileName)
{
	DEBUG5("Level::save(%s)\n", ASCII(aFileName));
	QDomDocument myDocument("mydocument");
	QDomElement myRoot = myDocument.createElement(theRootNodeString);
	myDocument.appendChild(myRoot);

	// LevelInfo
	QDomElement myLevelInfo = myDocument.createElement(theLevelInfoString);
	myRoot.appendChild(myLevelInfo);
	addTextElement(myLevelInfo, theLevelNameString, theLevelName);
	addTextElement(myLevelInfo, theLevelAuthorString, theLevelAuthor);
	addTextElement(myLevelInfo, theLevelLicenseString, theLevelLicense);
	addTextElement(myLevelInfo, theLevelDescriptionString, theLevelDescription);
	addTextElement(myLevelInfo, theLevelDateString, theLevelDate);

	// Scene
	QDomElement mySceneParent = myDocument.createElement(theSceneString);
	myRoot.appendChild(mySceneParent);
	// ... add scenesize
	QDomElement mySceneSizeNode = myDocument.createElement(theSceneSizeString);
	mySceneSizeNode.setAttribute(theWidthAttributeString, theWorldPtr->theWorldWidth);
	mySceneSizeNode.setAttribute(theHeightAttributeString, theWorldPtr->theWorldHeight);
	mySceneParent.appendChild(mySceneSizeNode);	
	// ... add the predefined elements
	QDomElement myPredefinedParent = myDocument.createElement(thePredefinedString);
	mySceneParent.appendChild(myPredefinedParent);
	World::BaseObjectPtrList::iterator myI = theWorldPtr->theObjectPtrList.begin();
	for (; myI != theWorldPtr->theObjectPtrList.end(); ++myI)
		addBaseObject(myPredefinedParent, *(*myI));
	// ... TODO: add view


	// Toolbox
	QDomElement myToolboxParent = myDocument.createElement(theToolboxString);
	myRoot.appendChild(myToolboxParent);

	// success: we're going to write!
	QFile myFile(aFileName);
	if (!myFile.open(QFile::WriteOnly | QFile::Text))
	{
		QString myError = tr("Cannot write file '%1': %2.")
			.arg(aFileName, myFile.errorString());
		DEBUG1("ERROR: %s\n", ASCII(myError));
		return false;
	}
	const int IndentSize = 4;
	QTextStream myOut(&myFile);
	myDocument.save(myOut, IndentSize);
	return true;
}

void
Level::setLevelFileName(const QString& aName)
{
	theFileName = aName;
}
