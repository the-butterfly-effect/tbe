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
	static const char* theSceneSizeString  = "scenesize";
	static const char* theViewString       = "view";
	static const char* thePredefinedString = "predefined";
	static const char* theObjectString     = "object";
	static const char* thePropertyString   = "license";
static const char* theToolboxString = "toolbox";


static const char* theWidthAttributeString     = "width";
static const char* theHeightAttributeString    = "height";
static const char* theXAttributeString         = "X";
static const char* theYAttributeString         = "Y";
static const char* theAngleAttributeString     = "angle";
static const char* theTypeAttributeString      = "type";


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

QString
Level::load(const QString& aFileName)
{
	QString myErrorMessage = tr("Cannot read file '%1'").arg(aFileName);
	QDomDocument myDocument("mydocument");
	
	QDomNode myNode, q;
	QDomNode mySceneNode;
	QDomElement myElement;
	QDomElement myDocElem;
	QDomNamedNodeMap myNodeMap;
    bool isOK1, isOK2;

    dReal myWidth;
    dReal myHeight;
    
	QFile myFile(aFileName);
	if (!myFile.open(QIODevice::ReadOnly))
		goto not_good;

	myErrorMessage = tr("Cannot parse file - not valid XML?");
	if (!myDocument.setContent(&myFile))
		goto not_good;
	myFile.close();

	myDocElem = myDocument.documentElement();

	//
	// parse the Level Info section
	//
	myErrorMessage = tr("Parsing '%1' section failed").arg(theLevelInfoString);
	myNode=myDocElem.firstChildElement(theLevelInfoString);
	if (myNode.isNull())
		goto not_good;
	theLevelName   = myNode.firstChildElement(theLevelNameString).text();
	theLevelAuthor = myNode.firstChildElement(theLevelAuthorString).text();;
	theLevelLicense= myNode.firstChildElement(theLevelLicenseString).text();;

	if (theLevelName.isEmpty() || theLevelAuthor.isEmpty() || theLevelLicense.isEmpty())
		goto not_good;
	DEBUG5("level name:    '%s'\n", theLevelName.toAscii().constData());
	DEBUG5("level author:  '%s'\n", theLevelAuthor.toAscii().constData());
	DEBUG5("level license: '%s'\n", theLevelLicense.toAscii().constData());
	theWorldPtr->setLevelName(theLevelName);
	
	//
	// parse the Scene section
	//
	myErrorMessage = tr("Parsing '%1' section failed").arg(theSceneString);
	mySceneNode=myDocElem.firstChildElement(theSceneString);

	myNode=mySceneNode.firstChildElement(theSceneSizeString);
	myNodeMap = myNode.attributes();
	myWidth = myNodeMap.namedItem(theWidthAttributeString).nodeValue().toDouble(&isOK1);
	myHeight= myNodeMap.namedItem(theHeightAttributeString).nodeValue().toDouble(&isOK2);
    if (!isOK1 || !isOK2)
    	goto not_good;
	theWorldPtr->setTheWorldSize(myWidth, myHeight);

	// TODO: implement view
	myErrorMessage = tr("Parsing '%1' section failed").arg(theViewString);
	myNode=mySceneNode.firstChildElement(theViewString);
	
	
	myErrorMessage = tr("Parsing '%1' section failed").arg(thePredefinedString);
	myNode = mySceneNode.firstChildElement(thePredefinedString);
	for (q=myNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// an object entry has the following layout:
		// <object type="Ramp" X="1.0" Y="0.5" width="2.0"/>
		// of these, type, X and Y are *MANDATORY*
		// optional are:  angle, width, height
		// TODO: also optional are properties
	
		// simple sanity check
		if (q.nodeName() != theObjectString)
			goto not_good;

		// the nodemap contains all the parameters, or not...
		myNodeMap = q.attributes();
	    
	    BaseObject* myBOPtr;
	    myBOPtr = ObjectFactory::createObject(
	    	myNodeMap.namedItem(theTypeAttributeString).nodeValue(), 
	    	Position( myNodeMap.namedItem(theXAttributeString).nodeValue().toDouble(&isOK1),
	    			  myNodeMap.namedItem(theYAttributeString).nodeValue().toDouble(&isOK2),
	    			  myNodeMap.namedItem(theAngleAttributeString).nodeValue().toDouble()));
	    if (!isOK1 || !isOK2)
	    	goto not_good;
	    QString myValue = myNodeMap.namedItem(theWidthAttributeString).nodeValue();
	    if (myValue.isEmpty()==false)
	    	myBOPtr->setTheWidth(myValue.toDouble(&isOK1));
	    myValue = myNodeMap.namedItem(theHeightAttributeString).nodeValue();
	    if (myValue.isEmpty()==false)
	    	myBOPtr->setTheHeight(myValue.toDouble(&isOK1));
	    if (!isOK1 || !isOK2)
	    	goto not_good;
	    theWorldPtr->addObject(myBOPtr);

	    // TODO: add Property handling here
	    
		if (q==myNode.lastChild())
			break;
	}
	
	//
	// parse the Toolbox section
	// 
	myErrorMessage = tr("Parsing '%1' section failed").arg(theToolboxString);
	// TODO: not implemented for Milestone 2.

	
	
	// and everything went OK - we're done :-)
	myErrorMessage = "";

	// if goto not_good was called, we get here, too
	// theErrorMessage is set - let's return it!
not_good:
	return myErrorMessage;
}
