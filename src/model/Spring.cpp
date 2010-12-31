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

#include "Spring.h"
#include "tbe_global.h"

//// the Spring class' ObjectFactory
class SpringObjectFactory : public ObjectFactory
{
public:
	SpringObjectFactory(void)
	{	announceObjectType("Spring", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Spring()); }
};
static SpringObjectFactory theSpringObjectFactory;


// this object is a complicated one to get right, because it toys with
// variable width and such... Complicating factor is the fact that the
// physical object is significantly smaller than the image or the real object
//
// let's try to draw:
//
// INITIAL STATE
//  (the DrawObject is drawn over the total outline)
//      +-------------+-------------+
//      |  physobject |  physobject |
//      |   Spring    *  SpringEnd  |
//      |             |             |
//      +-------------+-------------+
//
//      |<------------------------->|
//             getTheWidth()
//  * = what getOrigCenter returns
//
//
//
// DURING SIMULATION
//  (the DrawObject is drawn over the current outline)
//      +---------+===+----------+
//      |         |   |          |
//      | Spring  | * |SXringEnd |
//      |         |   |          |
//      +---------+===+----------+
//
//      |<------------------------->|
//             getTheWidth()
//      |<---------------------->|
//             getTempWidth()
//  * = what getTempCenter returns
//  X = what getOrigCenter returns - WHICH IS NOT IN THE MIDDLE ANYMORE!!!
//      (Klaas doesn't believe that actually matters here)
//
// this is why adjustParameters() and createPhysicsObject()
// have (new) implementations


Spring::Spring()
		:	RectObject( QObject::tr("Detonator Box"),
				"",
				"spring20",
				0.4, 0.2, 0.8, 0.0), theHandleObjectPtr(NULL)
{
//	theProps.setDefaultPropertiesString(
//		Property::PHONENUMBER_STRING + QString(":/") );
}

Spring::~Spring()
{
	delete theHandleObjectPtr;
	theHandleObjectPtr = NULL;
}

void Spring::adjustParameters(void)
{
	clearShapeList();
	buildShapeList();

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}

void Spring::buildShapeList(void)
{
	DEBUG5("Spring::buildShapeList wxh=%fx%f\n", getTheWidth(),getTheHeight());
	b2PolygonShape* myBoxShape = new b2PolygonShape();
	myBoxShape->SetAsBox(getTheWidth()/4.0, getTheHeight()/2.0);
	b2FixtureDef* myBoxFixture = new b2FixtureDef();

	// get mass:  springs always have a mass!
	float myMass;
	if (theProps.property2Float(Property::MASS_STRING, &myMass))
		myBoxFixture->density = myMass / (getTheWidth()*getTheHeight()) / 2.0;
	assert (myMass > 0.0);
	myBoxFixture->shape    = myBoxShape;
	myBoxFixture->userData = this;
	setFriction(myBoxFixture);
	theShapeList.push_back(myBoxFixture);
}

void Spring::createPhysicsObject(void)
{
	clearShapeList();
	buildShapeList();

	RectObject::createPhysicsObject(getOrigCenter()+Vector(-0.25*getTheWidth(),0));

	if (theHandleObjectPtr)
	{
		theHandleObjectPtr->setTheWidth(0.5*getTheWidth());
		theHandleObjectPtr->setTheHeight(getTheHeight());
		theHandleObjectPtr->setOrigCenter(getOrigCenter()+Vector(0.25*getTheWidth(),0));
		theHandleObjectPtr->createPhysicsObject();
	}
}

void Spring::deletePhysicsObject(void)
{
	if(theHandleObjectPtr)
		theHandleObjectPtr->deletePhysicsObject();
	RectObject::deletePhysicsObject();
}

Position Spring::getTempCenter (void) const
{
	// no physics object, no temp center
	if (isPhysicsObjectCreated()==false)
		return getOrigCenter();

	Vector myP1(theB2BodyPtr->GetPosition());
	Vector myP2(theHandleObjectPtr->theB2BodyPtr->GetPosition());
	return Position(0.5*Vector(myP1+myP2), theB2BodyPtr->GetAngle());
}

qreal Spring::getTempWidth() const
{
	if (theHandleObjectPtr==NULL)
		return 0;
	return theHandleObjectPtr->getDistance() + getTheWidth();
}

const QString Spring::getToolTip ( ) const
{
	//: Translators: The %1 will be replaced by a phone number.
	return QObject::tr("Send BOOM to me");
}

void Spring::reset(void)
{
	RectObject::reset();

	// the reset will already do a delete+create
	// of the handle objectptr, so no need to reset it specifically here...
	if (theHandleObjectPtr==NULL)
	{
		theHandleObjectPtr = new SpringHandle(this, getOrigCenter()+0.25*getTheWidth(), getTheWidth()/2.0, getTheHeight());
		theWorldPtr->addObject(theHandleObjectPtr);
	}
}

void Spring::setOrigCenter ( Position new_var )
{
	RectObject::setOrigCenter(new_var);

	Vector myOffset = Vector(0.25*getTheWidth(),0);
	if (theHandleObjectPtr!=NULL)
		theHandleObjectPtr->setOrigCenter(new_var + myOffset);
}

// ##########################################################################
// ##########################################################################
// ##########################################################################

SpringHandle::SpringHandle(Spring* aDBox, const Position& aPos, qreal aWidth, qreal aHeight)
		:	RectObject( QObject::tr("Spring End"),
				"no tooltip",
				"spring20",
				aWidth, aHeight, 0.1, 0.0), theDBoxPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(aPos);
	createPhysicsObject();
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
	theIsMovable = false;
}

SpringHandle::~SpringHandle()
{
	theWorldPtr->removeObject(this);
}

void SpringHandle::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
// FIXME/TODO: need ticket:243 complete before I can finish this:
//	theJointPtr->SetMotorForce(5 * theJointPtr->GetJointTranslation());
}

DrawObject*  SpringHandle::createDrawObject(void)
{
	return NULL;
}

void SpringHandle::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();

	// initialise the prismatic (translation) joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(theDBoxPtr->theB2BodyPtr,
						  theB2BodyPtr,
						  theDBoxPtr->getOrigCenter().toB2Vec2(),
						  Vector(1.0,0.0).toB2Vec2());
	myJointDef.userData = NULL;
	myJointDef.collideConnected = false;
	myJointDef.maxMotorForce = 1.0f;
	myJointDef.motorSpeed = 0.0;
	myJointDef.lowerTranslation = - getTheWidth();
	myJointDef.upperTranslation = + getTheWidth();
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = true;

	assert(theJointPtr==NULL);
	theJointPtr = reinterpret_cast<b2PrismaticJoint*>(getB2WorldPtr()->CreateJoint(&myJointDef));
}

void SpringHandle::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal SpringHandle::getDistance(void) const
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}

void SpringHandle::reset(void)
{
	RectObject::reset();
	theWorldPtr->registerCallback(this);
}
