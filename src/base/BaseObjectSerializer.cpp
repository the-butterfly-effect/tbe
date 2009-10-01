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

#include "BaseObjectSerializer.h"
#include <QDomElement>


// these are all declared in Level.cpp
extern const char* theObjectString;
extern const char* theWidthAttributeString;
extern const char* theHeightAttributeString;
extern const char* theXAttributeString;
extern const char* theYAttributeString;
extern const char* theAngleAttributeString;
extern const char* theTypeAttributeString;
extern const char* thePropertyString;


BaseObjectSerializer::BaseObjectSerializer(const BaseObject* anObjectPtr)
		: theBaseObjectPtr(anObjectPtr)
{
}


void
BaseObjectSerializer::serialize(QDomElement* aParent) const
{
	QDomElement myNode = aParent->ownerDocument().createElement(theObjectString);
	myNode.setAttribute(theTypeAttributeString, theBaseObjectPtr->getName());
	myNode.setAttribute(theXAttributeString, theBaseObjectPtr->getOrigCenter().x);
	myNode.setAttribute(theYAttributeString, theBaseObjectPtr->getOrigCenter().y);
	if (theBaseObjectPtr->isRotatable())
		myNode.setAttribute(theAngleAttributeString, theBaseObjectPtr->getOrigCenter().angle);
	if (theBaseObjectPtr->isResizable() & BaseObject::HORIZONTALRESIZE)
		myNode.setAttribute(theWidthAttributeString, theBaseObjectPtr->getTheWidth());
	if (theBaseObjectPtr->isResizable() & BaseObject::VERTICALRESIZE)
		myNode.setAttribute(theHeightAttributeString, theBaseObjectPtr->getTheHeight());

	if (theBaseObjectPtr->theProperties.isEmpty()==false)
	{
		BaseObject::PropertyMap::const_iterator i;
		for ( i = theBaseObjectPtr->theProperties.constBegin();
			  i!= theBaseObjectPtr->theProperties.constEnd();
			 ++i)
		{
			QDomElement myProperty = aParent->ownerDocument().createElement(thePropertyString);
			myProperty.setAttribute("key", i.key());
			QDomText myT = aParent->ownerDocument().createTextNode(i.value());
			myProperty.appendChild(myT);
			myNode.appendChild(myProperty);
		}
	}

	aParent->appendChild(myNode);
}


BaseObject*
BaseObjectSerializer::createObjectFromDom(const QDomNode& q, bool isXYMandatory)
{
	QDomNamedNodeMap myNodeMap;
	bool isOK1, isOK2;
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != "object")
	{
		DEBUG2("createObjectFromDom: expected <object> but got <%s>\n", ASCII(q.nodeName()));
		return NULL;
	}

	// the nodemap contains all the parameters, or not...
	myNodeMap = q.attributes();

	QString myObjectType = myNodeMap.namedItem(theTypeAttributeString).nodeValue();
	Position myObjectPosition(
			myNodeMap.namedItem(theXAttributeString).nodeValue().toDouble(&isOK1),
			myNodeMap.namedItem(theYAttributeString).nodeValue().toDouble(&isOK2),
			myNodeMap.namedItem(theAngleAttributeString).nodeValue().toDouble());
	if (!isOK1 || !isOK2)
	{
		if (isXYMandatory)
		{
			DEBUG2("createObjectFromDom: '%s' has invalid X or Y\n", ASCII(myObjectType));
			return NULL;
		}
		myObjectPosition = Position(0,0,0);
	}

	BaseObject* myBOPtr = ObjectFactory::createObject( myObjectType, myObjectPosition);
	if (myBOPtr==NULL)
	{
		DEBUG2("createObjectFromDom: '%s' has problems in its factory\n", ASCII(myObjectType));
		goto not_good;
	}

	isOK1=true;
	myValue = myNodeMap.namedItem(theWidthAttributeString).nodeValue();
	if (myValue.isEmpty()==false)
		myBOPtr->setTheWidth(myValue.toDouble(&isOK1));
	if (!isOK1)
	{
		DEBUG2("createObjectFromDom: '%s' has invalid %s\n", ASCII(myObjectType), theWidthAttributeString);
		goto not_good;
	}

	isOK1=true;
	myValue = myNodeMap.namedItem(theHeightAttributeString).nodeValue();
	if (myValue.isEmpty()==false)
		myBOPtr->setTheHeight(myValue.toDouble(&isOK1));
	if (!isOK1)
	{
		DEBUG2("createObjectFromDom: '%s' has invalid %s\n", ASCII(myObjectType), theHeightAttributeString);
		goto not_good;
	}
	if (q.hasChildNodes()==true)
	{
		// to parse:   <property key="texture">used_wood_bar</property>
		QDomElement i;
		for (i=q.firstChildElement(); !i.isNull(); i=i.nextSiblingElement())
		{
			if (i.nodeName() != thePropertyString)
				goto not_good;
			QString myKey = i.attributes().item(0).nodeValue();
			QString myValue = i.text();
			DEBUG5("   property: '%s'='%s'\n",
				   myKey.toAscii().constData(),
				   myValue.toAscii().constData());
			myBOPtr->setProperty(myKey, myValue);
		}
	}

	DEBUG4("createObjectFromDom for '%s' successful\n", ASCII(myObjectType));
	return myBOPtr;
not_good:
	delete myBOPtr;
	return NULL;
}
