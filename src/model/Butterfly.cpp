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
		: RectObject()
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

	setState(STATIONARY_FLAP_OPEN);
}

Butterfly::~Butterfly()
{
	;
}


void Butterfly::callBackSensor(b2ContactPoint*)
{
	hasContact = true;
}

void Butterfly::callbackStep (qreal, qreal)
{
	DEBUG6("Butterfly receives callback\n");

	theCountdown--;
	if (theCountdown >0)
		return;
	theCountdown=6;

	switch(getState())
	{
		case STILL: // not implemented yet
		case DEAD:	// not implemented yet
			// nothing to do
			break;
		case STATIONARY_FLAP_OPEN:
		case STATIONARY_FLAP_HALF:
		{
			Position myDistance = theStationaryPosition - getTempCenter();
			if (getTempCenter().y < theStationaryPosition.y)
			{
				Position myImpulseVector = 0.4 * theButterflyMass * myDistance;
				// FIXME/TODO: limit the maximum impulse
				theB2BodyPtr->ApplyImpulse(myImpulseVector.toB2Vec2(), getTempCenter().toB2Vec2());

				if (getState()==STATIONARY_FLAP_HALF)
					setState(STATIONARY_FLAP_OPEN);
				else
					setState(STATIONARY_FLAP_HALF);
			}
			break;
		}
		case MOTION_FLAP_OPEN:
		case MOTION_FLAP_HALF:
			// nothing to do yet
			break;
	}
}


DrawObject*  Butterfly::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawButterfly(this, getProperty(IMAGE_NAME_STRING));
	return theDrawObjectPtr;
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
	theStationaryPosition = getOrigCenter();
	setState(STATIONARY_FLAP_OPEN);
}
