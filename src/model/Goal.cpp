/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "BaseObject.h"
#include "Goal.h"
#include "Position.h"
#include "Property.h"
#include "World.h"
#include "GoalEditor.h"

#include <QStringList>

Goal::Goal()
{
	// nothing to do here
}

Goal::~Goal()
{
	// nothing to do here
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////// GoalDistance //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalDistance::GoalDistance()
		: theType(NOTYPE), theFirstPtr(NULL), theSecondPtr(NULL)
{
	// nothing to do here
}

GoalDistance::~GoalDistance()
{
	// nothing to do here
}

bool GoalDistance::checkForSuccess(void)
{
	// whine loudly in a debug build or softly in a release build
	assert(theFirstPtr != NULL);
	assert(theSecondPtr != NULL);
	if (theFirstPtr == NULL || theSecondPtr == NULL)
		return false;

	Vector myDistanceVector = (theFirstPtr->getTempCenter().toVector())
							  - (theSecondPtr->getTempCenter().toVector());
	switch(theType)
	{
	case NOTYPE:
		return false;
	case LESSTHAN:
		if (myDistanceVector.length() < theLimit)
			return true;
		break;
	case MORETHAN:
		if (myDistanceVector.length() > theLimit)
			return true;
		break;
	}
	return false;
}

bool GoalDistance::parseProperties(World* aWPtr)
{
	assert(aWPtr!=NULL);
	if (aWPtr==NULL)
		return false;

	// there are 4 types of properties: ("lessthan" OR "morethan") AND "object1" AND "object2"
	// but we expect only to have 3 properties - any other number is wrong
	if (theProps.getPropertyCount() != 3)
	{
		DEBUG2("wrong number of properties at beginning of parseProperties - not good\n");
		return false;
	}


	if (theProps.property2Float(Property::S_LESSTHAN, &theLimit,false))
		theType=LESSTHAN;
	if (theProps.property2Float(Property::S_MORETHAN, &theLimit,false))
		theType=MORETHAN;
	if (theType == NOTYPE)
	{
		DEBUG2("no valid property found\n");
		return false;
	}


	// parse object1 and object2
	if (theProps.property2ObjectPtr(aWPtr, Property::OBJECT1_STRING, &theFirstPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT1_STRING);
		return false;
	}
	if (theProps.property2ObjectPtr(aWPtr, Property::OBJECT2_STRING, &theSecondPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT2_STRING);
		return false;
	}

	return true;
}

QString GoalDistance::goalToStringList() const
{
	// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	QString myString = QString("%1;%2;%3;%4;%5")
					   .arg(GoalSerializer::getColumnZero()[GoalSerializer::DISTANCE])
					   .arg(theFirstPtr->getID())
					   .arg(theType==MORETHAN?">":"<")
					   .arg(QString::number(theLimit))
					   .arg(theSecondPtr->getID());
	return myString;
}



//////////////////////////////////////////////////////////////////////////////
////////////////////////////// GoalPositionChange ////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalPositionChange::GoalPositionChange()
		: theType(NOTYPE), theBOPtr(NULL)
{
	// nothing to do here
}

GoalPositionChange::~GoalPositionChange()
{
	// nothing to do here
}

bool GoalPositionChange::checkForSuccess(void)
{
	// whine loudly in a debug build or softly in a release build
	assert(theBOPtr != NULL);
	if (theBOPtr == NULL)
		return false;

	Position myNewPos = theBOPtr->getTempCenter();
	Position myOldPos = theBOPtr->getOrigCenter();
	const float MINCHANGE = 0.01;
	switch(theType)
	{
	case NOTYPE:
		return false;
	case XCHANGED:
		if (fabs(myNewPos.x - myOldPos.x) > MINCHANGE)
			return true;
		break;
	case XBELOW:
		if (myNewPos.x < theLimit)
			return true;
		break;
	case XOVER:
		if (myNewPos.x > theLimit)
			return true;
		break;
	case YCHANGED:
		if (fabs(myNewPos.y - myOldPos.y) > MINCHANGE)
			return true;
		break;
	case YBELOW:
		if (myNewPos.y < theLimit)
			return true;
		break;
	case YOVER:
		if (myNewPos.y > theLimit)
			return true;
		break;
	case ANGLECHANGED:
		if (fabs(myNewPos.angle - myOldPos.angle) > MINCHANGE)
			return true;
		break;
	case ANYTHINGCHANGED:
		if (myNewPos == myOldPos)
			return false;
		return true;
	}
	return false;
}

bool GoalPositionChange::parseProperties(World* aWPtr)
{
	assert(aWPtr!=NULL);
	if (aWPtr==NULL)
		return false;

	// there are 5 types of properties: (xchanged/ychanged/anglechanged/anythingchanged AND object)
	// but we expect only to have 2 properties - any other number is wrong
	if (theProps.getPropertyCount() != 2)
	{
		DEBUG2("wrong number of properties at beginning of parseProperties - not good\n");
		return false;
	}


	// parse *changed - no value, only a key
	if (theProps.doesPropertyExists(Property::S_XCHANGED))
		theType=XCHANGED;
	if (theProps.property2Float(Property::S_XBELOW, &theLimit,false))
		theType=XBELOW;
	if (theProps.property2Float(Property::S_XOVER, &theLimit,false))
		theType=XOVER;
	if (theProps.doesPropertyExists(Property::S_YCHANGED))
		theType=YCHANGED;
	if (theProps.property2Float(Property::S_YBELOW, &theLimit,false))
		theType=YBELOW;
	if (theProps.property2Float(Property::S_YOVER, &theLimit,false))
		theType=YOVER;
	if (theProps.doesPropertyExists(Property::S_ACHANGED))
		theType=ANGLECHANGED;
	if (theProps.doesPropertyExists(Property::S_ANYTHING))
		theType=ANYTHINGCHANGED;
	if (theType == NOTYPE)
	{
		DEBUG2("no valid property found\n");
		return false;
	}

	// parse object
	if (theProps.property2ObjectPtr(aWPtr, Property::OBJECT_STRING, &theBOPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT_STRING);
		return false;
	}
	return true;
}


QString GoalPositionChange::goalToStringList() const
{
	QString myVariable;
	QString myCondition;
	QString myLimit;

	if ((theType&0x03)==1)
		myCondition=QObject::tr("changed");
	if ((theType&0x03)==2)
		myCondition="<";
	if ((theType&0x03)==3)
		myCondition=">";

	if ((theType&0x03)==2 || (theType&0x03)==3)
		myLimit = QString::number(theLimit);

	// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is not present here)
	QString myString = QString("%1;%2;%3;%4;")
					   //: translators: %1 can be X, Y, Angle or X/Y/Angle
					   .arg(GoalSerializer::getColumnZero()[theType/4])
					   .arg(theBOPtr->getID())
					   .arg(myCondition)
					   .arg(myLimit);
	return myString;
}
