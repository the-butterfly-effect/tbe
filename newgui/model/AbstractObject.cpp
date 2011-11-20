/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#include "AbstractObject.h"
#include "Box2D.h"

// I wonder if this should be b2_linearSlop instead of this number...
const float AbstractObject::MINIMUM_DIMENSION = 0.03;


AbstractObject::AbstractObject()
{
}

const QString AbstractObject::getName ( )
{
	// TODO: make abstract
	return "AbstractObject";
}


bool AbstractObject::isMovable ( ) const
{
	if (theIsLevelEditor)
		return true;
	else
		return theIsMovable;
}

bool AbstractObject::isRotatable ( ) const
{
	bool myRotatableInfo = false;
	theProps.property2Bool(Property::ROTATABLE_STRING, &myRotatableInfo);
	return myRotatableInfo;
}


void AbstractObject::parseProperties(void)
{
	// use the default if nothing else...
	float myFloat;
	theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
	setTheBounciness(myFloat);

#if 0 // directly copied from BaseObject.cpp - not used for newgui yet
	Vector myDelta;
	if (theProps.property2Vector(Property::PIVOTPOINT_STRING, &myDelta))
	{
		PivotPoint* myPP = new PivotPoint(this, myDelta);
		myPP->markAsChild();
		theWorldPtr->addObject(myPP);
	}
	float myAngle;
	if (theProps.property2Float(Property::TRANSLATIONGUIDE_STRING, &myAngle))
	{
		TranslationGuide* myTJ = new TranslationGuide(this, myAngle);
		myTJ->markAsChild();
		theWorldPtr->addObject(myTJ);
	}

	QString myNoCollisionObjectIDs;
	theProps.property2String(Property::NOCOLLISION_STRING, &myNoCollisionObjectIDs);
	QStringList myObjIDList = myNoCollisionObjectIDs.split(";", QString::SkipEmptyParts);
	QStringList::iterator myI = myObjIDList.begin();
	while (myI != myObjIDList.end())
	{
		BaseObject* myObjPtr = theWorldPtr->findObjectByID(*myI);
		if (myObjPtr!=NULL)
			theWorldPtr->addNoCollisionCombo(this, myObjPtr);
		++myI;
	}

	// For normal situations, i.e. created by Level for World
	// the setupCache() is run twice. But that's not a problem.
	// For inserted objects (i.e. drag from toolbox), this is
	// the only time setupCache is called. Let's cherish it.
	if (theDrawObjectPtr)
		theDrawObjectPtr->setupCache();
#endif
}
