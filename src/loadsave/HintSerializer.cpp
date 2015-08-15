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
extern const char* theHintString;
extern const char* theTypeAttributeString;
extern const char* thePropertyString;
extern const char* theKeyAttributeString;
extern const char* theIsFailAttributeString;

Hint*
HintSerializer::createObjectFromDom(const QDomNode& q)
{
	QDomNamedNodeMap myNodeMap;
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != theHintString)
	{
		DEBUG2("createHintFromDom: expected <%s> but got <%s>", ASCII(theHintString), ASCII(q.nodeName()));
		return NULL;
	}

	// the nodemap contains all the parameters, or not...
	myNodeMap = q.attributes();

	QString myObjectType = myNodeMap.namedItem(theTypeAttributeString).nodeValue();

	Hint* myGPtr = new Hint();

	if (myGPtr==NULL)
	{
		DEBUG2("createHintFromDom: '%s' has problems in its factory", ASCII(myObjectType));
		goto not_good;
	}

//	if (myNodeMap.namedItem(theIsFailAttributeString).nodeValue() == "true")
//	{
//		myGPtr->isFail = true;
//	}

	if (q.hasChildNodes()==true)
	{
		// to parse:   <property key="lessthan">0.3</property>
		QDomElement i;
		for (i=q.firstChildElement(); !i.isNull(); i=i.nextSiblingElement())
		{
			if (i.nodeName() != thePropertyString)
				goto not_good;
			QString myKey = i.attributes().item(0).nodeValue();
			QString myValue = i.text();
			DEBUG5("   %s", ASCII(QString("property: '%1'='%2'").arg(myKey).arg(myValue)));
//			myGPtr->theProps.setProperty(myKey, myValue);
		}
	}

	DEBUG4("createHintFromDom for '%s' successful\n", ASCII(myObjectType));
	return myGPtr;
not_good:
	delete myGPtr;
	return NULL;
}
