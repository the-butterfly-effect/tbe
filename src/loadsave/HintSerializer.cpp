/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#include "tbe_global.h"
#include "Hint.h"
#include "HintSerializer.h"
#include "Level.h"
#include "World.h"
#include <QtXml/QDomElement>
#include <QtCore/QStringList>


// these are all declared in Level.cpp
extern const char* theAngleAttributeString;
extern const char* theHintString;
extern const char* theIsFailAttributeString;
extern const char* theKeyAttributeString;
extern const char* theNumberString;
extern const char* theObjectString;
extern const char* thePropertyString;
extern const char* theTypeAttributeString;
extern const char* theXAttributeString;
extern const char* theYAttributeString;


Hint*
HintSerializer::createObjectFromDom(const QDomNode& q)
{
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != theHintString)
	{
		DEBUG2("createHintFromDom: expected <%s> but got <%s>", ASCII(theHintString), ASCII(q.nodeName()));
		return nullptr;
	}

	Hint* myHPtr = new Hint();

	// the nodemap contains all the attributes...
	QDomNamedNodeMap myNodeMap = q.attributes();

	for (int i=0; i< myNodeMap.length(); i++)
	{
		QString myAName  = myNodeMap.item(i).nodeName();
		QString myAValue = myNodeMap.item(i).nodeValue();
		DEBUG5("  hint attribute: name %s", ASCII(myAName));
		DEBUG5("  hint attribute: value %s", ASCII(myAValue));

		if (theNumberString == myAName)
		{
			myHPtr->theHintIndex=myAValue.toInt();
			continue;
		}
		if (theObjectString == myAName)
		{
			myHPtr->theObjectName=myAValue;
			continue;
		}
		myHPtr->theParams.setProperty(myAName, myAValue);
	}

	// sanity checks:
	if (myHPtr->theHintIndex==0 || myHPtr->theObjectName=="-")
	{
		DEBUG2("Hint parsing failed: mandatory field(s) missing");
		goto not_good;
	}
	if (myHPtr->theParams.getPropertyCount()==0)
	{
		DEBUG2("Hint parsing failed: no parameter fields, that's just wrong");
		goto not_good;
	}

	DEBUG4("createHintFromDom %d for '%s' successful", myHPtr->theHintIndex, ASCII(myHPtr->theObjectName));
	return myHPtr;
not_good:
	delete myHPtr;
	return nullptr;
}


bool HintSerializer::serialize(const Hint* aHintPtr, QDomElement& aParent)
{
	QDomElement myHintElement = aParent.ownerDocument().createElement(theHintString);

	// <hint X="0.7" Y="1.5" number="1" object="BowlingBall"/>

	// number and object are mandatory
	myHintElement.setAttribute(theNumberString, aHintPtr->theHintIndex);
	myHintElement.setAttribute(theObjectString, aHintPtr->theObjectName);

	// The rest are properties. We only save actually existing properties
	PropertyList::PropertyMap::const_iterator i;
	for ( i = aHintPtr->theParams.constBegin();
		  i!= aHintPtr->theParams.constEnd();
		 ++i)
	{
		myHintElement.setAttribute(i.key(), i.value());
	}

	aParent.appendChild(myHintElement);
	return true;
}
