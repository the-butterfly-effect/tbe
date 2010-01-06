/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

Goal::Goal()
{
	// nothing to do here
}

Goal::~Goal()
{
	// nothing to do here
}

bool Goal::propertyToFloat(const QString& aPropertyName,
										 float* aFloat)
{
	QString myValue = getProperty(aPropertyName);
	if (myValue.isEmpty())
		return false;

	bool isOK = false;
	float myFloat = myValue.toFloat(&isOK);
	if (isOK == false)
		return false;

	*aFloat = myFloat;
	return true;
}

bool Goal::propertyToObjectPtr(
		World* aWPtr,
		const QString& aPropertyName,
		BaseObject** aBOPtrPtr)
{
	QString myValue = getProperty(aPropertyName);
	if (myValue.isEmpty())
		return false;
	*aBOPtrPtr = aWPtr->findObjectByID(myValue);
	if (*aBOPtrPtr == NULL)
		return false;
	return true;
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

	Position myDistanceVector = theFirstPtr->getTempCenter() - theSecondPtr->getTempCenter();
	switch(theType)
	{
	case NOTYPE:
		return false;
		break;
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
	if (theProperties.count() != 3)
	{
		DEBUG2("wrong number of properties at beginning of parseProperties - not good\n");
		return false;
	}


	if (propertyToFloat(Property::S_LESSTHAN, &theLimit))
		theType=LESSTHAN;
	if (propertyToFloat(Property::S_MORETHAN, &theLimit))
		theType=MORETHAN;
	if (theType == NOTYPE)
	{
		DEBUG2("no valid property found\n");
		return false;
	}


	// parse object1 and object2
	if (propertyToObjectPtr(aWPtr, Property::OBJECT1_STRING, &theFirstPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT1_STRING);
		return false;
	}
	if (propertyToObjectPtr(aWPtr, Property::OBJECT2_STRING, &theSecondPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT2_STRING);
		return false;
	}

	return true;
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
		break;
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
		if (fabs(myNewPos.x - myOldPos.x) > MINCHANGE)
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
		if (fabs(myNewPos.x - myOldPos.x) > MINCHANGE)
			return true;
		break;
	case ANYTHINGCHANGED:
		if (myNewPos == myOldPos)
			return false;
		else
			return true;
		break;
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
	if (theProperties.count() != 2)
	{
		DEBUG2("wrong number of properties at beginning of parseProperties - not good\n");
		return false;
	}


	// parse *changed - no value, only a key
	if (getProperty(Property::S_XCHANGED).isEmpty()==false)
		theType=XCHANGED;
	if (propertyToFloat(Property::S_XBELOW, &theLimit))
		theType=XBELOW;
	if (propertyToFloat(Property::S_XOVER, &theLimit))
		theType=XOVER;
	if (getProperty(Property::S_YCHANGED).isEmpty()==false)
		theType=YCHANGED;
	if (propertyToFloat(Property::S_YBELOW, &theLimit))
		theType=YBELOW;
	if (propertyToFloat(Property::S_YOVER, &theLimit))
		theType=YOVER;
	if (getProperty(Property::S_ACHANGED).isEmpty()==false)
		theType=ANGLECHANGED;
	if (getProperty(Property::S_ANYTHING).isEmpty()==false)
		theType=ANYTHINGCHANGED;
	if (theType == NOTYPE)
	{
		DEBUG2("no valid property found\n");
		return false;
	}

	// parse object
	if (propertyToObjectPtr(aWPtr, Property::OBJECT_STRING, &theBOPtr)==false)
	{
		DEBUG2("%s is not an existing, valid object\n", Property::OBJECT_STRING);
		return false;
	}
	return true;
}

