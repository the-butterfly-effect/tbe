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

#include "Ramp.h"
#include "tbe_global.h"
#include "PivotPoint.h"
#include "World.h"
#include "Box2D.h"
#include "DrawObject.h"

/// pointer to World's groundbody.
static b2Body* theGroundBodyPtr = NULL;

//// this class' ObjectFactory
class PivotPointObjectFactory : public ObjectFactory
{
public:
	PivotPointObjectFactory(void)
	{	announceObjectType("PivotPoint", this); }
	virtual BaseObject* createObject(void) const
	{	return new PivotPoint(); }
};
static PivotPointObjectFactory theRFactory;



PivotPoint::PivotPoint()
		: theFirstPtr(NULL), theSecondPtr(NULL), theJointPtr(NULL)
{
	DEBUG5("PivotPoint::PivotPoint\n");
}

PivotPoint::PivotPoint(BaseObject* aBaseObject, const Position& aPosition)
		: theFirstPtr(aBaseObject), theSecondPtr(NULL), theJointPtr(NULL)
{
	DEBUG4("PivotPoint::PivotPoint(%p, (%f,%f))\n", aBaseObject, aPosition.x, aPosition.y);
	setOrigCenter(aPosition);
}

DrawObject*  PivotPoint::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	if (isPhysicsObjectCreated()==false)
		createPhysicsObject();
	QString myImageName = theProps.getProperty(Property::IMAGE_NAME_STRING);
	if (myImageName.isEmpty())
		return NULL;
	theDrawObjectPtr = new DrawObject(this, myImageName);
	return theDrawObjectPtr;
}

void PivotPoint::createPhysicsObject(void)
{
	DEBUG5("PivotPoint::createPhysObj() with %p...\n", theWorldPtr);

	if (theWorldPtr==NULL)
		return;

	// *** parse object/object1
	// NOTE: if we used the constructor with baseobject, this will still work
	// because propertyToObjectPtr only modifies theFirstPtr if successful
	propertyToObjectPtr(theWorldPtr, Property::OBJECT1_STRING, &theFirstPtr);
	if (theFirstPtr==NULL)
		propertyToObjectPtr(theWorldPtr, Property::OBJECT_STRING, &theFirstPtr);
	if (theFirstPtr==NULL)
	{
		DEBUG2("PivotPoint: No valid first object found...\n");
		return;
	}
	b2Body* myFirstB2BodyPtr = theFirstPtr->theB2BodyPtr;
	assert (myFirstB2BodyPtr);

	// *** parse (optional) object2
	propertyToObjectPtr(theWorldPtr, Property::OBJECT2_STRING, &theSecondPtr);

	// if there is no object2, use the ground body.
	// available as theGroundBodyPtr...
	b2Body* mySecondB2BodyPtr = theGroundBodyPtr;
	if (theSecondPtr != NULL)
	mySecondB2BodyPtr = theSecondPtr->theB2BodyPtr;

	Vector myCoordinates = getOrigCenter().toVector();
	if (myCoordinates == Vector(0,0))
	{
		if (theSecondPtr == NULL)
		{
			myCoordinates = theFirstPtr->getOrigCenter().toVector();
		}
		else
		{
			// average between the two object's centers
			myCoordinates = 0.5* (theSecondPtr->getOrigCenter().toVector()
								  + theFirstPtr->getOrigCenter().toVector());
		}
	}

	// *** initialise Box2D's joint:
	// note: Initialize() uses a global coordinate...
	b2RevoluteJointDef myJointDef;
	myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr, myCoordinates.toB2Vec2());
	myJointDef.userData = this;

	// TODO/FIXME: not implemented property yet
	myJointDef.collideConnected = false;

	theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);
}

void PivotPoint::deletePhysicsObject(void)
{
	DEBUG5("PivotPoint::deletePhys()...\n");
	if (theJointPtr)
		delete theJointPtr;
	theJointPtr = NULL;
}

Position PivotPoint::getTempCenter (void) const
{
	// no physics object, no temp center
	// FIXME/TODO: this is not entirely correct as a pivot point *could* move.
	return getOrigCenter();
}

bool PivotPoint::propertyToObjectPtr(
		World* aWPtr,
		const QString& aPropertyName,
		BaseObject** aBOPtrPtr)
{
	QString myValue = theProps.getProperty(aPropertyName);
	if (myValue.isEmpty())
		return false;
	*aBOPtrPtr = aWPtr->findObjectByID(myValue);
	if (*aBOPtrPtr == NULL)
		return false;
	return true;
}

void PivotPoint::reset(void)
{
	// nothing to do here
}

void PivotPoint::setGroundBodyPtr(b2Body* aPtr)
{
	theGroundBodyPtr = aPtr;
}
