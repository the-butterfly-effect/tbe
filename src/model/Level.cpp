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

#include <QDomDocument>
#include <QFile>

// TODO: remove this
#include <iostream>

// Strings identifying elements/nodes in the XML file
//
static const char* theLevelInfoString = "levelinfo";
	static const char* theLevelNameString    = "title";
	static const char* theLevelAuthorString  = "author";
	static const char* theLevelLicenseString = "license";
static const char* theSceneString = "scene";
	static const char* theViewString       = "view";
	static const char* thePredefinedString = "predefined";
	static const char* theObjectString     = "object";
	static const char* thePropertyString   = "license";
static const char* theToolboxString = "toolbox";


// Constructors/Destructors
//  

Level::Level ( ) 
{
    theWorldPtr = new World(getName());

#if 0
    BaseObject* myBOPtr;
    
    myBOPtr = ObjectFactory::createObject("Ramp", Position(0.75, 0.5, 0), 1.5);
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Ball", Position(0.3, 2.0, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3.23, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3.46, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);
#endif
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

bool
Level::load(const QString& aFileName)
{
	theErrorMessage = tr("Cannot read file '%1'").arg(aFileName);
	QDomDocument myDocument("mydocument");
	QFile myFile(aFileName);
	if (!myFile.open(QIODevice::ReadOnly))
		return false;

	theErrorMessage = tr("Cannot parse file - not valid XML?");
	if (!myDocument.setContent(&myFile))
		return false;
	myFile.close();

	QDomElement docElem = myDocument.documentElement();

	//
	// parse the Level Info section
	//
	theErrorMessage = tr("Parsing '%1' section failed").arg(theLevelInfoString);
	QDomNode myLevelInfoNode=docElem.firstChildElement(theLevelInfoString);
	if (myLevelInfoNode.isNull())
		return false;
	theLevelName   = myLevelInfoNode.firstChildElement(theLevelNameString).text();
	theLevelAuthor = myLevelInfoNode.firstChildElement(theLevelAuthorString).text();;
	theLevelLicense= myLevelInfoNode.firstChildElement(theLevelLicenseString).text();;

	if (theLevelName.isEmpty() || theLevelAuthor.isEmpty() || theLevelLicense.isEmpty())
		return false;
	DEBUG5("level name:    '%s'\n", theLevelName.toAscii().constData());
	DEBUG5("level author:  '%s'\n", theLevelAuthor.toAscii().constData());
	DEBUG5("level license: '%s'\n", theLevelLicense.toAscii().constData());
	
	//
	// parse the Scene section
	//
	theErrorMessage = tr("Parsing '%1' section failed").arg(theSceneString);
	QDomNode mySceneNode=docElem.firstChildElement(theSceneString);

	// TODO: implement view
	theErrorMessage = tr("Parsing '%1' section failed").arg(theViewString);
	QDomNode myViewNode=mySceneNode.firstChildElement(theViewString);
	
	
	theErrorMessage = tr("Parsing '%1' section failed").arg(thePredefinedString);
	QDomNode myPredefinedNode = mySceneNode.firstChildElement(thePredefinedString);
	QDomNode q;
	for (q=myPredefinedNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// an object entry has the following layout:
		// <object type="Ramp" X="1.0" Y="0.5" width="2.0"/>
		// of these, type, X and Y are *MANDATORY*
		// optional are:  angle, width, height
		// TODO: also optional are properties
	
		if (q.nodeName() != theObjectString)
			return false;

		// the nodemap contains all the parameters, or not...
		QDomNamedNodeMap myNodes = q.attributes();
	    
	    bool isOK1, isOK2;

	    BaseObject* myBOPtr;
	    myBOPtr = ObjectFactory::createObject(
	    	myNodes.namedItem("type").nodeValue(), 
	    	Position( myNodes.namedItem("X").nodeValue().toDouble(&isOK1),
	    			  myNodes.namedItem("Y").nodeValue().toDouble(&isOK2),
	    			  myNodes.namedItem("angle").nodeValue().toDouble()));
	    if (!isOK1 || !isOK2)
	    	return false;
	    QString myValue = myNodes.namedItem("width").nodeValue();
	    if (myValue.isEmpty()==false)
	    	myBOPtr->setTheWidth(myValue.toDouble(&isOK1));
	    myValue = myNodes.namedItem("height").nodeValue();
	    if (myValue.isEmpty()==false)
	    	myBOPtr->setTheHeight(myValue.toDouble(&isOK1));
	    if (!isOK1 || !isOK2)
	    	return false;
	    theWorldPtr->addObject(myBOPtr);

	    // TODO: add Property handling here
	    
		if (q==myPredefinedNode.lastChild())
			break;
	}
	
	//
	// parse the Toolbox section
	// 
	// TODO: not implemented for Milestone 2.
	theErrorMessage = tr("Parsing '%1' section failed").arg(theToolboxString);

	// and everything went OK - we're done :-)
	theErrorMessage = "";
	return true;
}
