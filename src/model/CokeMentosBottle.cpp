/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#include "CokeMentosBottle.h"
#include "DrawCokeMentosBottle.h"
#include "tbe_global.h"
#include "Box2D.h"

/// Note: CokeSplatter does not have an ObjectFactory
/// as it cannot be instantiated by a user (neither in-game
/// nor in the level editor)


//// this class' ObjectFactory
class CokeMentosObjectFactory : public ObjectFactory
{
public:
	CokeMentosObjectFactory(void)
	{	announceObjectType("Coke+Mentos Bottle", this); }
	virtual BaseObject* createObject(void) const
	{	return new CokeMentosBottle(); }
};
static CokeMentosObjectFactory theCMBottleObjectFactory;



CokeMentosBottle::CokeMentosBottle()
		: RectObject()
{
	setProperty(IMAGE_NAME_STRING, "CokeMentosBottle");
	setProperty(DESCRIPTION_STRING, QObject::tr("This is a prepared Coke bottle with a Mentos in it.\nLook: it blows if triggered!"));
	setTheWidth(0.166);
	setTheHeight(0.501);

	// the first one is the real bottle
	{
		b2PolygonDef* my5PointPinDef = new b2PolygonDef();
		my5PointPinDef->vertexCount = 6;
		my5PointPinDef->vertices[0].Set(-0.01 ,  0.25);
		my5PointPinDef->vertices[1].Set(-0.083,  0.12);
		my5PointPinDef->vertices[2].Set(-0.075, -0.251);
		my5PointPinDef->vertices[3].Set( 0.075, -0.251);
		my5PointPinDef->vertices[4].Set( 0.083,  0.12);
		my5PointPinDef->vertices[5].Set( 0.01 ,  0.25);
		// approximation of the initial mass - we'll fix it later on...
		my5PointPinDef->density = 2.1 / (0.166*0.501);
		my5PointPinDef->friction = 0.0;
		theShapeList.push_back(my5PointPinDef);
	}

	// the second one is the sensor
	{
		b2PolygonDef* my5PointPinDef = new b2PolygonDef();
		my5PointPinDef->vertexCount = 6;
		my5PointPinDef->vertices[0].Set(-0.01 ,  0.25);
		my5PointPinDef->vertices[1].Set(-0.083,  0.12);
		my5PointPinDef->vertices[2].Set(-0.075, -0.251);
		my5PointPinDef->vertices[3].Set( 0.075, -0.251);
		my5PointPinDef->vertices[4].Set( 0.083,  0.12);
		my5PointPinDef->vertices[5].Set( 0.01 ,  0.25);
		my5PointPinDef->isSensor = true;
		my5PointPinDef->userData = this;
		theShapeList.push_back(my5PointPinDef);
	}

	setTheBounciness(0.3);
	setBottleStatus(UNTRIGGERED);
}

void CokeMentosBottle::callBackSensor(b2ContactPoint*)
{
	hasContact = true;
}

void CokeMentosBottle::callbackStep (qreal, qreal)
{
	DEBUG6("coke receives callback\n");

	switch(theBottleStatus)
	{
	case UNTRIGGERED:
		{
			//  check for impact
			b2Vec2 myVelo = theB2BodyPtr->GetLinearVelocity();
			float  myAVelo= theB2BodyPtr->GetAngularVelocity();
			if (myVelo.Length()<0.1 && myAVelo <0.1)
				break;
			if ( abs(1.0-thePreviousVelocity.Length()/myVelo.Length()) > 0.2 && hasContact)
			{
				DEBUG5("CokeBottle was hit - LINEAR\n");
				setBottleStatus(TRIGGERED);
				break;
			}
			if ( abs(1.0-thePreviousAngVelocity/myAVelo) > 0.2 && hasContact)
			{
				DEBUG5("CokeBottle was hit - ANGULAR\n");
				setBottleStatus(TRIGGERED);
				break;
			}
			thePreviousVelocity = myVelo;
			thePreviousAngVelocity = myAVelo;
			hasContact = false;
			break;
		}
	case TRIGGERED:
		theCountdown--;
		if (theCountdown > 0)
			break;
		setBottleStatus(BLOWING);
	case BLOWING:
		// this is just a rate limiter
		theCountdown--;
		if (((theCountdown%3)==0))
		{
			newSplatter(theSplatterCount++);
		}
		break;
	case EMPTY:
		break;
	}


}


DrawObject*  CokeMentosBottle::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawCokeMentosBottle(this, getProperty(IMAGE_NAME_STRING));
	return theDrawObjectPtr;
}


void CokeMentosBottle::reset(void)
{
	theWorldPtr->registerCallback(this);
	BaseObject::reset();
	setBottleStatus(UNTRIGGERED);
	thePreviousVelocity = b2Vec2(0,0);
	thePreviousAngVelocity = 0.0;
	hasContact = false;
}


void CokeMentosBottle::setBottleStatus(BottleStatus aNewStat)
{
	switch(aNewStat)
	{
	case UNTRIGGERED:
		theCokeAmount = 2.0;
		theSplatterCount = 0;
		theBottleStatus=UNTRIGGERED;
		break;
	case TRIGGERED:
		theBottleStatus=TRIGGERED;
		theCountdown = 100;
		break;
	case BLOWING:
		theBottleStatus=BLOWING;
		break;
	case EMPTY:
		theBottleStatus=EMPTY;
		break;
	}
}

void CokeMentosBottle::updateMass(void)
{
	if (theCokeAmount < 0.0)
	{
		theCokeAmount = 0.0;
		setBottleStatus(EMPTY);
	}
	b2MassData myMass;
	myMass.center = theB2BodyPtr->GetLocalCenter();
	myMass.I      = theB2BodyPtr->GetInertia();
	myMass.mass   = theCokeAmount + theBottleMass;
	theB2BodyPtr->SetMass(&myMass);

	// test
	myMass.mass = 0;
}

void CokeMentosBottle::newSplatter(unsigned int aSequenceNr)
{
	// the first splatter is to reach 1.5m height, improving to 2.5m with the 20th
	// then deteriorating quickly - until none left
	//
	// for simplicity, we assume that a horizontal bottle has the same
	// exit velocity and such...
	//
	// the opening exit velocity can be calculated using energy:  m*g*h = 0.5*m*v*v
	// i.e.  v = sqrt(2*g*h)    <- note that the mass of the splatter is not in there

	// so we need a 2nd order equation that starts at (0, 1.5), reaches (20,2.5)
	// and then decays until (40, 1.5) again.
	// then we use a linear equation for the remainder
	//
	//   h=height, n=splatternumber
	// i.e. h = -1/400*n*n + 1/10*n + 1.5
	// and from 40 onwards:
	//      h = 1.5 - 3*(n-40)/220
	//
	// TODO/FIXME: there is a discontinuity in h' - that can be fixed by calculating
	// the point where the extension of h' goes through (150,0) -> for later :-)

	CokeSplatter* mySplatter = new CokeSplatter();

	// start position for the splatter is above the opening: (0, +0.59*object height)
	Position myStartPos = getTempCenter();

	// the bottle position contains the angle, we need that to compensate for
	// rotated bottles
	qreal myAngle = myStartPos.angle;
	myStartPos = myStartPos + Position(getTheHeight()*-0.59*sin(myAngle), getTheHeight()*0.59*cos(myAngle), PI/2.0);

	// departure velocity
	qreal myH;
	if (aSequenceNr<40)
		myH = (-1/400)*aSequenceNr*aSequenceNr + 0.1*aSequenceNr + 1.5;
	else
		myH = 1.5 - 3*(aSequenceNr-40)/220;

	// if the expected height is less than 0, we're no longer blowing
	if (myH <0.0)
		return;

	// note: hardcoded constant for G here, because we're using constants from the
	// normal world to calculate it - not related to the G in a alternative world.
	// (otherwise, the coke bottle would blow as high on the moon as on earth)
	qreal myV = sqrt(2.0 * 9.81 * myH);
	const qreal mySplatterMass = 0.015;
	mySplatter->setAll(theWorldPtr, myStartPos, myV, mySplatterMass);

	theCokeAmount -= mySplatterMass;
	updateMass();

	// and don't forget Newton's action = -reaction !!!
	qreal myImpulse = mySplatterMass*myV;
	// FIXME: to improve the "feeling", we help the impulse a bit here...
	Position myImpulseVector = -2.0*Position(-myImpulse*sin(myAngle), myImpulse*cos(myAngle), 0.0);
	theB2BodyPtr->ApplyImpulse(myImpulseVector.toB2Vec2(), myStartPos.toB2Vec2());
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

CokeSplatter::CokeSplatter()
		: RectObject()
{
	DEBUG5("CokeSplatter::CokeSplatter\n");
	setProperty(IMAGE_NAME_STRING, "CokeSplatter");
	setProperty(DESCRIPTION_STRING, "");

	// the actual shape
	b2CircleDef* myBubbleDef = new b2CircleDef();
	myBubbleDef->radius = theRadius;
	myBubbleDef->density= 0.01 / (PI * theRadius * theRadius);
	theShapeList.push_back(myBubbleDef);

	// the sensor - slightly larger
	// (sensor is used for collision detection & getting rid of the object if needed)
	b2CircleDef* mySensorDef = new b2CircleDef();
	mySensorDef->radius = 1.01 * theRadius;
	mySensorDef->isSensor = true;
	mySensorDef->userData = this;
	theShapeList.push_back(mySensorDef);

	setTheBounciness(0.0);
}

CokeSplatter::~CokeSplatter()
{
	DEBUG5("CokeSplatter::~CokeSplatter()\n");
}


void CokeSplatter::callBackSensor(b2ContactPoint*)
{
	// oww we hit something.
	// that's the end for us
	//   - just allow for some time to transfer the impact
	theWorldPtr->removeMe(this, 0.1);
}


void CokeSplatter::setAll(World* aWorldPtr,
						  const Position& aStartPos,
						  qreal aVelocity,
						  qreal aSplatterMass)
{
	DEBUG5("CokeSplatter::setAll(%p, (%f,%f,%f), %f, %f)\n",
		   aWorldPtr, aStartPos.x, aStartPos.y, aStartPos.angle,
		   aVelocity, aSplatterMass);

	setOrigCenter(aStartPos);
	// the displayed image is larger than the actual object
	setTheWidth(5*theRadius);
	setTheHeight(3.5*theRadius);
	aWorldPtr->addObject(this);
	createPhysicsObject();


	qreal myAngle = aStartPos.angle;
	b2Vec2 myVelVec(aVelocity * cos(myAngle), aVelocity * sin(myAngle));
	DEBUG5("velocity: %f,%f\n", myVelVec.x, myVelVec.y);
	theB2BodyPtr->SetLinearVelocity(myVelVec);

	// FIXME: variable mass is ignored for now...
}
