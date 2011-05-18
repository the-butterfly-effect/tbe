/* The Butterfly Effect
 * This file copyright (C) 2010,2011  Klaas van Gend
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

#include "BalloonCactus.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "Property.h"


//
//  This source file lists 3 classes:
//    * Balloon
//    * Cactus
//    * BedOfNails
//

///---------------------------------------------------------------------------
///------------------------ Balloon ------------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
class BalloonObjectFactory : public ObjectFactory
{
public:
	BalloonObjectFactory(void)
	{	announceObjectType("Balloon", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Balloon()); }
};
static BalloonObjectFactory theBalloonObjectFactory;


const qreal Balloon::POPPING_TIME = 0.3;
const qreal Balloon::POPPED_MASS  = 0.6;
const qreal Balloon::POPPED_TIME  = 2.0;


Balloon::Balloon()
		: PolyObject(QObject::tr("Balloon"),
					 QObject::tr("a Helium Balloon. Lighter than air, it moves up."),
					 "Balloon;BalloonPoof;BalloonRest;Empty",
					 "(-0.018,0.18)=(-0.07,0.16)=(-0.12,0.1)=(-0.13,0.017)=(-0.1,-0.08)"
					 "=(-0.03,-0.16)=(0.006,-0.17)=(0.039,-0.16)=(0.10,-0.08)"
					 "=(0.13,0.015)=(0.11,0.11)=(0.07,0.16)=(0.01,0.18)",
					 0.27, 0.36, 0.1, 0.7)
{
	theState = BALLOON;
	theB2BodyDefPtr->linearDamping  = 2.1f;
	theB2BodyDefPtr->angularDamping = 0.4f;
}

Balloon::~Balloon()
{
}


void Balloon::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
	DEBUG6("Balloon receives callback\n");

	switch(theState)
	{
	case BALLOON:
		callbackStepBalloon(aDeltaTime, aTotalTime);
		break;
	case POPPING:
		callbackStepPopping(aDeltaTime, aTotalTime);
		break;
	case POPPED:
		callbackStepPopped(aDeltaTime, aTotalTime);
		break;
	case GONE:
		break;
	}
}

void Balloon::callbackStepBalloon(qreal, qreal)
{
	// the upward force...
	// the balloon weighs 100 grams, i.e. it should have a constant gravity
	// pull of 0.981 N.
	//
	// Why isn't that the case?
	// See PolyObject::fillShapeList - the density is calculated over the AABB
	// of the balloon, not the actual surface.
	// so Box2D thinks our Balloon is actually lighter than 100 grams...
	//
	// That's why a 0.8N upwards force suffices for now :-)
	theB2BodyPtr->ApplyForce(b2Vec2(0,0.8), (getTempCenter()+Vector(0,0.1)).toB2Vec2());

	// damping is now handled by Box2D - linearDamping and AngularDamping...
}

void Balloon::callbackStepPopped(qreal, qreal aTotalTime)
{
	assert(thePoppingTimeStart>0.1);
	qreal myDelta = aTotalTime - thePoppingTimeStart - POPPING_TIME;
	if ( myDelta >= POPPED_TIME)
		goToState(GONE);
}

void Balloon::callbackStepPopping(qreal, qreal aTotalTime)
{
	// is this the first callback in Popping state???
	if (thePoppingTimeStart<0.0001)
	{
		thePoppingTimeStart=aTotalTime;
		switchToSmallShape();
	}
	if (aTotalTime-thePoppingTimeStart > POPPING_TIME)
		goToState(POPPED);
}


void Balloon::createPhysicsObject(void)
{
	theState = BALLOON;
	thePoppingTimeStart = 0;
	clearShapeList();
	fillShapeList();
	PolyObject::createPhysicsObject();
	theWorldPtr->registerCallback(this);
}

void Balloon::deletePhysicsObject(void)
{
	// nothing much to do here that actually has to do with delete...
	theState = BALLOON;
	thePoppingTimeStart = 0;
	clearShapeList();
	fillShapeList();
	
	PolyObject::deletePhysicsObject();
}

void Balloon::deletePhysicsObjectForReal(void)
{
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = NULL;
}


Balloon::States Balloon::goToState(Balloon::States aNewState)
{
	DEBUG5("Balloon change state request from %d to %d.\n", theState, aNewState);

	switch (theState)
	{
	case BALLOON:
		if (aNewState==POPPING)
		{
			theState = POPPING;
			// everything else will be handled in callbackStepPopping
		}
		if (aNewState==POPPED)
		{
			// typical case of "this should never happen"
			assert(false);
		}
		break;
	case POPPING:
		if (aNewState==POPPED)
		{
			theState = POPPED;
		}
		break;
	case POPPED:
		if (aNewState==GONE)
		{
			theState=GONE;
			deletePhysicsObjectForReal();
		}
		break;
	case GONE:
		// nothing to be done here...
		break;
	}
	return theState;
}

void Balloon::notifyJoints(JointInterface::JointStatus aStatus)
{
	if (theState != BALLOON && aStatus==JointInterface::CREATED)
		return;
	BaseObject::notifyJoints(aStatus);
}


void Balloon::reportNormalImpulseLength(qreal anImpulseLength)
{
	// also pop the balloon if it is maltreated
	// WARNING: Magic number here!!!
	if (anImpulseLength > 2.2 && theState==BALLOON)
		goToState(POPPING);
}


void Balloon::stung(void)
{
	if (theState == BALLOON)
		goToState(POPPING);
}

void Balloon::switchToSmallShape(void)
{
	// save the current position - as it is only stored within the B2Body
	// and we'll kill it in the next line...
	Position myCurrentPos = getTempCenter();

	deletePhysicsObjectForReal();
	clearShapeList();

	b2PolygonShape* myRestShape = new b2PolygonShape();
	myRestShape->SetAsBox(0.05, 0.05);
	b2FixtureDef* myRestDef = new b2FixtureDef();
	myRestDef->density= 0.001 / (0.1 * 0.1);
	myRestDef->userData = this;
	myRestDef->shape   = myRestShape;
	theShapeList.push_back(myRestDef);

	PolyObject::createPhysicsObject(myCurrentPos);
}


///---------------------------------------------------------------------------
///------------------------ Cactus -------------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
class CactusObjectFactory : public ObjectFactory
{
public:
	CactusObjectFactory(void)
	{	announceObjectType("Cactus", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Cactus()); }
};
static CactusObjectFactory theCactusObjectFactory;


Cactus::Cactus() : PolyObject(QObject::tr("Cactus"),
							  QObject::tr("Cactacea Bulbuous Stingus - a Cactus has spines!"),
							  "Cactus",
							  // first the pot:
							  "(-0.09,-0.038)=(-0.04,-0.2)=(0.04,-0.2)=(0.09,-0.038);"
							  // then the plant itself:
							  "(0.045,-0.036)=(0.085,0.012)=(0.092,0.089)=(0.046,0.156)=(0.00,0.171)"
							  "=(-0.046,0.156)=(-0.092,0.089)=(-0.085,0.012)=(-0.045,-0.036)"
							  ";(-0.125,-0.2);(0.125,0.2)"
							  ,
							  0.25, 0.4, 1.0, 0.3)
{
}

Cactus::~Cactus()
{
}

void Cactus::callBackSensor(const ContactInfo& aPoint)
{
	BaseObject* myOtherObject=NULL;

	// which one of the two shapes is not me?
	if (aPoint.myFixtureA->GetUserData()==this)
		myOtherObject = aPoint.myFixtureB->GetUserData();
	if (aPoint.myFixtureB->GetUserData()==this)
		myOtherObject = aPoint.myFixtureA->GetUserData();
	if (myOtherObject==NULL)
		return;

	// is it a Balloon?
	// then pop it!
	Balloon* myBalloonPtr = dynamic_cast<Balloon*>(myOtherObject);
	if (myBalloonPtr!=NULL)
		myBalloonPtr->stung();
}

void Cactus::fillShapeList(void)
{
	PolyObject::fillShapeList();

	// And add the sensor to the shapes
	b2PolygonShape* mySensorShape = new b2PolygonShape();
	mySensorShape->m_vertexCount = 10;
	mySensorShape->m_vertices[0]=b2Vec2( 0.078, -0.036);
	mySensorShape->m_vertices[1]=b2Vec2( 0.116,  0.012);
	mySensorShape->m_vertices[2]=b2Vec2( 0.125,  0.085);
	mySensorShape->m_vertices[3]=b2Vec2( 0.099,  0.147);
	mySensorShape->m_vertices[4]=b2Vec2( 0.038,  0.2);
	mySensorShape->m_vertices[5]=b2Vec2(-0.038,  0.2);
	mySensorShape->m_vertices[6]=b2Vec2(-0.099,  0.147);
	mySensorShape->m_vertices[7]=b2Vec2(-0.125,  0.085);
	mySensorShape->m_vertices[8]=b2Vec2(-0.116,  0.012);
	mySensorShape->m_vertices[9]=b2Vec2(-0.078, -0.036);
	b2FixtureDef* mySensorDef = new b2FixtureDef();
	mySensorDef->shape    = mySensorShape;
	mySensorDef->isSensor = true;
	mySensorDef->userData = this;
	theShapeList.push_back(mySensorDef);
}


///---------------------------------------------------------------------------
///------------------------ Bed Of Nails -------------------------------------
///---------------------------------------------------------------------------


// this class' ObjectFactory
class BedOfNailsObjectFactory : public ObjectFactory
{
public:
	BedOfNailsObjectFactory(void)
	{	announceObjectType("BedOfNails", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new BedOfNails()); }
};
static BedOfNailsObjectFactory theBedOfNailsObjectFactory;


BedOfNails::BedOfNails()
		: PolyObject(QObject::tr("BedOfNails"),
					 QObject::tr("Do not touch a bed of nails - it stings!"),
					 "BedOfNails",
					 // first the bar:
					 "(-0.4,-0.075)=(0.4,-0.075)=(0.4,0.006)=(-0.4,0.006);"
					 // then the nails block:
					 "(-0.34,-0.05)=(0.34,-0.05)=(0.34,0.06)=(-0.34,0.06)"
					 ,
					 0.8, 0.15, 0.0, 0.2)
{
}

BedOfNails::~BedOfNails()
{
}

void BedOfNails::callBackSensor(const ContactInfo& aPoint)
{
	BaseObject* myOtherObject=NULL;

	// which one of the two shapes is not me?
	if (aPoint.myFixtureA->GetUserData()==this)
		myOtherObject = aPoint.myFixtureB->GetUserData();
	if (aPoint.myFixtureB->GetUserData()==this)
		myOtherObject = aPoint.myFixtureA->GetUserData();
	if (myOtherObject==NULL)
		return;

	// is it a Balloon?
	// then pop it!
	Balloon* myBalloonPtr = dynamic_cast<Balloon*>(myOtherObject);
	if (myBalloonPtr!=NULL)
		myBalloonPtr->stung();
}

void BedOfNails::fillShapeList(void)
{
	PolyObject::fillShapeList();

	// And add the sensor to the shapes
	b2PolygonShape* mySensorShape = new b2PolygonShape();
	mySensorShape->m_vertexCount = 4;
	mySensorShape->m_vertices[0]=b2Vec2(-0.335, -0.04);
	mySensorShape->m_vertices[1]=b2Vec2( 0.335, -0.04);
	mySensorShape->m_vertices[2]=b2Vec2( 0.335,  0.07);
	mySensorShape->m_vertices[3]=b2Vec2(-0.335,  0.07);
	b2FixtureDef* mySensorDef = new b2FixtureDef();
	mySensorDef->shape    = mySensorShape;
	mySensorDef->isSensor = true;
	mySensorDef->userData = this;
	theShapeList.push_back(mySensorDef);
}

///---------------------------------------------------------------------------
///-------------------------- CircularSaw ------------------------------------
///---------------------------------------------------------------------------


// this class' ObjectFactory
class CircularSawObjectFactory : public ObjectFactory
{
public:
	CircularSawObjectFactory(void)
	{	announceObjectType("CircularSaw", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new CircularSaw()); }
};
static CircularSawObjectFactory theCircularSawObjectFactory;

CircularSaw::CircularSaw()
		: AbstractBall(QObject::tr("CircularSaw"),
					 QObject::tr("a rotating disc with sharp teeth."),
					 "CircularSaw",
					 0.19, 0.4, 0.1)
{
}

CircularSaw::~CircularSaw()
{
}

void CircularSaw::callBackSensor(const ContactInfo& aPoint)
{
	BaseObject* myOtherObject=NULL;

	// which one of the two shapes is not me?
	if (aPoint.myFixtureA->GetUserData()==this)
		myOtherObject = aPoint.myFixtureB->GetUserData();
	if (aPoint.myFixtureB->GetUserData()==this)
		myOtherObject = aPoint.myFixtureA->GetUserData();
	if (myOtherObject==NULL)
		return;

	// is it a Balloon?
	// then pop it!
	Balloon* myBalloonPtr = dynamic_cast<Balloon*>(myOtherObject);
	if (myBalloonPtr!=NULL)
		myBalloonPtr->stung();
}

void CircularSaw::createBallShapeFixture(float aRadius, float aMass)
{
	// add the original ball shape
	AbstractBall::createBallShapeFixture(aRadius, aMass);

	// and add our sensor
	b2CircleShape* ballShape = new b2CircleShape();
	ballShape->m_radius = aRadius+0.01;

	b2FixtureDef* ballFixDef = new b2FixtureDef();
	ballFixDef->density = 0;
	ballFixDef->userData = this;
	ballFixDef->shape = ballShape;
	ballFixDef->isSensor = true;

	theShapeList.push_back(ballFixDef);
}
