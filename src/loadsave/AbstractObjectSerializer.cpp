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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObjectSerializer.h"
#include <QtXml/QDomElement>

#include <cstdio>

// these are all declared in Level.cpp
extern const char* theObjectString;
extern const char* theWidthAttributeString;
extern const char* theHeightAttributeString;
extern const char* theXAttributeString;
extern const char* theYAttributeString;
extern const char* theAngleAttributeString;
extern const char* theTypeAttributeString;
extern const char* theIDAttributeString;
extern const char* thePropertyString;
extern const char* theToolTipString;

AbstractObjectSerializer::AbstractObjectSerializer(const AbstractObjectPtr anObjectPtr)
		: theAbstractObjectPtr(anObjectPtr)
{
}


QString AbstractObjectSerializer::floatToString(float aValue) const
{
	// HACK HACK - yes, I'm aware I'm using C-style tricks here
	// to work around the i18n issues that QT provides me with :-(
	char myString[256];
	snprintf(myString,200, "%-2.3f", aValue);
	return QString(myString);
}


void
AbstractObjectSerializer::serialize(QDomElement* aParent) const
{
	// do not serialize the object if it is a child.
	// e.g. a PivotPoint created by an object as one of its properties
    if (!theAbstractObjectPtr->theProps.getPropertyNoDefault(
            Property::ISCHILD_STRING).isEmpty())
		return;

	// save basic values that are part of the object
	// TODO: only save values that are different from default
	QDomElement myNode = aParent->ownerDocument().createElement(theObjectString);
	myNode.setAttribute(theTypeAttributeString, theAbstractObjectPtr->getInternalName());
	myNode.setAttribute(theXAttributeString, floatToString(theAbstractObjectPtr->getOrigCenter().x));
	myNode.setAttribute(theYAttributeString, floatToString(theAbstractObjectPtr->getOrigCenter().y));
	myNode.setAttribute(theAngleAttributeString, floatToString(theAbstractObjectPtr->getOrigCenter().angle));
	myNode.setAttribute(theWidthAttributeString, floatToString(theAbstractObjectPtr->getTheWidth()));
	myNode.setAttribute(theHeightAttributeString, floatToString(theAbstractObjectPtr->getTheHeight()));
	if (theAbstractObjectPtr->getID().isEmpty()==false)
		myNode.setAttribute(theIDAttributeString, theAbstractObjectPtr->getID());

	// save properties
	// only save properties that are non-default
	if (theAbstractObjectPtr->theProps.getPropertyCount() > 0)
	{
		PropertyList::PropertyMap::const_iterator i;
		for ( i = theAbstractObjectPtr->theProps.constBegin();
			  i!= theAbstractObjectPtr->theProps.constEnd();
			 ++i)
		{
			QString myDefValue = theAbstractObjectPtr->theProps.getDefaultProperty(i.key());
			// do not save default values
			if (myDefValue == i.value())
				continue;

			QDomElement myProperty = aParent->ownerDocument().createElement(thePropertyString);
			myProperty.setAttribute("key", i.key());
			QDomText myT = aParent->ownerDocument().createTextNode(i.value());
			myProperty.appendChild(myT);
			myNode.appendChild(myProperty);
		}
	}

	// Save *custom* tooltips only, not the default ones
	// TODO: Note: not saving other languages as that will soon be removed anyway
	if (theAbstractObjectPtr->hasCustomToolTip)
	{
		QString myTooltipTextString = theAbstractObjectPtr->theToolTip.english();
		QDomElement myTooltip = aParent->ownerDocument().createElement(theToolTipString);
		QDomText myTooltipText= aParent->ownerDocument().createTextNode(myTooltipTextString);
		myTooltip.appendChild(myTooltipText);
		myNode.appendChild(myTooltip);
	}

	aParent->appendChild(myNode);
}


AbstractObjectPtr AbstractObjectSerializer::createObjectFromDom(const QDomNode& q,
											  bool isMovable,
											  bool isXYMandatory)
{
	QDomNamedNodeMap myNodeMap;
	bool isOK1, isOK2;
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != "object")
	{
		DEBUG2("createObjectFromDom: expected <object> but got <%s>", ASCII(q.nodeName()));
		return nullptr;
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
			DEBUG2("createObjectFromDom: '%s' has invalid X or Y", ASCII(myObjectType));
			return nullptr;
		}
		myObjectPosition = Position(0,0,0);
	}

    AbstractObjectPtr myAOPtr = ObjectFactory::createObject(myObjectType, myObjectPosition);
    if (myAOPtr==nullptr)
	{
		DEBUG2("createObjectFromDom: '%s' has problems in its factory", ASCII(myObjectType));
		goto not_good;
	}

    myAOPtr->setID(myNodeMap.namedItem(theIDAttributeString).nodeValue());
    myAOPtr->theIsMovable = isMovable;

	isOK1=true;
	myValue = myNodeMap.namedItem(theWidthAttributeString).nodeValue();
	if (myValue.isEmpty()==false)
        myAOPtr->setTheWidth(myValue.toDouble(&isOK1));
	if (!isOK1)
	{
		DEBUG2("createObjectFromDom: '%s' has invalid %s", ASCII(myObjectType), theWidthAttributeString);
		goto not_good;
	}

	isOK1=true;
	myValue = myNodeMap.namedItem(theHeightAttributeString).nodeValue();
	if (myValue.isEmpty()==false)
        myAOPtr->setTheHeight(myValue.toDouble(&isOK1));
	if (!isOK1)
	{
		DEBUG2("createObjectFromDom: '%s' has invalid %s", ASCII(myObjectType), theHeightAttributeString);
		goto not_good;
	}
	if (q.hasChildNodes()==true)
	{
		// to parse:   <property key="texture">used_wood_bar</property>
		QDomElement i;
		for (i=q.firstChildElement(); !i.isNull(); i=i.nextSiblingElement())
		{
			QString myKey = i.attributes().item(0).nodeValue();
			QString myTValue = i.text();
			if (i.nodeName() == thePropertyString)
			{
				DEBUG5("   %s", ASCII(QString("property: '%1'='%2'").arg(myKey).arg(myTValue)));
				// make sure there's no 'Description' - that's now outlawed
				Q_ASSERT(myKey != "Description");
				myAOPtr->theProps.setProperty(myKey, myTValue);
				continue;
			}
			if (i.nodeName() == theToolTipString)
			{
				DEBUG5("   %s", ASCII(QString("tooltip: '%1'='%2'").arg(myKey).arg(myTValue)));
				myAOPtr->theToolTip.add(myTValue, myKey);
				myAOPtr->hasCustomToolTip = true;
				continue;
			}
			// if we get here, nodeName is not something we know about
			goto not_good;
		}
	}

	DEBUG4("createObjectFromDom for '%s' successful", ASCII(myObjectType));
    return myAOPtr;
not_good:
    // myAOPtr is a shared_ptr - will take care of its own destruction if needed
    return nullptr;
}
