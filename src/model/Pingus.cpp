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
	virtual AbstractObject* createObject(void) const
	{	return fixObject(new Pingus()); }
};
static PingusObjectFactory thePingusObjectFactory;


static const qreal PINGUS_RADIUS  = 0.16; // m
static const qreal PINGUS_MASS    = 1.50; // kg
static const qreal SPLATTING_TIME = 0.16; // seconds
static const qreal WALKING_SPEED  = 0.30; // m/s
static const int WALKINGSEQS_PER_SECOND = 1; // one sequence per second for now

const unsigned int Pingus::FramesPerState[] = { 8, 8, 8, 1, 1, 16, 1};


Pingus::Pingus()
		: CircleObject(QObject::tr("Pingus"),
					 QObject::tr("the famous penguin. He walks and believes in you. Keep him alive!"),
					 "",
					 PINGUS_RADIUS, PINGUS_MASS, 0.4 )
{
	theState = WALKINGRIGHT;
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
		break;
	case SLIDELEFT:
	case SLIDERIGHT:
		callbackStepSliding(aDeltaTime, aTotalTime);
		break;
	case SPLATTING:
		callbackStepSplatting(aDeltaTime, aTotalTime);
		break;
	case DEAD:
		// nothing to do
		break;
	}

	static_cast<ViewPingus*>(theViewObjectPtr)->setNewAnimationFrame(theState, theAnimationFrameIndex);
}


void Pingus::callbackStepSliding(qreal, qreal aTotalTime)
{
	// * check for the horizontal speed
	//   - if the speed is below walking, switch to walking
	// * if we have a sufficient vertical component, we're falling
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

	Q_ASSERT(theAnimationFrameIndex > 0);

	// are we done?
	if (aTotalTime-theSplattingTimeStart > SPLATTING_TIME)
		goToState(DEAD);
}


void Pingus::callbackStepWalking(qreal, qreal aTotalTime)
{
	// * check for the horizontal speed and adjust it if needed
	//   by applying a force
	//   - if the force becomes too big, turn around
	//   - if the force is sufficiently negative, switch to sliding
	// * if we have a sufficient vertical component, we're falling
	// * set theAnimationFrameIndex to the horizontal position
	qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
	qreal myXImpulse = 0;
	if (myXd < WALKING_SPEED)
		myXImpulse = 0.025;

	// in WALKING_SPEED [m/s], we have WALKINGSEQS_PER_SECOND*Pingus::FramesPerState[WALKINGLEFT] animation frames to draw
	// i.e. first modulo by WALKING SPEED/WALKINGSEQS_PER_SECOND and normalize to 0-1, then divide by number of frames
	qreal temp = fmodf(theB2BodyPtr->GetPosition().x, (WALKING_SPEED)) / WALKING_SPEED;
	printf("temp: %f  ", temp);
	theAnimationFrameIndex = temp * static_cast<qreal>(Pingus::FramesPerState[WALKINGLEFT]);
	printf("theAnimationFrameIndex: %d\n", theAnimationFrameIndex);
}


void Pingus::createPhysicsObject(void)
{
	theState = WALKINGRIGHT;
	theSplattingTimeStart = -1.0;
	createBallShapeFixture(PINGUS_RADIUS, PINGUS_MASS);
	CircleObject::createPhysicsObject();
	theWorldPtr->registerCallback(this);
}


ViewObject*  Pingus::createViewObject(float aDefaultDepth)
{
	assert(theViewObjectPtr==NULL);
	theViewObjectPtr = new ViewPingus(getThisPtr());
	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	return theViewObjectPtr;
}


void Pingus::deletePhysicsObject(void)
{
	// nothing much to do here that actually has to do with delete...
	theState = WALKINGRIGHT;
	theSplattingTimeStart = -1.0;
	clearShapeList();

	CircleObject::deletePhysicsObject();
}

void Pingus::deletePhysicsObjectForReal(void)
{
	updateViewObject(false);
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = NULL;
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
			break;
		case WALKINGRIGHT:
			break;
		case FALLING:
			break;
		case SLIDELEFT:
			break;
		case SLIDERIGHT:
			break;
		case SPLATTING:
			break;
		case DEAD:
			// nothing to be done here...
			break;
		}
	}
	DEBUG1("Switched Pingus to state %d", theState);
	// TODO: update the ViewPingus object for the new state
	theAnimationFrameIndex = 0;
	return theState;
}


void Pingus::reportNormalImpulseLength(qreal anImpulseLength)
{
	// also pop the Pingus if it is maltreated
	// WARNING: Magic number here!!!
//	if (anImpulseLength > 2.1 && theState==Pingus)
//		goToState(Splatting);
}


void Pingus::switchToSmallShape(void)
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

	CircleObject::createPhysicsObject(myCurrentPos);
}
