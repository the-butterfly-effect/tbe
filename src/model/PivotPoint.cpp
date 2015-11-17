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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "Box2D.h"
#include "tbe_global.h"
#include "ObjectFactory.h"
#include "PivotPoint.h"
#include "ViewObject.h"
#include "World.h"

//// this class' ObjectFactory
class PivotPointObjectFactory : public ObjectFactory
{
public:
	PivotPointObjectFactory(void)
	{	announceObjectType("PivotPoint", this); }
    AbstractObject* createObject(void) const override
    {	return fixObject(new PivotPoint()); }
};
static PivotPointObjectFactory theRFactory;



PivotPoint::PivotPoint()
		: AbstractJoint(), theFirstPtr(nullptr)

{
	DEBUG5("PivotPoint::PivotPoint");
	initPivotAttributes ();
}

PivotPoint::PivotPoint(AbstractObjectPtr aAbstractObject, const Vector& aRelativePosition)
		: AbstractJoint(), theFirstPtr(aAbstractObject)
{
	DEBUG4("PivotPoint::PivotPoint(aAbstractObject*=%p, aPos=(%f,%f))",
           aAbstractObject.get(), aRelativePosition.dx, aRelativePosition.dy);
    thePosRelativeToFirst = aRelativePosition;
	updateOrigCenter();
	initPivotAttributes();
}

PivotPoint::~PivotPoint()
{
	DEBUG5("PivotPoint::~PivotPoint() for %p", this);
}

void PivotPoint::createPhysicsObject(void)
{
    assert(theWorldPtr!=nullptr);
    if (theWorldPtr==nullptr)
		return;

    // *** parse object/object1 - if we didn't get theFirstPtr from the constructor
    if (theFirstPtr==nullptr)
    {
        theFirstPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT1_STRING);
        if (theFirstPtr==nullptr)
            theFirstPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT_STRING);
    }
    if (theFirstPtr==nullptr)
	{
		DEBUG4("PivotPoint: No valid first object found...");
		return;
	}

	Vector myRelPos = (getOrigCenter().toVector()-theFirstPtr->getOrigCenter().toVector())
					  .rotate(-theFirstPtr->getOrigCenter().angle);
	b2Body* myFirstB2BodyPtr = getB2BodyPtrFor(theFirstPtr, myRelPos);
    theFirstPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));
	assert (myFirstB2BodyPtr);

	// *** parse (optional) object2
    theSecondPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT2_STRING);

	// if there is no object2, use the ground body.
	// available as theGroundBodyPtr...
	b2Body* mySecondB2BodyPtr = getGroundBodyPtr();
    if (theSecondPtr != nullptr)
	{
		Vector my2RelPos = (getOrigCenter().toVector()-theSecondPtr->getOrigCenter().toVector())
					  .rotate(-theSecondPtr->getOrigCenter().angle);
		mySecondB2BodyPtr = getB2BodyPtrFor(theSecondPtr, my2RelPos);
        theSecondPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));
	}

	// *** initialise Box2D's joint:
	// note: Initialize() uses a global coordinate...
	b2RevoluteJointDef myJointDef;
	myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr, getOrigCenter().toB2Vec2());
    myJointDef.userData = this;
	myJointDef.collideConnected = areObjectsColliding;

	// set motor speed and/or torque
	// note that we have the + defined in the mathematical way,
	// with the y in the opposite direction from QT, that means a minus somewhere
	float myTorque = 1000.0;
	myJointDef.enableMotor = theProps.property2Float(Property::TORQUE_STRING, &myTorque);
	float myMotorSpeed = 0.0;
	if (theProps.property2Float(Property::SPEED_STRING, &myMotorSpeed))
		myJointDef.enableMotor = true;
	myJointDef.maxMotorTorque = myTorque;
	myJointDef.motorSpeed = -myMotorSpeed;

	theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);
}

void PivotPoint::initPivotAttributes ( )
{
	theSecondPtr = nullptr;
	areObjectsColliding=false;

    theToolTip = QObject::tr("Objects rotate around this point");
    theProps.setDefaultPropertiesString(
		Property::OBJECT1_STRING + QString(":/") +
		Property::OBJECT2_STRING + QString(":/") +
		Property::SPEED_STRING + QString(":/") +
		Property::TORQUE_STRING + QString(":/") +
		Property::COLLIDE_STRING + QString(":false/") +
		"-" + Property::MASS_STRING + ":/" );
}

void PivotPoint::parseProperties(void)
{
	AbstractJoint::parseProperties();
	theProps.property2Bool(Property::COLLIDE_STRING, &areObjectsColliding);
}


void PivotPoint::updateOrigCenter(void)
{
	setOrigCenter(theFirstPtr->getOrigCenter()+thePosRelativeToFirst);
}
