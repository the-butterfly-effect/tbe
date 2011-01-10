/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "RatBread.h"
#include <QStringList>

// this class' ObjectFactory
class RatObjectFactory : public ObjectFactory
{
public:
	RatObjectFactory(void)
	{	announceObjectType("Rat", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Rat()); }
};
static RatObjectFactory theRatObjectFactory;


const static float theRatMass = 0.8;

/// this list contains all items that have an objectID that starts with "Food"
/// i.e. "Food1", "Food:Bread" but also "Foodology" would work.
static QStringList theAllFoodIDs;


Rat::Rat()
		: RectObject(QObject::tr("Rat"),
					 QObject::tr("a rat is always looking for food or sleeping."),
					 "Rat_Sleeping1R;Rat_Sleeping2R;Rat_Sniffing1R;Rat_Sniffing2R;Rat_Walking1R;Rat_Walking2R;Rat_Walking3R;Rat_Nomming1R;Rat_Noming2R",
					0.4,0.1, 0.8, 0.4)
					 , theCurrentState(SLEEPING1)
{
	theProps.setDefaultPropertiesString(
			Property::OBJECT_STRING + QString(":/") +
			Property::BOUNCINESS_STRING + QString(":0/") +
			Property::IMAGE_NAME_STRING  + QString(":Rat_Sleeping1R;Rat_Sleeping2R;Rat_Sniffing1R;Rat_Sniffing2R;Rat_Walking1R;Rat_Walking2R;Rat_Walking3R;Rat_Nomming1R;Rat_Nomming2R/") +
			Property::MASS_STRING
				+ ":" + QString::number(theRatMass) + "/");

}

Rat::~Rat()
{
	;
}


void Rat::callbackStep (qreal aTimeStep, qreal aTotalTime)
{
	switch(theCurrentState)
	{
	case SLEEPING1:
	case SLEEPING2:
		callbackStepSleeping(aTimeStep, aTotalTime);
		break;
	case SNIFFINGLEFT:
	case SNIFFINGRIGHT:
		callbackStepSniffing(aTimeStep, aTotalTime);
		break;
	case WALKING1:
	case WALKING2:
	case WALKING3:
		callbackStepWalking(aTimeStep, aTotalTime);
		break;
	case NOMMING1:
	case NOMMING2:
		callbackStepNomming(aTimeStep, aTotalTime);
		break;
	}
}

void Rat::callbackStepNomming (qreal aTimeStep, qreal aTotalTime)
{
}

void Rat::callbackStepSleeping (qreal aTimeStep, qreal aTotalTime)
{
}

void Rat::callbackStepSniffing (qreal aTimeStep, qreal aTotalTime)
{
}

void Rat::callbackStepWalking (qreal aTimeStep, qreal aTotalTime)
{
}

BaseObject* Rat::findClosestFood()
{
	if (theAllFoodIDs.isEmpty())
	{
		QStringList myList = theWorldPtr->getAllIDs();
		for (int i=0; i< myList.count(); i++)
		{
			if (myList[i].startsWith("Food"))
				theAllFoodIDs.push_back(myList[i]);
		}
	}
	if (theAllFoodIDs.isEmpty())
		return NULL;
	int i;
	qreal myLength = getVectorToFood(theClosestFoodPtr).length();
	for(i=0;i<theAllFoodIDs.count();i++)
	{
		BaseObject* myBOPtr = theWorldPtr->findObjectByID(theAllFoodIDs[i]);
		qreal myTempLength = getVectorToFood(myBOPtr).length();
		if (myTempLength < myLength)
		{
			theClosestFoodPtr = myBOPtr;
			myLength = myTempLength;
		}
	}
	return theClosestFoodPtr;
}

Vector Rat::getVectorToFood(BaseObject* aFoodPtr) const
{
	if (aFoodPtr==NULL)
		return Vector(1000,1000);
	return Vector(aFoodPtr->getTempCenter()) - Vector(getTempCenter());
}

Rat::RatStates Rat::goToState(Rat::RatStates aNewState)
{
	theCurrentState = aNewState;
	return theCurrentState;
}

void Rat::createPhysicsObject()
{
	RectObject::createPhysicsObject();
	theAllFoodIDs.clear();
}
