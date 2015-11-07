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


Pingus::Pingus()
		: PolyObject(QObject::tr("Pingus"),
					 QObject::tr("the famous penguin. He walks and believes in you. Keep him alive!"),
					 "Pingus",
					 "(0.02,0.17)=(-0.02,0.17)=(-0.045,0.14)=(-0.04,0.065)=(0.0,0.04)"
					 "=(0.04,0.065)=(0.045,0.14);"
					 "(-0.04,0.02)=(-0.06,-0.04)=(-0.06,-0.11)=(-0.035,-0.17)"
					 "=(0.035,-0.17)=(0.06,-0.11)=(0.06,-0.04)=(0.04,0.02)",
					 0.12, 0.34, 1.5, 0.4 )
{
	theState = WALKINGRIGHT;
}

Pingus::~Pingus()
{
}


void Pingus::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
	DEBUG6("Pingus receives callback");

	switch(theState)
	{
	case WALKINGLEFT:
	case WALKINGRIGHT:
	case FALLING:
	case SLIDELEFT:
	case SLIDERIGHT:
	case SPLATTING:
	case DEAD:
		break;
	}
}


void Pingus::createPhysicsObject(void)
{
	theState = WALKINGRIGHT;
	clearShapeList();
	fillShapeList();
	PolyObject::createPhysicsObject();
	theWorldPtr->registerCallback(this);
}

void Pingus::deletePhysicsObject(void)
{
	// nothing much to do here that actually has to do with delete...
	theState = WALKINGRIGHT;
	clearShapeList();
	fillShapeList();

	PolyObject::deletePhysicsObject();
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

	switch (theState)
	{
	case WALKINGLEFT:
	case WALKINGRIGHT:
	case FALLING:
	case SLIDELEFT:
	case SLIDERIGHT:
	case SPLATTING:
	case DEAD:
		break;
	}
	return theState;
}


void Pingus::reportNormalImpulseLength(qreal anImpulseLength)
{
	// also pop the Pingus if it is maltreated
	// WARNING: Magic number here!!!
//	if (anImpulseLength > 2.1 && theState==Pingus)
//		goToState(POPPING);
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

	PolyObject::createPhysicsObject(myCurrentPos);
}
