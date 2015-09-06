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

#include "tbe_global.h"
#include "Box2D.h"

#include "CircleObjects.h"
//#include "ViewCircleObject.h"
#include "Property.h"

// we are lazy and do not model the holes in the ball, nor do we attempt to model
// the non-uniform weight distribution in the ball - we assume it to be uniform
static CircleObjectFactory theBBFactory("BowlingBall",
    QT_TRANSLATE_NOOP("CircleObjectFactory", "Bowling Ball"),
    QT_TRANSLATE_NOOP("CircleObjectFactory", "Your average bowling ball - heavy, round and willing to roll"),
	"BowlingBall", 0.11, 6.0, 0.1 );

// we are lazy and do not model the air, we assume it to be uniform in mass
static CircleObjectFactory theVBFactory("VolleyBall",
    QT_TRANSLATE_NOOP("CircleObjectFactory", "Volley Ball"),
    QT_TRANSLATE_NOOP("CircleObjectFactory", "A volley ball - you know: light, soft and fairly bouncy."),
	"VolleyBall", 0.105, 0.280, 0.65);


// the official standards say that a tennis ball dropped from 100 inch should bounce 53-58 inch.
// thanks to http://en.wikipedia.org/wiki/Tennis_ball
// we are lazy and do not model the air, we assume it to be uniform in mass
static CircleObjectFactory theTBFactory("TennisBall",
    QT_TRANSLATE_NOOP("CircleObjectFactory", "Tennis Ball"),
    QT_TRANSLATE_NOOP("CircleObjectFactory", "A tennis ball is small, fuzzy and known for turning heads."),
	"TennisBall", 0.034, 0.058, 0.56);

// the official standards say that a soccer is 68-70cm circumference and weighs 410-450 grams
// thanks to http://en.wikipedia.org/wiki/Football_(ball)
// we are lazy and do not model the air, we assume it to be uniform in mass
static CircleObjectFactory theSoccerFactory("SoccerBall",
    QT_TRANSLATE_NOOP("CircleObjectFactory", "Soccer Ball"),
    QT_TRANSLATE_NOOP("CircleObjectFactory", "A football (of the spherical persuasion)."),
	"SoccerBall", 0.110, 0.430, 0.56);

// there is not much of official standards for a petanque ball, but
// thanks to http://en.wikipedia.org/wiki/Petanque we at least know:
// diameter between 70.5 and 80mm, weight between 650 and 800 grams.
static CircleObjectFactory thePetanqueFactory("PetanqueBoule",
    QT_TRANSLATE_NOOP_UTF8("CircleObjectFactory", "Pétanque Boule"),
    QT_TRANSLATE_NOOP_UTF8("CircleObjectFactory", "A pétanque ball is made of metal and heavy."),
	"PetanqueBoule", 0.038, 0.700, 0.1);

// Constructors/Destructors
//

CircleObject::CircleObject (const QString& aName,
				  const QString& aTooltip,
				  const QString& anImageName,
				  qreal aRadius,
				  qreal aMass,
				  qreal aBounciness)
    : theBallName(aName)
{
	DEBUG5ENTRY;
	createBallShapeFixture(aRadius, aMass);
	setTheBounciness(aBounciness);
    theToolTip =  aTooltip;
	theProps.setDefaultPropertiesString(
				QString("%1:%2/").arg(Property::IMAGE_NAME_STRING).arg(anImageName)
				+ QString(Property::BOUNCINESS_STRING) + QString(":%1").arg(aBounciness));
}

CircleObject::~CircleObject ( ) { }

//
// Methods
//


// Accessor methods
//


// Other methods
//

void CircleObject::createBallShapeFixture(float aRadius, float aMass)
{
	clearShapeList();
	b2CircleShape* ballShape = new b2CircleShape();
	ballShape->m_radius = aRadius;

	b2FixtureDef* ballFixDef = new b2FixtureDef();
	ballFixDef->density = aMass/(PI*aRadius*aRadius);
	ballFixDef->userData = this;
	ballFixDef->shape = ballShape;

	theShapeList.push_back(ballFixDef);

	if (aMass > 0.001)
		theB2ObjectType = b2_dynamicBody;
	else
		theB2ObjectType = b2_staticBody;

	setTheWidth(2.0*aRadius, false);
	setTheHeight(2.0*aRadius, false);
}


// ---------------------------------------------------------------------------
// ---------------------------------CustomBall--------------------------------
// ---------------------------------------------------------------------------

//// this class' ObjectFactory
class CustomCircleObjectFactory : public ObjectFactory
{
public:
    CustomCircleObjectFactory(void)
	{	announceObjectType("CustomBall", this); }
	virtual AbstractObject* createObject(void) const
	{	return fixObject(new CustomBall()); }
};
static CustomCircleObjectFactory theCustomCircleObjectFactory;


CustomBall::CustomBall (void)
	: CircleObject("CustomBall", "CustomBall", "", 1.0, 1.0, 0.2)
{
	DEBUG5ENTRY;

	theProps.setDefaultPropertiesString(
			Property::IMAGE_NAME_STRING + QString(":CustomBall/") +
			Property::RADIUS_STRING + ":0.1/" +
			Property::MASS_STRING +":1.0/" );
}

CustomBall::~CustomBall ( )
{
}

ViewObject* CustomBall::createViewObject(float aDefaultDepth)
{
	parseProperties();
	return CircleObject::createViewObject(aDefaultDepth);
}

void  CustomBall::parseProperties(void)
{
	DEBUG5ENTRY;
	CircleObject::parseProperties();

	float myRadius;
	float myMass;
	theProps.property2Float(Property::RADIUS_STRING, &myRadius);
	theProps.property2Float(Property::MASS_STRING, &myMass);
	if (myRadius<=Position::minimalMove)
		myRadius=0.1;

	createBallShapeFixture(myRadius, myMass);
}
