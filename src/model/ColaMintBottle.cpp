/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012  Klaas van Gend
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

#include "Box2D.h"
#include "ColaMintBottle.h"
#include "ObjectFactory.h"
#include "Property.h"
#include "tbe_global.h"

/// Note: ColaSplatter does not have an ObjectFactory
/// as it cannot be instantiated by a user (neither in-game
/// nor in the level editor)


//// this class' ObjectFactory
class ColaMintObjectFactory : public ObjectFactory
{
public:
	ColaMintObjectFactory(void)
	{	announceObjectType("ColaMintBottle", this); }
    virtual AbstractObject* createObject(void) const
	{	return fixObject(new ColaMintBottle()); }
};
static ColaMintObjectFactory theCMBottleObjectFactory;



const double ColaMintBottle::theBottleMass = 0.2;
static const float BOTTLEWIDTH=0.166;
static const float BOTTLEHEIGHT=0.50;


ColaMintBottle::ColaMintBottle()
		: PolyObject(QObject::tr("Cola+Mint Bottle"),
					 QObject::tr("This is a prepared cola bottle with a mint in it.\nLook: it blows if triggered!"),
					 "ColaBottleNormal;ColaBottleFoaming;ColaBottleBlowing;ColaBottleEmpty",
					 "(-0.01,0.25)=(-0.083,0.12)=(-0.075,-0.234)=(-0.060,-0.25)"
					 "=(0.060,-0.25)=(0.075,-0.234)=(0.083,0.12)=(0.01,0.25)",
					 BOTTLEWIDTH, BOTTLEHEIGHT, theBottleMass+1.5, 0.2)
{
	theColaAmount = 0.0;
	setBottleStatus(UNTRIGGERED);
	theProps.setDefaultPropertiesString(
		Property::THRUST_STRING + QString(":2.0/") );
}

void ColaMintBottle::callbackStep (qreal, qreal)
{
	DEBUG6ENTRY;
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

void ColaMintBottle::createPhysicsObject(void)
{
	setBottleStatus(UNTRIGGERED);
	theProps.property2Float(Property::THRUST_STRING, &theThrust);
	PolyObject::createPhysicsObject();
	theWorldPtr->registerCallback(this);
}

void ColaMintBottle::deletePhysicsObject(void)
{
	setBottleStatus(UNTRIGGERED);
	PolyObject::deletePhysicsObject();
}

void ColaMintBottle::reportNormalImpulseLength(qreal anImpulseLength)
{
	if (anImpulseLength > 0.6 && theBottleStatus==UNTRIGGERED)
		setBottleStatus(TRIGGERED);
}

void ColaMintBottle::setBottleStatus(BottleStatus aNewStat)
{
	switch(aNewStat)
	{
	case UNTRIGGERED:
		// allow to set arbitrary 'volume' of the fluid in the bottle
		// it doesn't afffect the size, though - magic :-D
		theProps.property2Float(Property::MASS_STRING, &theColaAmount);
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

void ColaMintBottle::updateMass(void)
{
	if (theColaAmount < 0.0)
	{
		theColaAmount = 0.0;
		setBottleStatus(EMPTY);
	}
	if (theB2BodyPtr==NULL)
		return;

	b2MassData myMass;
	myMass.center = theB2BodyPtr->GetLocalCenter();
	myMass.I      = theB2BodyPtr->GetInertia();
	myMass.mass   = theColaAmount + theBottleMass;

	theB2BodyPtr->SetMassData(&myMass);
}

void ColaMintBottle::newSplatter(unsigned int aSequenceNr)
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

    std::shared_ptr<ColaSplatter> mySplatterPtr =
            ObjectFactory::createChildObject<ColaSplatter>();

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
	// (otherwise, the cola bottle would blow as high on the moon as on earth)
	qreal myV = sqrt(2.0 * 9.81 * myH);
	const qreal mySplatterMass = 0.015;
    mySplatterPtr->setAll(theWorldPtr, myStartPos, myV, mySplatterMass);
    theWorldPtr->addObject(mySplatterPtr);

	theColaAmount -= mySplatterMass;
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
	// The Thrust is actually adjustable as a property
	// - default is 2.0 (see createPhysicsObject()).
	qreal myAngle = getTempCenter().angle;
	Vector myImpulseVector = -theThrust*Vector(-myImpulse*sin(myAngle), myImpulse*cos(myAngle));
    theB2BodyPtr->ApplyLinearImpulse(myImpulseVector.toB2Vec2(), myStartPos.toB2Vec2(), true);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

const qreal ColaSplatter::theRadius = 0.04;

ColaSplatter::ColaSplatter()
		: CircleObject("ColaSplatter","", "ColaSplatter",
					   theRadius, 0.01,  0.0), hasRequestedRemoval(false)
{
	DEBUG5ENTRY;
}

ColaSplatter::~ColaSplatter()
{
	DEBUG5ENTRY;
	// contrary to most objects, we need to take ourselves really out of the
	// physics simulation...
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
}

void ColaSplatter::reportNormalImpulseLength(qreal)
{
	// oww we hit something.
	// that's the end for us
	//   - just allow for some time to transfer the impact
	if (!hasRequestedRemoval)
	{
        theWorldPtr->removeMe(getThisPtr(), 0.1);
		hasRequestedRemoval = true;
	}
}


void ColaSplatter::setAll(World* aWorldPtr,
						  const Position& aStartPos,
						  qreal aVelocity,
						  UNUSED_ARG qreal aSplatterMass)
{
	DEBUG5("ColaSplatter::setAll(%p, (%f,%f,%f), %f, %f)\n",
		   aWorldPtr, aStartPos.x, aStartPos.y, aStartPos.angle,
		   aVelocity, aSplatterMass);

	setOrigCenter(aStartPos);
	// the displayed image is larger than the actual object
	setTheWidth(5*theRadius);
	setTheHeight(3.5*theRadius);

	qreal myAngle = aStartPos.angle;
	b2Vec2 myVelVec(aVelocity * cos(myAngle), aVelocity * sin(myAngle));
	DEBUG5("velocity: %f,%f\n", myVelVec.x, myVelVec.y);
	createPhysicsObject();
	theB2BodyPtr->SetLinearVelocity(myVelVec);
    // FIXME: variable ASplatterMass is ignored for now...
    // (but right now, it's constant anyway)
}
