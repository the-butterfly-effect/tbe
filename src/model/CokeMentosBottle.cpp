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

	b2PolygonDef* my5PointPinDef = new b2PolygonDef();
	my5PointPinDef->vertexCount = 5;
	my5PointPinDef->vertices[0].Set( 0    ,  0.25);
	my5PointPinDef->vertices[1].Set(-0.083,  0.12);
	my5PointPinDef->vertices[2].Set(-0.075, -0.251);
	my5PointPinDef->vertices[3].Set( 0.075, -0.251);
	my5PointPinDef->vertices[4].Set( 0.083,  0.12);
	// approximation of the initial mass - we'll fix it later on...
	my5PointPinDef->density = 2.1 / (0.166*0.501);
	
	// delete any shapes on the body
	// and create a new shape from the above polygon def
	theShapeList.push_back(my5PointPinDef);
	setTheBounciness(0.3);
	setBottleStatus(UNTRIGGERED);
}

void CokeMentosBottle::callbackStep (qreal aTimeStep, qreal aTotalTime)
{
	DEBUG5("coke receives callback\n");

	// this is just a simple delay/rate limiter
	static int myBlowcount = 0;
	myBlowcount++;
	if (myBlowcount > 50 && ((myBlowcount%4)==0))
	{
		static int mySplatterCount = 0;
		newSplatter(mySplatterCount++);
	}
}

void CokeMentosBottle::reset(void)
{
	theWorldPtr->registerCallback(this);
	BaseObject::reset();
}


void CokeMentosBottle::setBottleStatus(BottleStatus aNewStat)
{
	switch(aNewStat)
	{
	case UNTRIGGERED:
		theCokeAmount = 2.0;
		break;
	case TRIGGERED:
		break;
	case BLOWING:
		break;
	case EMPTY:
		break;
	}
}

void CokeMentosBottle::updateMass(void)
{
	b2MassData myMass;
	myMass.center = theB2BodyPtr->GetLocalCenter();
	myMass.I      = theB2BodyPtr->GetInertia();
	myMass.mass   = theCokeAmount + 0.1;
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

	// start position for the splatter is above the opening: (0, +0.53*object height)
	Position myStartPos = getTempCenter();

	// the bottle position contains the angle, we need that to compensate for
	// rotated bottles
	qreal myAngle = myStartPos.angle;
	myStartPos = myStartPos + Position(getTheHeight()*-0.53*sin(myAngle), getTheHeight()*0.53*cos(myAngle), PI/2.0);

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
	const qreal mySplatterMass = 0.01;
	mySplatter->setAll(theWorldPtr, myStartPos, myV, mySplatterMass);

	theCokeAmount -= mySplatterMass;
	updateMass();

	// TODO: don't forget Newton's action = -reaction;
	//theB2BodyPtr->ApplyImpulse();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

CokeSplatter::CokeSplatter()
		: RectObject(), isResetForReal(false)
{
	DEBUG5("CokeSplatter::CokeSplatter\n");
	setProperty(IMAGE_NAME_STRING, "CokeSplatter");
	setProperty(DESCRIPTION_STRING, "");

	b2CircleDef* myBubbleDef = new b2CircleDef();
	myBubbleDef->radius = theRadius;
	myBubbleDef->density= 0.01 / (PI * theRadius * theRadius);

	theShapeList.push_back(myBubbleDef);
	setTheBounciness(0.0);
	setTheWidth(2*theRadius);
	setTheHeight(2*theRadius);
}

CokeSplatter::~CokeSplatter()
{
}

void CokeSplatter::reset()
{
	DEBUG5("reset() body pos for 'CokeSplatter' %p\n", this);
	if(isResetForReal==false)
		return;
	assert(false);
	deregister();

	// we need this, but let's not - for now
	// delete this;
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
	aWorldPtr->addObject(this);
	createPhysicsObject();

	qreal myAngle = aStartPos.angle;
	b2Vec2 myVelVec(aVelocity * cos(myAngle), aVelocity * sin(myAngle));
	DEBUG5("velocity: %f,%f\n", myVelVec.x, myVelVec.y);
	theB2BodyPtr->SetLinearVelocity(myVelVec);

	isResetForReal = true;

	// FIXME: variable mass is ignored for now...
}
