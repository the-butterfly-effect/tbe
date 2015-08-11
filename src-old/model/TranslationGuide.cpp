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

#include "tbe_global.h"
#include "TranslationGuide.h"
#include "World.h"
#include "Box2D.h"
#include "DrawObject.h"

//// this class' ObjectFactory
class TranslationGuideObjectFactory : public ObjectFactory
{
public:
	TranslationGuideObjectFactory(void)
	{	announceObjectType("TranslationGuide", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new TranslationGuide()); }
};
static TranslationGuideObjectFactory theRFactory;



TranslationGuide::TranslationGuide()
		: BaseJoint(), theObjectPtr(NULL)
{
	DEBUG5("TranslationGuide::TranslationGuide\n");
	initAttributes ();
}

TranslationGuide::TranslationGuide(BaseObject* aBaseObject, qreal aDirection)
		: BaseJoint(), theObjectPtr(aBaseObject)
{
	DEBUG4("TranslationGuide::TranslationGuide(%p, %f)\n",
		   aBaseObject, aDirection);
	theDirection = aDirection;
	updateOrigCenter();
	initAttributes();
}

void TranslationGuide::createPhysicsObject(void)
{
	if (theWorldPtr==NULL)
		return;

	// *** parse object
	// NOTE: if we used the constructor with baseobject, this will still work
	// because propertyToObjectPtr only modifies theFirstPtr if successful
	theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT_STRING, &theObjectPtr);
	if (theObjectPtr==NULL)
		theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT1_STRING, &theObjectPtr);
	if (theObjectPtr==NULL)
	{
		DEBUG4("TranslationGuide: No valid object found...\n");
		return;
	}
	b2Body* myFirstB2BodyPtr = getB2BodyPtrFor(theObjectPtr,
											   getOrigCenter()-theObjectPtr->getOrigCenter());
	theObjectPtr->addJoint(this);
	assert (myFirstB2BodyPtr);

	// Contrary to PivotPoint, there is no object2, so use the ground body.
	// available as theGroundBodyPtr...
	b2Body* mySecondB2BodyPtr = getGroundBodyPtr();

	// *** initialise Box2D's joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr, getOrigCenter().toB2Vec2(), Vector(theDirection).toB2Vec2());
	myJointDef.userData = this;
	myJointDef.collideConnected = areObjectsColliding;

	// set motor speed and/or torque
	// note that we have the + defined in the mathematical way,
	// with the y in the opposite direction from QT, that means a minus somewhere
	float myForce = 1000.0;
	myJointDef.enableMotor = theProps.property2Float(Property::FORCE_STRING, &myForce);
	float myMotorSpeed = 0.0;
	if (theProps.property2Float(Property::SPEED_STRING, &myMotorSpeed))
		myJointDef.enableMotor = true;
	myJointDef.maxMotorForce = myForce;
	myJointDef.motorSpeed = -myMotorSpeed;

	theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);
}

void TranslationGuide::initAttributes ( )
{
	areObjectsColliding=false;

	theProps.setDefaultPropertiesString(
		Property::OBJECT_STRING + QString(":/") +
		Property::SPEED_STRING + QString(":/") +
		Property::FORCE_STRING + QString(":/") +
		Property::COLLIDE_STRING + QString(":false/") +
		"-" + Property::MASS_STRING + ":/" );
}

void TranslationGuide::parseProperties(void)
{
	BaseJoint::parseProperties();
	theProps.property2Bool(Property::COLLIDE_STRING, &areObjectsColliding);
}


void TranslationGuide::updateOrigCenter(void)
{
	setOrigCenter(theObjectPtr->getOrigCenter());
}

