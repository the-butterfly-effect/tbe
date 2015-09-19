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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "Butterfly.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "Property.h"

//// this class' ObjectFactory
class ButterflyObjectFactory : public ObjectFactory
{
public:
	ButterflyObjectFactory(void)
	{	announceObjectType("Butterfly", this); }
    virtual AbstractObject* createObject(void) const
	{	return fixObject(new Butterfly()); }
};
static ButterflyObjectFactory theButterflyObjectFactory;




const double Butterfly::theButterflyMass = 0.01;


Butterfly::Butterfly()
        : RectObject(), theCountdown(1)
{
    theToolTip = QObject::tr("Butterfly (Flappus Chaoticus Fragilius) - always in search of flowers.");
	theProps.setDefaultPropertiesString(
			Property::OBJECT_STRING + QString(":/") +
			Property::BOUNCINESS_STRING + QString(":0/") +
			Property::IMAGE_NAME_STRING  + QString(":ButterflyStill;ButterflyOpen;ButterflyHalfOpen;ButterflyStill/") +
			Property::MASS_STRING
				+ ":" + QString::number(theButterflyMass) + "/");

	// butterflies don't bounce *ever*
	setTheBounciness(0.0);

	// the SVG is 223x339 "pixels"
	setTheWidth(0.15);
	setTheHeight(0.15*339./223.);

	setState(FLAP_OPEN);
}

Butterfly::~Butterfly()
{
	;
}


void Butterfly::callbackStep (qreal aDeltaTime, qreal)
{
	DEBUG6("Butterfly receives callback");

	switch(getState())
	{
		case STILL: // not implemented yet
			break;
		case DEAD:
			// nothing to do, we just leave gravity take its course

			// FIXME: it would be nice if the butterfly would land on its back
			// This should be doable by moving the center of the mass to the back.

            theWorldPtr->objectDied(getThisPtr());
			break;
		case FLAP_OPEN:
		case FLAP_HALF:
		{
			// remain in either FLAP position for a while
			// (i.e. 25 frames right now)
			theCountdown--;
			if (theCountdown > 0)
				return;
			theCountdown=25;

			// calculate vertical flapping impulse - rate limited
			qreal myDY = theTargetPos.y - getTempCenter().y;
			qreal myYd = theB2BodyPtr->GetLinearVelocity().y;
			qreal myYImpulse = 4 * myDY - 4.8*myYd;

			if (myYImpulse < 0.0)
				myYImpulse = 0;

			// maintain horizontal speed
			// TODO/FIXME: Butterfly can only fly from left to right
			// see also Ticket:67
			qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
			qreal myXImpulse = 0;
			if (myXd<0.1)
				myXImpulse = 0.025;

			// the pull to the flower is done at the butterfly's center
			// whilst the push upwards is done slightly above the center
			// this allows the butterfly to tilt when needed, but it will
			// correct itself later upwards again (similar to Balloon)
			Vector myTotXImpulse = aDeltaTime * Vector(myXImpulse, 0);
			Vector myTotYImpulse = aDeltaTime * Vector(0, myYImpulse);
			theB2BodyPtr->ApplyLinearImpulse(
                    myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);
			Position myPoint = getTempCenter() + Vector(0,0.01);
			theB2BodyPtr->ApplyLinearImpulse(
                    myTotYImpulse.toB2Vec2(), myPoint.toB2Vec2(), true);

			// and flap
			if (getState()==FLAP_HALF)
				setState(FLAP_OPEN);
			else
				setState(FLAP_HALF);
			break;
		}
	}
}


void Butterfly::goToFlower(void)
{
    AbstractObjectPtr myFlowerPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT_STRING);
    if (myFlowerPtr==nullptr)
		theTargetPos = getOrigCenter();
	else
		theTargetPos = myFlowerPtr->getOrigCenter();
}


void Butterfly::reportNormalImpulseLength(qreal anImpulseLength)
{
	if(anImpulseLength<0.008)
		return;
	setState(DEAD);
}


void Butterfly::setState(ButterflyStatus aNewStateSuggestion)
{
	theButterflyState = aNewStateSuggestion;
}

void Butterfly::createPhysicsObject(void)
{
	setState(FLAP_OPEN);
	theWorldPtr->registerCallback(this);
	goToFlower();
	RectObject::createPhysicsObject();
}
