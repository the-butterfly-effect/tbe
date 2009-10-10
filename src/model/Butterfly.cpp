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
#include "Flower.h"
#include "DrawButterfly.h"
#include "tbe_global.h"
#include "Box2D.h"


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
			theCountdown--;
			if (theCountdown >0)
				return;
			theCountdown=9;

			// calculate vertical impulse - rate limited
			qreal myDY = theTargetPos.y - getTempCenter().y;
			qreal myYd = theB2BodyPtr->GetLinearVelocity().y;
			qreal myYImpulse = 0.0;
			if (myDY > 0.0)
			{
				myYImpulse = 0.2 * myDY - 0.1*myYd;
			}
			const qreal myYRate = 2.0 * theButterflyMass;
			if (myYImpulse > myYRate)
				myYImpulse = myYRate;
			if (myYImpulse < -myYRate)
				myYImpulse = -myYRate;

			// maintain horizontal speed
			// TODO/FIXME: Butterfly can only fly from left to right
			qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
			qreal myXImpulse = 0;
			if (myXd<0.1)
				myXImpulse = 0.01;

			Position myTotImpulse = aDeltaTime * Position(myXImpulse, myYImpulse);

			theB2BodyPtr->ApplyImpulse( myTotImpulse.toB2Vec2(), getTempCenter().toB2Vec2());

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


void Butterfly::goToFlower(void)
{
	Flower* myFlowerPtr = Flower::getFlowerPtr();
	if (myFlowerPtr == NULL)
		theTargetPos = getOrigCenter();
	else
		theTargetPos = myFlowerPtr->getOrigCenter();
}

void Butterfly::setState(ButterflyStatus aNewStateSuggestion)
{
	theButterflyState = aNewStateSuggestion;
}

void Butterfly::reset(void)
{
	theWorldPtr->registerCallback(this);
	RectObject::reset();
	hasContact = false;
	
	goToFlower();
}
