/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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
#include "tbe_global.h"
#include "Box2D.h"
#include "Property.h"

/// Note: CokeSplatter does not have an ObjectFactory
/// as it cannot be instantiated by a user (neither in-game
/// nor in the level editor)


//// this class' ObjectFactory
class CokeMentosObjectFactory : public ObjectFactory
{
public:
	CokeMentosObjectFactory(void)
	{	announceObjectType("CokeMentosBottle", this); }
	virtual BaseObject* createObject(void) const
	{	return new CokeMentosBottle(); }
};
static CokeMentosObjectFactory theCMBottleObjectFactory;



const double CokeMentosBottle::theBottleMass = 0.2;


CokeMentosBottle::CokeMentosBottle()
		: RectObject()
{
	theToolTipString = QObject::tr("This is a prepared Coke bottle with a Mentos in it.\nLook: it blows if triggered!");
	theNameString = QObject::tr("Coke+Mentos Bottle");
	setTheWidth(0.166);
	setTheHeight(0.501);

	// the first one is the real bottle
	{
		b2PolygonDef* my8PointPinDef = new b2PolygonDef();
		my8PointPinDef->vertexCount = 8;
		my8PointPinDef->vertices[0].Set(-0.01 ,  0.25);
		my8PointPinDef->vertices[1].Set(-0.083,  0.12);
		my8PointPinDef->vertices[2].Set(-0.075, -0.234);
		my8PointPinDef->vertices[3].Set(-0.060, -0.251);
		my8PointPinDef->vertices[4].Set( 0.060, -0.251);
		my8PointPinDef->vertices[5].Set( 0.075, -0.234);
		my8PointPinDef->vertices[6].Set( 0.083,  0.12);
		my8PointPinDef->vertices[7].Set( 0.01 ,  0.25);
		// approximation of the initial mass - we'll fix it later on...
		my8PointPinDef->density = 2.1 / (0.166*0.501);
		my8PointPinDef->friction = 0.0;
		my8PointPinDef->userData = this;
		theShapeList.push_back(my8PointPinDef);
	}

	setTheBounciness(0.2);
	setBottleStatus(UNTRIGGERED);

	theProps.setDefaultPropertiesString(
		Property::IMAGE_NAME_STRING  + QString(":CokeBottleNormal;CokeBottleFoaming;CokeBottleBlowing;CokeBottleEmpty/") +
		Property::MASS_STRING + QString(":2.0/") +
		Property::THRUST_STRING + QString(":2.0/") );
}

void CokeMentosBottle::callbackStep (qreal, qreal)
{
	DEBUG6("coke receives callback\n");

	switch(theBottleStatus)
	{
	case UNTRIGGERED:
		break;
	case TRIGGERED:
		theCountdown--;
		if (theCountdown > 0)
			break;
		setBottleStatus(BLOWING);
		break;
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

void CokeMentosBottle::reportNormalImpulseLength(qreal anImpulseLength)
{
	if (anImpulseLength > 0.6 && theBottleStatus==UNTRIGGERED)
		setBottleStatus(TRIGGERED);
}


void CokeMentosBottle::reset(void)
{
	theWorldPtr->registerCallback(this);
	BaseObject::reset();
	setBottleStatus(UNTRIGGERED);

	theProps.property2Float(Property::THRUST_STRING, &theThrust);
}


void CokeMentosBottle::setBottleStatus(BottleStatus aNewStat)
{
	switch(aNewStat)
	{
	case UNTRIGGERED:
		// allow to set arbitrary illing of the bottle
		theProps.property2Float(Property::MASS_STRING, &theCokeAmount);
		updateMass();
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
	if (theB2BodyPtr==NULL)
		return;

	b2MassData myMass;
	myMass.center = theB2BodyPtr->GetLocalCenter();
	myMass.I      = theB2BodyPtr->GetInertia();
	myMass.mass   = theCokeAmount + theBottleMass;

	theB2BodyPtr->SetMass(&myMass);
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

	// Start position for the splatter is above the opening:
	// i.e. (0, +0.59*object height). The magic number 0.59 is "slightly more than 1/2"...
	// I figured out the number by experiment
	const qreal SPLATTER_START = 0.60;
	Position myStartPos = getTempCenter();
	myStartPos = myStartPos + Vector(0, SPLATTER_START*getTheHeight());
	// we need this 90 degrees because the bottle is modelled vertical,
	// not horizontal
	myStartPos.angle += PI/2.0;

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

	// Small HACK :-)
	// if we're out-of-bounds, let's misuse the thrust to limit the velocity...
	// this will have two effects:
	//   1) the last splatter (giving impulse to a nearly-empty bottle) won't work
	//   2) and using thrust in the wrong direction actually limits speed!
	if (myStartPos.x > 1.4*theWorldPtr->getTheWorldWidth() ||
		myStartPos.y > 1.4*theWorldPtr->getTheWorldHeight())
	{
		if (theThrust>0)
			theThrust = - 0.7;
	}

	// and don't forget Newton's action = -reaction !!!
	qreal myImpulse = mySplatterMass*myV;
	// HACK HACK HACK: to improve the "feeling", we help the impulse a bit here...
	// The Thrust is actually adjustable as a property - default is 2.0 (see reset()).
	qreal myAngle = getTempCenter().angle;
	Vector myImpulseVector = -theThrust*Vector(-myImpulse*sin(myAngle), myImpulse*cos(myAngle));
	theB2BodyPtr->ApplyImpulse(myImpulseVector.toB2Vec2(), myStartPos.toB2Vec2());
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

const qreal CokeSplatter::theRadius = 0.04;


CokeSplatter::CokeSplatter()
		: RectObject()
{
	DEBUG5("CokeSplatter::CokeSplatter\n");
	theProps.setProperty(Property::IMAGE_NAME_STRING, "CokeSplatter");
	theProps.setProperty(Property::DESCRIPTION_STRING, "");

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
						  UNUSED_ARG qreal aSplatterMass)
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
