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

#include "tbe_global.h"
#include "GoalSerializer.h"
#include <QDomElement>


// these are all declared in Level.cpp
extern const char* theGoalString;
extern const char* theTypeAttributeString;
extern const char* thePropertyString;
extern const char* theKeyAttributeString;



Goal*
GoalSerializer::createObjectFromDom(const QDomNode& q)
{
	QDomNamedNodeMap myNodeMap;
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != theGoalString)
	{
		DEBUG2("createObjectFromDom: expected <%s> but got <%s>\n", ASCII(theGoalString), ASCII(q.nodeName()));
		return NULL;
	}

	// the nodemap contains all the parameters, or not...
	myNodeMap = q.attributes();

	QString myObjectType = myNodeMap.namedItem(theTypeAttributeString).nodeValue();

	Goal* myGPtr = NULL;
	if (myObjectType=="distance")
		myGPtr = new GoalDistance();

	if (myGPtr==NULL)
	{
		DEBUG2("createObjectFromDom: '%s' has problems in its factory\n", ASCII(myObjectType));
		goto not_good;
	}

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
			DEBUG5("   property: '%s'='%s'\n",
				   ASCII(myKey),
				   ASCII(myValue));
			myGPtr->setProperty(myKey, myValue);
		}
	}

	DEBUG4("createObjectFromDom for '%s' successful\n", ASCII(myObjectType));
	return myGPtr;
not_good:
	delete myGPtr;
	return NULL;
}
