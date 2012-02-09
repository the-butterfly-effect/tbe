/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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
#include "Goal.h"
#include "GoalEditor.h"
#include "GoalSerializer.h"
#include "World.h"
#include <QtXml/QDomElement>
#include <QtCore/QStringList>
#include <cassert>


// these are all declared in Level.cpp
extern const char* theGoalString;
extern const char* theTypeAttributeString;
extern const char* thePropertyString;
extern const char* theKeyAttributeString;
extern const char* theIsFailAttributeString;

Goal*
GoalSerializer::createObjectFromDom(const QDomNode& q)
{
	QDomNamedNodeMap myNodeMap;
	QString myValue;

	/// simple sanity check first...
	if (q.nodeName() != theGoalString)
	{
		DEBUG2("createGoalFromDom: expected <%s> but got <%s>\n", ASCII(theGoalString), ASCII(q.nodeName()));
		return NULL;
	}

	// the nodemap contains all the parameters, or not...
	myNodeMap = q.attributes();

	QString myObjectType = myNodeMap.namedItem(theTypeAttributeString).nodeValue();

	Goal* myGPtr = NULL;
	if (myObjectType=="distance")
		myGPtr = new GoalDistance();
	if (myObjectType=="positionchange")
		myGPtr = new GoalPositionChange();
	if (myObjectType=="statechange")
		myGPtr = new GoalStateChange();

	if (myGPtr==NULL)
	{
		DEBUG2("createGoalFromDom: '%s' has problems in its factory\n", ASCII(myObjectType));
		goto not_good;
	}

	if (myNodeMap.namedItem(theIsFailAttributeString).nodeValue() == "true")
	{
		myGPtr->isFail = true;
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
			DEBUG5("   %s\n", ASCII(QString("property: '%1'='%2'").arg(myKey).arg(myValue)));
			myGPtr->theProps.setProperty(myKey, myValue);
		}
	}

	DEBUG4("createGoalFromDom for '%s' successful\n", ASCII(myObjectType));
	return myGPtr;
not_good:
	delete myGPtr;
	return NULL;
}

Goal* GoalSerializer::createObjectFromString(World* aWorldPtr, const QString& aString)
{
	Goal* myGoal = NULL;

	// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	QStringList myList = aString.split(";");

	if (myList.size()<3)
		return NULL;
	int myType = getColumnZero().indexOf(myList[0]);
	if (myType==-1)
	{
		DEBUG2("Problem: Goal type not recognized\n");
		return NULL;
	}

	switch (myType)
	{
	case DISTANCE:
		myGoal = new GoalDistance();
		if (myList.size()!=5)
			return NULL;
		if (myList[2]!=">" && myList[2]!="<")
			return NULL;
		if (myList[2]==">")
			myGoal->theProps.setProperty(Property::S_MORETHAN, myList[3]);
		if (myList[2]=="<")
			myGoal->theProps.setProperty(Property::S_LESSTHAN, myList[3]);
		myGoal->theProps.setProperty(Property::OBJECT1_STRING, myList[1]);
		myGoal->theProps.setProperty(Property::OBJECT2_STRING, myList[4]);
		break;
	case POSITIONX:
	case POSITIONY:
	case ANGLE:
	case ANYTHING:
		myGoal = new GoalPositionChange();
		myGoal->theProps.setProperty(Property::OBJECT_STRING, myList[1]);
		if (myType==POSITIONX && myList[2]==GoalEditor::getT10nOf_change())
			myGoal->theProps.setProperty(Property::S_XCHANGED, "");
		if (myType==POSITIONX && myList[2]==">")
			myGoal->theProps.setProperty(Property::S_XOVER, myList[3]);
		if (myType==POSITIONX && myList[2]=="<")
			myGoal->theProps.setProperty(Property::S_XBELOW, myList[3]);

		if (myType==POSITIONY && myList[2]==GoalEditor::getT10nOf_change())
			myGoal->theProps.setProperty(Property::S_YCHANGED, "");
		if (myType==POSITIONY && myList[2]==">")
			myGoal->theProps.setProperty(Property::S_YOVER, myList[3]);
		if (myType==POSITIONY && myList[2]=="<")
			myGoal->theProps.setProperty(Property::S_YBELOW, myList[3]);

		if (myType==ANGLE && myList[2]==GoalEditor::getT10nOf_change())
			myGoal->theProps.setProperty(Property::S_ACHANGED, "");
		if (myType==ANYTHING && myList[2]==GoalEditor::getT10nOf_change())
			myGoal->theProps.setProperty(Property::S_ANYTHING, "");
		break;
	case STATE:
		myGoal = new GoalStateChange();
		myGoal->theProps.setProperty(Property::OBJECT_STRING, myList[1]);
		if (myList[2]==">")
			myGoal->theProps.setProperty(Property::S_MORETHAN, myList[3]);
		if (myList[2]==GoalEditor::getT10nOf_change())
			myGoal->theProps.setProperty(Property::S_STATE_CH, "");
		break;
	}

	if (myGoal->parseProperties(aWorldPtr)==true)
		return myGoal;
	delete myGoal;
	return NULL;
}


QStringList GoalSerializer::getColumnZero(void)
{
	// this QStringList is populated in a strict order
	// don't change this - some code expects all POSITION entries (i.e. 0-3)
	// to come before DISTANCE (i.e. 4)
	QStringList myVariables;
	myVariables.insert(POSITIONX,   QObject::tr("Position X"));
	myVariables.insert(POSITIONY,   QObject::tr("Position Y"));
	myVariables.insert(ANGLE,       QObject::tr("Angle"));
	myVariables.insert(ANYTHING,    QObject::tr("X/Y/Angle"));
	myVariables.insert(DISTANCE,    QObject::tr("Distance"));
	myVariables.insert(STATE,       QObject::tr("Object State"));
	return myVariables;
}

QStringList GoalSerializer::goalToStringList(const Goal* aGoalPtr)
{
	return aGoalPtr->goalToStringList().split(";");
}


bool GoalSerializer::serialize(const Goal* aGoalPtr, QDomElement& aParent)
{
	QDomElement myNode = aParent.ownerDocument().createElement(theGoalString);

	// save Goal Type
	myNode.setAttribute(theTypeAttributeString, aGoalPtr->getGoalType());

	// save isFail
	myNode.setAttribute(theIsFailAttributeString, aGoalPtr->isFail?"true":"false");

	// save all properties (no defaults here)
	PropertyList::PropertyMap::const_iterator i = aGoalPtr->theProps.constBegin();
	while (i != aGoalPtr->theProps.constEnd())
	{
		QDomElement myProperty = aParent.ownerDocument().createElement(thePropertyString);
		myProperty.setAttribute("key", i.key());
		QDomText myT = aParent.ownerDocument().createTextNode(i.value());
		myProperty.appendChild(myT);
		myNode.appendChild(myProperty);
		++i;
	 }

	aParent.appendChild(myNode);
	return true;
}
