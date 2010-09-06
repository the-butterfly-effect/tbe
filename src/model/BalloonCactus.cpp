/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "BalloonCactus.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "Property.h"

//// this class' ObjectFactory
class BalloonObjectFactory : public ObjectFactory
{
public:
	BalloonObjectFactory(void)
	{	announceObjectType("Balloon", this); }
	virtual BaseObject* createObject(void) const
	{	return new Balloon(); }
};
static BalloonObjectFactory theBalloonObjectFactory;


Balloon::Balloon()
		: PolyObject()
{
	// no special properties
	theProps.setDefaultPropertiesString(
		Property::IMAGE_NAME_STRING + QString(":Balloon/") +
		Property::MASS_STRING + ":0.1/" +
		Property::POLYGONS_STRING + ":"
	"(-0.018,0.18)=(-0.07,0.16)=(-0.12,0.1)=(-0.13,0.017)=(-0.1,-0.08)"
	"=(-0.03,-0.16)=(0.006,-0.17)=(0.039,-0.16)=(0.10,-0.08)"
	"=(0.13,0.015)=(0.11,0.11)=(0.07,0.16)=(0.01,0.18)" + "/");

	BaseObject::setTheWidth(0.27);
	BaseObject::setTheHeight(0.36);

	// balloons bounce very well
	setTheBounciness(0.7);

	fillShapeList();
}

Balloon::~Balloon()
{
	;
}


void Balloon::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
	DEBUG6("Balloon receives callback\n");

	// the upward force...
	theB2BodyPtr->ApplyForce(b2Vec2(0,0.1), (getTempCenter()+Vector(0,0.1)).toB2Vec2());

	// the downward force...
	// i.e. the "drag" or "air restance"
	if (aDeltaTime >= aTotalTime)
	{
		// on the first call, set the previous position
		thePreviousPosition = getTempCenter();
	}

	Vector mySpeedVector = (getTempCenter().toVector() - thePreviousPosition.toVector());
	float myForce = -0.05 * (mySpeedVector.dx*mySpeedVector.dx + mySpeedVector.dy*mySpeedVector.dy)/aDeltaTime/aDeltaTime;

	if (mySpeedVector.length()/aDeltaTime < 0.01)
		myForce = 0;

	Vector myForceVector = myForce * Vector(mySpeedVector.toAngle());
	theB2BodyPtr->ApplyForce(myForceVector.toB2Vec2(), (getTempCenter()).toB2Vec2());
//	printf("speed: %f@%f / force: %f\n", mySpeedVector.length()/aDeltaTime,
//		   mySpeedVector.toAngle(), myForceVector.length());

	thePreviousPosition = getTempCenter();
}



void Balloon::reportNormalImpulseLength(qreal anImpulseLength)
{
}


void Balloon::reset(void)
{
	theWorldPtr->registerCallback(this);
	PolyObject::reset();
//	thePreviousPosition = getOrigCenter();
}


void Balloon::setTheWidth (qreal)
{
	// deliberately empty
	return;
}

void Balloon::setTheHeight (qreal)
{
	// deliberately empty
	return;
}


///---------------------------------------------------------------------------
///------------------------ Cactus -------------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
class CactusObjectFactory : public ObjectFactory
{
public:
	CactusObjectFactory(void)
	{	announceObjectType("Cactus", this); }
	virtual BaseObject* createObject(void) const
	{	return new Cactus(); }
};
static CactusObjectFactory theCactusObjectFactory;


Cactus::Cactus() : PolyObject(QObject::tr("Cactus"),
							  QObject::tr("Cactacea Bulbuous Stingus - a Cactus has spines!"),
							  "Cactus",
							  // first the pot:
							  "(-0.11,-0.038)=(-0.053,-0.2)=(0.053,-0.2)=(0.11,-0.038);"
							  // then the plant itself:
							  "(0.045,-0.036)=(0.085,0.012)=(0.092,0.089)=(0.046,0.156)=(0.00,0.171)"
							  "=(-0.046,0.156)=(-0.092,0.089)=(-0.085,0.012)=(-0.045,-0.036)"
							  ,
							  0.25, 0.4, 1.0, 0.3)
{
// SENSOR RIGHT HALF:	74,-36 112,12 121,85 96,144 38,188

}

Cactus::~Cactus()
{
}


///---------------------------------------------------------------------------
///------------------------ Bed Of Nails -------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
//class BedOfNailsObjectFactory : public ObjectFactory
//{
//public:
//	BedOfNailsObjectFactory(void)
//	{	announceObjectType("BedOfNails", this); }
//	virtual BaseObject* createObject(void) const
//	{	return new BedOfNails(); }
//};
//static BedOfNailsObjectFactory theBedOfNailsObjectFactory;
