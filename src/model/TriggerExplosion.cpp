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

#include "TriggerExplosion.h"
#include "tbe_global.h"

//// the DetonatorBox class' ObjectFactory
class DetonatorBoxObjectFactory : public ObjectFactory
{
public:
	DetonatorBoxObjectFactory(void)
	{	announceObjectType("DetonatorBox", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new DetonatorBox()); }
};
static DetonatorBoxObjectFactory theDetonatorBoxObjectFactory;


const qreal  DetonatorBox::ACTIVATED_TIME = 0.5;
const qreal  DetonatorBox::RINGING_TIME   = 0.5;
#define STARTDISTANCE 0.2f
const Vector DetonatorBox::HANDLEOFFSET   = Vector(0,STARTDISTANCE);

// FIXME/TODO: Armed and Done are the same image...

DetonatorBox::DetonatorBox()
		:	RectObject( QObject::tr("Detonator Box"),
				"Push the handle down to BOOM",
				"DetonatorBoxArmed;DetonatorBoxActivated;DetonatorBoxRinging;DetonatorBoxDone",
//				"Empty",
				0.33, 0.35, 4.0, 0.0), theHandleObjectPtr(NULL)
{
}

DetonatorBox::~DetonatorBox()
{
	delete theHandleObjectPtr;
	theHandleObjectPtr = NULL;
}

void DetonatorBox::callbackStep (qreal aTimeStep, qreal aTotalTime)
{
	// TODO
}

void DetonatorBox::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();
	if (theHandleObjectPtr)
		theHandleObjectPtr->createPhysicsObject();
}

void DetonatorBox::deletePhysicsObject(void)
{
	if(theHandleObjectPtr)
		theHandleObjectPtr->deletePhysicsObject();
	RectObject::deletePhysicsObject();
}


DetonatorBox::States DetonatorBox::goToState(DetonatorBox::States aNewState)
{
	// TODO
	return ARMED;
}

void DetonatorBox::reset(void)
{
	RectObject::reset();

	// the reset will already do a delete+create
	// of the handle objectptr, so no need to reset it specifically here...
	if (theHandleObjectPtr==NULL)
	{
		theHandleObjectPtr = new DetonatorBoxHandle(this, getOrigCenter()+HANDLEOFFSET);
		theWorldPtr->addObject(theHandleObjectPtr);
	}

	isTriggered = false;
	theWorldPtr->registerCallback(this);
}

void DetonatorBox::setOrigCenter ( Position new_var )
{
	RectObject::setOrigCenter(new_var);

	assert(theHandleObjectPtr!=NULL);
	theHandleObjectPtr->setOrigCenter(new_var+HANDLEOFFSET);
}

void DetonatorBox::setTriggered(void)
{
	// let's just be nice asynchronously
	isTriggered = true;
}


// ##########################################################################
// ##########################################################################
// ##########################################################################

DetonatorBoxHandle::DetonatorBoxHandle(DetonatorBox* aDBox, const Position& aPos)
		:	RectObject( QObject::tr("Detonator Box Handle"),
				"Push Here To BOOM",
				"DetonatorBoxHandle",
				0.25, 0.26, 0.1, 0.0), theDBoxPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(aPos);
	createPhysicsObject();
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

DetonatorBoxHandle::~DetonatorBoxHandle()
{
	theWorldPtr->removeObject(this);
}

void DetonatorBoxHandle::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
	// if down, stay down and signal the box
	if (theJointPtr->GetJointTranslation() <= theJointPtr->GetLowerLimit())
	{
		theJointPtr->SetMaxMotorForce(0);
		theDBoxPtr->setTriggered();
	}
}

DrawObject*  DetonatorBoxHandle::createDrawObject(void)
{
	RectObject::createDrawObject();
	// redo the ZValue: BaseObject will set it to 2.0 (default for DrawObjects)
	// if not in Properties, set to 1.9: the Handle draws behind the Box
	setDrawObjectZValue(1.0);
	return theDrawObjectPtr;
}

void DetonatorBoxHandle::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();

	// initialise the prismatic (translation) joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(theDBoxPtr->theB2BodyPtr, theB2BodyPtr, getOrigCenter().toB2Vec2(), Vector(0,1.0).toB2Vec2());
	myJointDef.userData = NULL;
	myJointDef.collideConnected = false;
	myJointDef.maxMotorForce = 120.0f;
	myJointDef.motorSpeed = 2.0;
	myJointDef.lowerTranslation = - STARTDISTANCE/2.0f;
	myJointDef.upperTranslation = 0.0;
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = true;

	assert(theJointPtr==NULL);
	theJointPtr = reinterpret_cast<b2PrismaticJoint*>(getB2WorldPtr()->CreateJoint(&myJointDef));
}

void DetonatorBoxHandle::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal DetonatorBoxHandle::getDistance(void)
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}

void DetonatorBoxHandle::reset(void)
{
	RectObject::reset();
	theWorldPtr->registerCallback(this);
}


// ##########################################################################
// ##########################################################################
// ##########################################################################

/*
//// the DetonatorBox class' ObjectFactory
class DynamiteObjectFactory : public ObjectFactory
{
public:
	DynamiteObjectFactory(void)
	{	announceObjectType("Dynamite", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Dynamite()); }
};
static DynamiteObjectFactory theDynamiteObjectFactory;


Dynamite::Dynamite()
		: PolyObject(QObject::tr("Dynamite"),
			 QObject::tr("invented by Alfred Nobel, this is a nice explosive."),
			 "Balloon;BalloonPoof;BalloonRest;Empty",
			 "(-0.018,0.18)=(-0.07,0.16)=(-0.12,0.1)=(-0.13,0.017)=(-0.1,-0.08)"
			 "=(-0.03,-0.16)=(0.006,-0.17)=(0.039,-0.16)=(0.10,-0.08)"
			 "=(0.13,0.015)=(0.11,0.11)=(0.07,0.16)=(0.01,0.18)",
			 0.27, 0.36, 0.1, 0.7)
{
}

Dynamite::~Dynamite()
{
}

void Dynamite::callbackStep (qreal aTimeStep, qreal aTotalTime)
{
}

Dynamite::States Dynamite::goToState(Dynamite::States aNewState)
{
	return WAITING;
}

void Dynamite::reset(void)
{
	PolyObject::reset();
}
*/
