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

#include "Flower.h"

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
		: RectObject(), theCountdown(1), theJointPtr(NULL)
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
			theCountdown=10;
			DEBUG5("****stationary flapping Butterfly\n");

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


bool Butterfly::setupFlowerJoint(void)
{
	// FIXME: REMOVE
	DEBUG2("****************setupFlowerJoint enter\n");

	Flower* myFlowerPtr = Flower::getFlowerPtr();
	if (myFlowerPtr == NULL)
		return false;
	b2Body* myFlowerBodyPtr = myFlowerPtr->theB2BodyPtr;
	if (myFlowerBodyPtr == NULL)
		return false;

	DEBUG2("*** GOOD: FlowerBody exists\n");

	if (theB2BodyPtr == NULL)
		return false;
	DEBUG2("*** GOOD: ButterflyBody exists\n");

	b2PrismaticJointDef myJointDef;

	Position myAxis = myFlowerPtr->getTempCenter() - getOrigCenter();

//	myJointDef.Initialize(myFlowerBodyPtr, theB2BodyPtr, myFlowerBodyPtr->GetLocalCenter(), myAxis.toB2Vec2());
	myJointDef.Initialize(myFlowerBodyPtr, theB2BodyPtr, b2Vec2(0,0),  myAxis.toB2Vec2());
	myJointDef.lowerTranslation = -3*myAxis.length();
	myJointDef.upperTranslation = 3*myAxis.length();
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = true;
	myJointDef.maxMotorForce = 0.01f;
	myJointDef.motorSpeed = 0.2;	// move 5 cm per second towards the flower (i.e. distance shrinks)

	if (theJointPtr)
		delete theJointPtr;
	theJointPtr = new Joint(&myJointDef,theWorldPtr);

//	b2PrismaticJointDef jp;
//	jp.Initialize(m_ground,m_elev, bd.position, b2Vec2(0.0f, 1.0f));
//	jp.lowerTranslation =  0.0f;
//	jp.upperTranslation = 100.0f;
//	jp.enableLimit = true;
//	jp.enableMotor = true;
//	jp.maxMotorForce = 10000.f;
//	jp.motorSpeed    = 0.f;
//	m_joint_elev = (b2PrismaticJoint*)m_world->CreateJoint(&jp);


	// FIXME: REMOVE
	DEBUG2("****************setupFlowerJoint successfulexit\n");
	return true;
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
	
	if (theJointPtr)
		delete theJointPtr;
	theJointPtr = NULL;
	setupFlowerJoint();
}
