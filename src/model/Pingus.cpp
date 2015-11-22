/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#include "Pingus.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "Property.h"
#include "ViewPingus.h"

//// this class' ObjectFactory
class PingusObjectFactory : public ObjectFactory
{
public:
	PingusObjectFactory(void)
	{	announceObjectType("Pingus", this); }
	AbstractObject* createObject(void) const override
	{	return fixObject(new Pingus()); }
};
static PingusObjectFactory thePingusObjectFactory;


static const qreal FALLING_TIME   = 0.16; // seconds
static const qreal PINGUS_RADIUS  = 0.14; // m
static const qreal PINGUS_MASS    = 1.50; // kg
static const qreal SPLATTING_IMPULSE = 7.0; // ###
static const qreal SPLATTING_TIME = 0.32; // seconds, twice the FALLING_TIME
static const qreal WALKING_SPEED  = 0.50; // m/s
static const qreal WALKINGSEQS_PER_SECOND = 1.5; // 1.5 sequences per horizontal distance of [walking speed*1second]

const unsigned int Pingus::FramesPerState[] = { 8, 8, 8, 1, 1, 16, 6, 1};


Pingus::Pingus()
		: CircleObject(QObject::tr("Pingus"),
					 QObject::tr("the famous penguin. He walks and believes in you. Keep him alive!"),
					 "",
					 PINGUS_RADIUS, PINGUS_MASS, 0.0 )
{
	theState = FALLING;
	theAnimationFrameIndex = 0;
	theFallingTimeStart = -1.0;
	theSplattingTimeStart = -1.0;
}


Pingus::~Pingus()
{
}


void Pingus::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
	DEBUG6("Pingus receives callback");
	if (isPhysicsObjectCreated()==false)
		return;

	switch(theState)
	{
	case WALKINGLEFT:
	case WALKINGRIGHT:
		callbackStepWalking(aDeltaTime, aTotalTime);
		break;
	case FALLING:
		callbackStepFalling(aDeltaTime, aTotalTime);
		break;
	case SLIDELEFT:
	case SLIDERIGHT:
		callbackStepSliding(aDeltaTime, aTotalTime);
		break;
	case SPLATTING:
		callbackStepSplatting(aDeltaTime, aTotalTime);
		break;
	case WAITING:
		callbackStepWaiting(aDeltaTime, aTotalTime);
		break;
	case DEAD:
		// nothing to do
		break;
	}

	static_cast<ViewPingus*>(theViewObjectPtr)->setNewAnimationFrame(theState, theAnimationFrameIndex);
}


void Pingus::callbackStepFalling(qreal, qreal aTotalTime)
{
	// is this the first callback in Falling state???
	if (theFallingTimeStart<0)
		theFallingTimeStart=aTotalTime;

	// calculate the frame to display
	// the falling sequence has 8 images, let's take 0.16s to display them
	// (at 60fps, that means approx 2 displayframes for each animation frame)
	// note that contrary to Splatting, we do repeat the sequence
	theAnimationFrameIndex = Pingus::FramesPerState[FALLING] *
			fmodf(aTotalTime-theFallingTimeStart, FALLING_TIME)/FALLING_TIME;
}


void Pingus::callbackStepSliding(qreal aTimeStep, qreal aTotalTime)
{
	// * check for the horizontal speed
	//   - if the speed is below walking, switch to walking
	// * if we have a sufficient vertical component, we're falling
	qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
	if (fabs(myXd) < WALKING_SPEED)
	{
		goToState(WALKINGLEFT);
		callbackStepWalking(aTimeStep, aTotalTime);
		return;
	}
	if (myXd > 0.0 && theState==SLIDELEFT)
		goToState(SLIDERIGHT);
	if (myXd < 0.0 && theState==SLIDERIGHT)
		goToState(SLIDELEFT);

	// Add a bit of friction
	qreal myXImpulse = 0.0;
	if (fabs(myXd) > WALKING_SPEED)
		myXImpulse = -copysign(0.8, myXd);
	Vector myTotXImpulse = aTimeStep * Vector(myXImpulse, 0.0);
	theB2BodyPtr->ApplyLinearImpulse(
			myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);
}


void Pingus::callbackStepSplatting(qreal, qreal aTotalTime)
{
	// is this the first callback in Splatting state???
	if (theSplattingTimeStart<0)
	{
		theSplattingTimeStart=aTotalTime;
		switchToSmallShape();
	}

	// calculate the frame to display
	// the splatting sequence has 16 images, let's take 0.32s to display them
	// (at 60fps, that means approx 2 displayframes for each animation frame)
	theAnimationFrameIndex = (Pingus::FramesPerState[SPLATTING] * (aTotalTime-theSplattingTimeStart) / SPLATTING_TIME);

	// are we done?
	if (aTotalTime-theSplattingTimeStart > SPLATTING_TIME)
		goToState(DEAD);
}


void Pingus::callbackStepWalking(qreal aTimeStep, qreal aTotalTime)
{
	// * check for the horizontal speed and adjust it if needed
	//   by applying a force
	//   - if the force becomes too big, turn around
	//   - if the force is sufficiently negative, switch to sliding
	// * if we have a sufficient vertical component, we're falling
	// * set theAnimationFrameIndex to the horizontal position
	qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
	if (fabs(myXd) > WALKING_SPEED)
	{
		goToState(SLIDELEFT);
		callbackStepSliding(aTimeStep, aTotalTime);
		return;
	}

	if (myXd > 0 && theState==WALKINGLEFT)
		goToState(WALKINGRIGHT);
	if (myXd < 0 && theState==WALKINGRIGHT)
		goToState(WALKINGLEFT);

	// Add the horizontal walking force.
	// Note that this is just the P-action of a PID-controller, so there's
	// guaranteed no overshoot as myXd will never reach the WALKING_SPEED.
	// With the current settings it remains 0.0003 m/s below it.
	qreal myXImpulse = 0;
	if (fabs(myXd) < WALKING_SPEED)
		myXImpulse = copysign(5.0*(WALKING_SPEED-fabs(myXd))/WALKING_SPEED, myXd);
	Vector myTotXImpulse = aTimeStep * Vector(myXImpulse, 0);
	theB2BodyPtr->ApplyLinearImpulse(
			myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);

	// in WALKING_SPEED [m/s], we have Pingus::FramesPerState[WALKINGLEFT] animation frames to draw
	qreal temp = fmodf(theB2BodyPtr->GetPosition().x, WALKING_SPEED/WALKINGSEQS_PER_SECOND);
	temp /= WALKING_SPEED/WALKINGSEQS_PER_SECOND;
	theAnimationFrameIndex = temp * static_cast<qreal>(Pingus::FramesPerState[WALKINGLEFT]);
}


void Pingus::callbackStepWaiting(qreal aTimeStep, qreal aTotalTime)
{
}


void Pingus::createPhysicsObject(void)
{
	theState = FALLING;
	theFallingTimeStart = -1.0;
	theSplattingTimeStart = -1.0;
	createBallShapeFixture(PINGUS_RADIUS, PINGUS_MASS);
	CircleObject::createPhysicsObject();
	theWorldPtr->registerCallback(this);
}


ViewObject*  Pingus::createViewObject(float aDefaultDepth)
{
	assert(theViewObjectPtr==nullptr);
	theViewObjectPtr = new ViewPingus(getThisPtr());
	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	return theViewObjectPtr;
}


void Pingus::deletePhysicsObject(void)
{
	// nothing much to do here that actually has to do with delete...
	theState = FALLING;
	theAnimationFrameIndex=0;
	theFallingTimeStart = -1.0;
	theSplattingTimeStart = -1.0;
	clearShapeList();

	CircleObject::deletePhysicsObject();
}

void Pingus::deletePhysicsObjectForReal(void)
{
	updateViewObject(false);
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = nullptr;
}


Pingus::States Pingus::goToState(Pingus::States aNewState)
{
	DEBUG5("Pingus change state request from %d to %d.", theState, aNewState);

	// if we're not yet splatting or dead, we're splatting!
	if (aNewState == SPLATTING)
	{
		if (theState!=DEAD)
			theState=SPLATTING;
	}
	else
	{
		// we're not going to be splatting... what do we need to do?
		switch (theState)
		{
		case WALKINGLEFT:
			theState = aNewState;
			break;
		case WALKINGRIGHT:
			theState = aNewState;
			break;
		case FALLING:
			theFallingTimeStart = -1.0;
			theState = aNewState;
			break;
		case SLIDELEFT:
			theState = aNewState;
			break;
		case SLIDERIGHT:
			theState = aNewState;
			break;
		case SPLATTING:
			// it should not be possible to get out of splatting, other than dead
			if (aNewState==DEAD)
			{
				theState=DEAD;
				deletePhysicsObjectForReal();
			}
			else
			{
				/* do nothing */;
			}
			break;
		case WAITING:
			// nothing to be done here...
			break;
		case DEAD:
			// nothing to be done here...
			break;
		}
	}
	DEBUG1("Switched Pingus %p to state %d", this, theState);
	// TODO: update the ViewPingus object for the new state
	theAnimationFrameIndex = 0;
	return theState;
}


void Pingus::reportNormalImpulseLength(qreal anImpulseLength)
{
	// pop the Pingus if it is maltreated
	// switch between falling and walking/sliding where appropriate
//	printf("anImpulseLength: %f\n", anImpulseLength);

	if (anImpulseLength>SPLATTING_IMPULSE)
	{
		goToState(SPLATTING);
		return;
	}
	if (anImpulseLength>0.01 && anImpulseLength<0.1)
	{
		if (theState==FALLING)
			goToState(WALKINGLEFT);
		return;
	}
	goToState(FALLING);
}


void Pingus::switchToSmallShape(void)
{
//	// save the current position - as it is only stored within the B2Body
//	// and we'll kill it in the next line...
//	Position myCurrentPos = getTempCenter();

//	deletePhysicsObjectForReal();
//	clearShapeList();

//	b2PolygonShape* myRestShape = new b2PolygonShape();
//	myRestShape->SetAsBox(0.05, 0.05);
//	b2FixtureDef* myRestDef = new b2FixtureDef();
//	myRestDef->density= 0.001 / (0.1 * 0.1);
//	myRestDef->userData = this;
//	myRestDef->shape   = myRestShape;
//	theShapeList.push_back(myRestDef);

//	CircleObject::createPhysicsObject(myCurrentPos);
}
