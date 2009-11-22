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
#include "World.h"

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

	// we expect only 4 types of properties: ("lessthan" OR "morethan") AND "object1" AND "object2"
	// but we will only have 3 properties - any other number is wrong
	if (theProperties.count() != 0)
	{
		DEBUG2("wrong number of properties at beginning of parseProperties - not good\n");
		return false;
	}


	// parse lessthan or morethan
	const char* S_LESSTHAN = "lessthan";
	const char* S_MORETHAN = "morethan";
	QString myValue = getProperty(S_LESSTHAN);
	if (myValue.isEmpty()==false)
		theType=LESSTHAN;
	else
	{
		myValue = getProperty(S_MORETHAN);
		if (myValue.isEmpty())
		{
			DEBUG2("no valid property %s or %s found\n", S_MORETHAN, S_LESSTHAN);
			return false;
		}
	}
	bool isOK;
	theLimit=myValue.toDouble(&isOK);
	if (isOK==false)
	{
		DEBUG2("value '%s' to property %s/%s is invalid\n", ASCII(myValue), S_MORETHAN, S_LESSTHAN);
		return false;
	}

	// parse object1 and object2
	myValue = getProperty("object1");
	theFirstPtr = aWPtr->findObjectByID(myValue);
	if (theFirstPtr==NULL)
	{
		DEBUG2("object1 is not an existing, valid object\n");
		return false;
	}

	myValue = getProperty("object2");
	theSecondPtr = aWPtr->findObjectByID(myValue);
	if (theSecondPtr==NULL)
	{
		DEBUG2("object2 is not an existing, valid object\n");
		return false;
	}

	return true;
}
