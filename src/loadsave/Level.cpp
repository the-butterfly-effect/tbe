/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011  Klaas van Gend
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

#include "AbstractObjectSerializer.h"
#include "BackgroundSerializer.h"
#include "GameResources.h"
#include "Goal.h"
#include "GoalSerializer.h"
#include "Hint.h"
#include "HintSerializer.h"
#include "Level.h"
#include "ToolboxGroupSerializer.h"
#include "World.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

// Strings identifying elements/nodes in the XML file
// (the ones without static are also used in AbstractObjectSerializer.cpp)
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
		   const char* theToolTipString    = "tooltip";
	const char* theBackgroundString = "background";
static const char* theToolboxString = "toolbox";

static const char* theGoalsString = "goals";
	   const char* theGoalString  = "goal";

static const char* theHintsString = "hints";
	   const char* theHintString  = "hint";

const char* theWidthAttributeString     = "width";
const char* theHeightAttributeString    = "height";
const char* theXAttributeString         = "X";
const char* theYAttributeString         = "Y";
const char* theAngleAttributeString     = "angle";
const char* theTypeAttributeString      = "type";
const char* theIDAttributeString        = "ID";
const char* theIsFailAttributeString    = "isFail";

// static (always accessible) data member
// for this file only
static QString theFileName;


static Level* theCurrentLevelPtr = nullptr;

// Constructors/Destructors
//

Level::Level ( )
{
	theFileName = "";
	theWorldPtr = new World();
	theWorldPtr->theWorldWidth  = 3.0;
	theWorldPtr->theWorldHeight = 2.0;

	assert(theCurrentLevelPtr==nullptr);
	theCurrentLevelPtr = this;
}

Level::~Level ( )
{
    DEBUG1ENTRY;
    assert(theCurrentLevelPtr==this);
    theCurrentLevelPtr=nullptr;
    delete theWorldPtr;
    theWorldPtr = nullptr;

	while(theHintPtrList.size())
	{
		auto i = theHintPtrList.back();
		theHintPtrList.pop_back();;
		delete i;
	}
}

//
// Methods
//


ToolboxGroup*
Level::findToolBoxGroup(AbstractObjectPtr anAOPtr)
{
	foreach(ToolboxGroup* i, theCurrentLevelPtr->theToolboxList)
	{
		if (i->theInternalName == anAOPtr->getInternalName())
			return i;
	}
	return nullptr;
}


QString
Level::getPathToLevelFile(void)
{
	if (theFileName.isEmpty())
		return ".";
	else
		return QFileInfo(theFileName).absolutePath();
}


Hint *Level::getHint(int anIndex)
{
	if (anIndex >= theHintPtrList.size())
		return nullptr;
	else
		return theHintPtrList.at(anIndex);
}


QString
Level::getLevelFileName()
{
	return theFileName;
}


QString
Level::load(const QString& aFileName, GameResources* aLevelInfoToolbox)
{
	theFileName = aFileName;
	DEBUG2("Level::load(\"%s\")", ASCII(aFileName));

	QString myErrorMessage = tr("Cannot read file '%1'").arg(aFileName);
	QDomDocument myDocument("mydocument");

	QDomNode myNode, q;

	// TODO: rename mySceneNode to myActiveNode
	QDomNode mySceneNode;

	QDomElement myElement;
	QDomElement myDocElem;
	QDomNamedNodeMap myNodeMap;
	bool isOK1, isOK2;
	bool hasProblem = false;
	QString myResult;

    qreal myWidth;
    qreal myHeight;

	QFile myFile(aFileName);
	if (!myFile.open(QIODevice::ReadOnly))
	{
		myErrorMessage = tr("Cannot read file '%1'").arg(aFileName);
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
	theLevelName       .fillFromDOM(myNode,theLevelNameString);
	theLevelAuthor     = myNode.firstChildElement(theLevelAuthorString).text();
	theLevelLicense    = myNode.firstChildElement(theLevelLicenseString).text();
	theLevelDate       = myNode.firstChildElement(theLevelDateString).text();
	theLevelDescription.fillFromDOM(myNode, theLevelDescriptionString);

	DEBUG5("level name:    '%s'", ASCII(theLevelName.result()));
	DEBUG5("level author:  '%s'", ASCII(theLevelAuthor));
	DEBUG5("level license: '%s'", ASCII(theLevelLicense));
	theWorldPtr->theLevelName = theLevelName.result();

	//
	// parse the Toolbox section
	//
	myErrorMessage = tr("Parsing '%1' section failed: ").arg(theToolboxString);
	myNode=myDocElem.firstChildElement(theToolboxString);
	for (q=myNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// no sanity checks, leave that to the serializer
		QString myExtraError;
		ToolboxGroup* myTbGPtr = ToolboxGroupSerializer::createObjectFromDom(q, &myExtraError);
		if (myTbGPtr == nullptr)
		{
			myErrorMessage += myExtraError;
			goto not_good;
		}
		theToolboxList.insert(myTbGPtr->theGroupName, myTbGPtr);
	}

	//
	// parse the Scene section
	//
	mySceneNode=myDocElem.firstChildElement(theSceneString);

	myErrorMessage = tr("Parsing '%1' section failed: ").arg(theSceneSizeString);
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
	myErrorMessage = tr("Parsing '%1' section failed: ").arg(theViewString);
	myNode=mySceneNode.firstChildElement(theViewString);

	myResult = BackgroundSerializer::createObjectFromDom(
			mySceneNode.firstChildElement(theBackgroundString),
			&(theWorldPtr->theBackground));
	if (myResult.isEmpty() == false)
	{
		myErrorMessage = tr("Parsing '%1' section failed: %2")
						 .arg(theBackgroundString)
						 .arg(myResult);
		goto not_good;
	}

	//
	// Parse the predefined section
	//
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

        AbstractObjectPtr myAOPtr = AbstractObjectSerializer::createObjectFromDom(q, false, true);
        if (myAOPtr == nullptr)
		{
			myErrorMessage += tr("createObjectFromDom failed");
			goto not_good;
		}
        theWorldPtr->addObject(myAOPtr);

		if (q==myNode.lastChild())
			break;
	}

	//
	// parse the Goals section
	//
	do {
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
				myErrorMessage += tr("expected a <%1> section, got <%2>. ").arg(theGoalString).arg(q.nodeName());
				hasProblem = true;
				continue;
			}

			Goal* myGPtr = GoalSerializer::createObjectFromDom(q);
			if (myGPtr == nullptr)
			{
				myErrorMessage += tr("createObjectFromDom failed");
				hasProblem = true;
				continue;
			}
			if (myGPtr->parseProperties(theWorldPtr)==false)
			{
				myErrorMessage += tr("<%1> properties could not be parsed. ").arg(theGoalString);
				hasProblem = true;
				continue;
			}

			theWorldPtr->addGoal(myGPtr);

			if (q==myNode.lastChild())
				break;
		}
	} while (false);  // i.e. always run this loop only once
	if (hasProblem==true)
	{
		return "W " + myErrorMessage;
	}

	//
	// parse the Hints section
	//
	do {
		myErrorMessage = tr("Parsing '%1' section failed: ").arg(theHintsString);
		mySceneNode=myDocElem.firstChildElement(theHintsString);
		if (mySceneNode.nodeName()!= theHintsString)
		{
			// hints are not mandatory
			break; // exit the do-while loop
		}
		for (q=mySceneNode.firstChild(); !q.isNull(); q=q.nextSibling())
		{
			// a hints section has the following layout:
			//	    <hints>
			//         <hint number="1" object="Ramp \" X="1.100" Y="1.200"/>
			//         <hint number="2" object="Quarter Arc" X="0.600" Y="0.400" angle="-3.140" />
			//		</hints>
			//
			// Of the arguments, 'number' and 'object' are mandatory
			// * object must be a name that is present in the Toolbox section
			// * numbers start at 1 and increment monotonously - 1,2,4 is not allowed.
			// * everything else is optional and depends on what is customized
			// * (not all objects allow rotation or resizing, we should check for that!)

			// simple sanity checks
			if (q.nodeName() == "#comment")
				continue;
			if (q.nodeName() != theHintString)
			{
				myErrorMessage += tr("expected a <%1> section, got <%2>. ").arg(theHintString).arg(q.nodeName());
				hasProblem = true;
				continue;
			}

			Hint* myHPtr = HintSerializer::createObjectFromDom(q);
			if (myHPtr == nullptr)
			{
				myErrorMessage += tr("createObjectFromDom failed");
				hasProblem = true;
				continue;
			}

			// add hint to level
			addHint(myHPtr);

			if (q==myNode.lastChild())
				break;
		}
	} while (false);  // i.e. always run this loop only once
	if (hasProblem==true)
	{
		return "W " + myErrorMessage;
	}

	// and everything went OK - we're done :-)
	aLevelInfoToolbox->setLevelPtr(this);
	return "";

	// if goto not_good was called, we get here, too
	// theErrorMessage is set - let's return it!
not_good:
	return "E " + myErrorMessage;
}


void
Level::addAbstractObject(QDomElement aParent, const AbstractObject& anObjectRef) const
{
    const AbstractObjectSerializer* myBOS = anObjectRef.getSerializer();
    if (myBOS!=nullptr)
    {
        myBOS->serialize(&aParent);
        delete myBOS;
	}
}

void Level::addHint(Hint *aHintPtr)
{
	Q_ASSERT(aHintPtr);
	theHintPtrList.push_back(aHintPtr);
}


void
Level::addTextElement(QDomElement aParent, const QString& anElementName, const QString& aText) const
{
    QDomElement myReturn = aParent.ownerDocument().createElement(anElementName);
    QDomText myT = aParent.ownerDocument().createTextNode(aText);
    myReturn.appendChild(myT);
    aParent.appendChild(myReturn);
}

void
Level::addTextElement(QDomElement aParent, const QString& anElementName, const LocalString& anLS) const
{
    LocalString::LocalStringList::const_iterator myL = anLS.theStringList.begin();
    while (myL != anLS.theStringList.end())
    {
        QDomElement myElement = aParent.ownerDocument().createElement(anElementName);
        QDomText myT = aParent.ownerDocument().createTextNode(myL.value());
        myElement.appendChild(myT);
        if (myL.key().isEmpty() == false)
            myElement.setAttribute("lang", myL.key());
        aParent.appendChild(myElement);
        ++myL;
    }
}


bool Level::save(const QString& aFileName)
{
	DEBUG5("Level::save(%s)", ASCII(aFileName));
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


    // Toolbox
    QDomElement myToolboxDomNode = myDocument.createElement(theToolboxString);
    myRoot.appendChild(myToolboxDomNode);
    // ... and add the various groups
    for (auto myI : theToolboxList)
    {
        myToolboxDomNode.appendChild(ToolboxGroupSerializer::serialize(myDocument, myI));
    }

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
    World::AbstractObjectPtrList::iterator myI = theWorldPtr->theObjectPtrList.begin();
    for (; myI != theWorldPtr->theObjectPtrList.end(); ++myI)
        addAbstractObject(myPredefinedParent, *(*myI));
    // ... add background
    BackgroundSerializer::serialize(&mySceneParent, &(theWorldPtr->theBackground));
    // ... TODO: add view

    // Goals
    QDomElement myGoalsParent = myDocument.createElement(theGoalsString);
    myRoot.appendChild(myGoalsParent);
    World::GoalPtrList::iterator myG = theWorldPtr->theGoalPtrList.begin();
    for (; myG != theWorldPtr->theGoalPtrList.end(); ++myG)
        GoalSerializer::serialize(*myG, myGoalsParent);

    // success: we're going to write!
    QFile myFile(aFileName);
    if (!myFile.open(QFile::WriteOnly | QFile::Text))
    {
        QString myError = tr("Cannot write file '%1': %2.")
            .arg(aFileName, myFile.errorString());
		DEBUG1("ERROR: %s", ASCII(myError));
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
