/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#include "ToolboxGroupSerializer.h"
#include "AbstractObjectSerializer.h"
#include <QtXml/QDomElement>
#include <QtCore/QStringList>

#include "tbe_global.h"
#include <cstdio>

// these are declared in Level.cpp
extern const char* theToolboxGroupString;
extern const char* theObjectString;

static const char* theToolboxItemString = "toolboxitem";
static const char* theNameString = "name";
static const char* theCountString = "count";

ToolboxGroup*
ToolboxGroupSerializer::createObjectFromDom(const QDomNode& aBaseDomNode,
											QString *anOutputErrorMsg)
{
	bool             isOK = false;
	int              myCount = 0;
	QString          myErrorMessage;
	QDomNamedNodeMap myNodeMap = aBaseDomNode.attributes();
	ToolboxGroup*    myTbGPtr = NULL;
	LocalString      myToolBoxGroupName;

	// a toolbox object entry has the following layout:
	// <toolboxitem count="1" name="Ramp \" icon="RightRamp">
	//      <name lang="nl">Helling \</name>
	//      <object width="2" height="1" type="RightRamp" />
	// </toolboxitem>

	// note that name is optional, if no name specified, we will take the
	// name from the object - which *might* be localized (maybe not)...

	// simple sanity checks
	if (aBaseDomNode.nodeName() != theToolboxItemString)
	{
		myErrorMessage = QString("parse error: expected <%1> but got <%2>\n")
			   .arg(theToolboxItemString).arg(aBaseDomNode.nodeName());
		goto not_good;
	}

	myToolBoxGroupName.fillFromDOM(aBaseDomNode, theNameString,
						   myNodeMap.namedItem(theNameString).nodeValue());
	myTbGPtr = new ToolboxGroup(myToolBoxGroupName);
	myCount = myNodeMap.namedItem(theCountString).nodeValue().toInt(&isOK);
	if (myCount == 0 || isOK == false)
		myCount = 1;

	for (int i=0; i< myCount; i++)
	{
		QDomNode myObjectTag = aBaseDomNode.firstChildElement(theObjectString);
		if (myObjectTag.isNull())
		{
			myErrorMessage = QString("parse error: no <%1> found\n")
				   .arg(theObjectString);
			goto not_good;
		}
        AbstractObjectPtr myAOPtr = AbstractObjectSerializer::createObjectFromDom(myObjectTag,
																				true, false);
		if (myAOPtr == NULL)
		{
			myErrorMessage = "createObjectFromDom failed";
			goto not_good;
		}
		myTbGPtr->addObject(myAOPtr);
	}

	return myTbGPtr;

not_good:
	*anOutputErrorMsg = myErrorMessage;
	delete myTbGPtr;
	return NULL;
}


QDomElement ToolboxGroupSerializer::serialize(QDomDocument& aDomDocument, ToolboxGroup* aToolboxGroupPtr)
{
    QDomElement myToolboxNode = aDomDocument.createElement(theToolboxItemString);
    myToolboxNode.setAttribute(theCountString, aToolboxGroupPtr->count());
    myToolboxNode.setAttribute(theNameString, aToolboxGroupPtr->theGroupName.english());

    aToolboxGroupPtr->theGroupName.serializeTo(myToolboxNode);
    {
        const AbstractObjectSerializer* myAOSerializerPtr = aToolboxGroupPtr->first()->getSerializer();
        myAOSerializerPtr->serialize(&myToolboxNode);
        delete myAOSerializerPtr;
    }

    return myToolboxNode;
}
