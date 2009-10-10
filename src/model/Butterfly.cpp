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

#include "Butterfly.h"
#include "DrawButterfly.h"
#include "tbe_global.h"
#include "Box2D.h"


static const char* FLOWER_X = "Flower-X";
static const char* FLOWER_Y = "Flower-Y";


//// this class' ObjectFactory
class ButterflyObjectFactory : public ObjectFactory
{
public:
	ButterflyObjectFactory(void)
	{	announceObjectType("Butterfly", this); }
	virtual BaseObject* createObject(void) const
	{	return new Butterfly(); }
};
static ButterflyObjectFactory theButterflyObjectFactory;



Butterfly::Butterfly()
		: RectObject(), theCountdown(1)
{
	setProperty(IMAGE_NAME_STRING, "Butterfly");
	setProperty(DESCRIPTION_STRING, QObject::tr("lalala FIXME"));
	setProperty(MASS_STRING, QString::number(theButterflyMass));

	// butterflies don't bounce *ever*
	setTheBounciness(0.0);

	// the SVG is 223x339 "pixels"
	setTheWidth(0.15);
	setTheHeight(0.15*339./223.);

	adjustParameters();

	setState(FLAP_OPEN);
}

Butterfly::~Butterfly()
{
	;
}


void Butterfly::callBackSensor(b2ContactPoint*)
{
	hasContact = true;
}

void Butterfly::callbackStep (qreal aDeltaTime, qreal)
{
	DEBUG6("Butterfly receives callback\n");

	switch(getState())
	{
		case STILL: // not implemented yet
		case DEAD:	// not implemented yet
			// nothing to do
			break;
		case FLAP_OPEN:
		case FLAP_HALF:
		{
			// always apply damping in X direction - linear and opposite to speed
			b2Vec2 mySpeed = theB2BodyPtr->GetLinearVelocity();
			mySpeed.y = 0;
			theB2BodyPtr->ApplyForce( -0.7*theButterflyMass*mySpeed, getTempCenter().toB2Vec2());

			theCountdown--;
			if (theCountdown >0)
				return;
			theCountdown=10;
			DEBUG5("****stationary flapping Butterfly\n");

			// if we are moving, how far are we?
			// i.e. if the butterfly should fly up/down


			// calculate vertical impulse - rate limited
			qreal myDY = theTargetPos.y - getTempCenter().y;
			qreal myYImpulse = 0.0;
			if (myDY > 0.0)
			{
				myYImpulse = 100 * theButterflyMass * myDY;
				// FIXME/TODO: rate limit this impulse
			}
			const qreal myYRate = 0.90*theButterflyMass;
			if (myYImpulse > myYRate)
				myYImpulse = myYRate;
			if (myYImpulse < -myYRate)
				myYImpulse = -myYRate;


			// we already know the horizontal impulse :-)
			qreal myDX= theTargetPos.x - getTempCenter().x;
			qreal myXImpulse = theHorizontalImpulsePerSecond;
			if (myDX < 0.0)
				myXImpulse *= -1.0;
//			const qreal myXRate = 0.015;
//			if (myXImpulse > myXRate)
//				myXImpulse = myXRate;
//			if (myXImpulse < -myXRate)
//				myXImpulse = -myXRate;

			// rate limit thrust
			// you cannot deliver both Y and X thrust
			// (i.e. remove impulse in same direction of speed)
			b2Vec2 myVelo = theB2BodyPtr->GetLinearVelocity();
			if (myVelo.y < 0.0)
				myVelo.y = 0;
			if (myVelo.Length() > 0.10)
				myXImpulse /= 2.0;

			Position myTotImpulse = Position(myXImpulse, myYImpulse);

printf("deltax:%f, dx:%f, impulse: %f,%f\n", myDX, myVelo.x,
	   myTotImpulse.x, myTotImpulse.y);

			theB2BodyPtr->ApplyImpulse( (aDeltaTime*myTotImpulse).toB2Vec2(),
									   getTempCenter().toB2Vec2());

			// and flap
			if (getState()==FLAP_HALF)
				setState(FLAP_OPEN);
			else
				setState(FLAP_HALF);
			break;
		}
	}
}


DrawObject*  Butterfly::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawButterfly(this, getProperty(IMAGE_NAME_STRING));
	return theDrawObjectPtr;
}


bool Butterfly::goToFlower(void)
{
	printf("****************goToFlower enter\n");

	// if there is no such setting, we'll have position (0.0)
	float myFlowerX = getProperty(FLOWER_X).toFloat();
	float myFlowerY = getProperty(FLOWER_Y).toFloat();
	if (myFlowerX == 0.0 && myFlowerY == 0.0)
		theTargetPos = getOrigCenter();
	else
		theTargetPos = Position(myFlowerX, myFlowerY);


	Position myDistance = getOrigCenter() - theTargetPos;

	// Let's limit a butterfly to a maximum horizontal speed of 10cm/second
	if (myDistance.length() == 0.0)
		theHorizontalImpulsePerSecond = 0.0;
	else
		theHorizontalImpulsePerSecond = 10.0*theButterflyMass/(myDistance.length()/0.10) * myDistance.length();

printf("****************goToFlower succeeded\n");
	return true;
}

void Butterfly::setState(ButterflyStatus aNewStateSuggestion)
{
	theButterflyState = aNewStateSuggestion;
}

void Butterfly::reset(void)
{
	theWorldPtr->registerCallback(this);
	BaseObject::reset();
	hasContact = false;
	
	goToFlower();
}
