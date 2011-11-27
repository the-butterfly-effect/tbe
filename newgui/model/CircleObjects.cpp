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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "Box2D.h"

#include "CircleObjects.h"
#include "ViewCircleObject.h"
#include "Property.h"
#include "ObjectFactory.h"

/** the CircleObject's ObjectFactory
 *  note that it is slightly more complex than usual, because it is generalised
 *  to create any type of ball. Below the declaration, there will be several
 *  global instances each identifying one ball type
 */
class BallObjectFactory : public ObjectFactory
{
public:
	BallObjectFactory(
		const QString& anInternalName,
		const QString& aDisplayName,
		const QString& aTooltip,
		const QString& anImageName,
		qreal aRadius,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theRadius(aRadius),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual AbstractObject* createObject(void) const
	{	return fixObject(new CircleObject(theDisplayName, theTooltip, theImageName,
								theRadius, theMass, theBounciness)); }
private:
		QString theDisplayName;
		QString theTooltip;
		QString theImageName;
		qreal theRadius;
		qreal theMass;
		qreal theBounciness;
};


// we are lazy and do not model the holes in the ball, nor do we attempt to model
// the non-uniform weight distribution in the ball - we assume it to be uniform
static BallObjectFactory theBBFactory("BowlingBall",
	QObject::tr("Bowling Ball"),
	QObject::tr("Your average bowling ball - heavy, round and willing to roll"),
	"BowlingBall", 0.11, 6.0, 0.1 );

// we are lazy and do not model the air, we assume it to be uniform in mass
static BallObjectFactory theVBFactory("VolleyBall",
	QObject::tr("Volley Ball"),
	QObject::tr("A volley ball - you know: light, soft and fairly bouncy."),
	"VolleyBall", 0.105, 0.280, 0.65);


// the official standards say that a tennis ball dropped from 100 inch should bounce 53-58 inch.
// thanks to http://en.wikipedia.org/wiki/Tennis_ball
// we are lazy and do not model the air, we assume it to be uniform in mass
static BallObjectFactory theTBFactory("TennisBall",
	QObject::tr("Tennis Ball"),
	QObject::tr("A tennis ball is small, fuzzy and known for turning heads."),
	"TennisBall", 0.034, 0.058, 0.56);

// the official standards say that a soccer is 68-70cm circumference and weighs 410-450 grams
// thanks to http://en.wikipedia.org/wiki/Football_(ball)
// we are lazy and do not model the air, we assume it to be uniform in mass
static BallObjectFactory theSoccerFactory("SoccerBall",
	QObject::tr("Soccer Ball"),
	QObject::tr("A football (of the spherical persuasion)."),
	"SoccerBall", 0.110, 0.430, 0.56);

// there is not much of official standards for a petanque ball, but
// thanks to http://en.wikipedia.org/wiki/Petanque we at least know:
// diameter between 70.5 and 80mm, weight between 650 and 800 grams.
static BallObjectFactory thePetanqueFactory("PetanqueBoule",
	QObject::tr("Petanque Boule"),    // TODO/FIXME: there should be an accent on the e
	QObject::tr("A petanque ball is made of metal and heavy."),
	"PetanqueBoule", 0.038, 0.700, 0.1);

// Constructors/Destructors
//

CircleObject::CircleObject (const QString& aName,
				  const QString& aTooltip,
				  const QString& anImageName,
				  qreal aRadius,
				  qreal aMass,
				  qreal aBounciness)
	: theBallName(aName), theBallTooltip(aTooltip), theBallImage(anImageName)
{
	DEBUG5("CircleObject::CircleObject\n");

	createBallShapeFixture(aRadius, aMass);
	setTheBounciness(aBounciness);

	// for none of the CircleObjects, you're supposed to change the image...
	theProps.setDefaultPropertiesString("-" + QString(Property::IMAGE_NAME_STRING) + ":/"
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

	setTheWidth(2.0*aRadius);
	setTheHeight(2.0*aRadius);
}

ViewObject*  CircleObject::createViewObject(void)
{
	QString myImageName = theProps.getPropertyNoDefault(Property::IMAGE_NAME_STRING);
	if (myImageName.isEmpty()==false)
		theBallImage = myImageName;
	else
		if (theBallImage.isEmpty())
			theBallImage = getName();
	theViewObjectPtr = new ViewCircleObject(this, theBallImage);
	setViewObjectZValue(3.0);
	return theViewObjectPtr;
}



// ---------------------------------------------------------------------------
// ---------------------------------CustomBall--------------------------------
// ---------------------------------------------------------------------------

//// this class' ObjectFactory
class CustomBallObjectFactory : public ObjectFactory
{
public:
	CustomBallObjectFactory(void)
	{	announceObjectType("CustomBall", this); }
	virtual AbstractObject* createObject(void) const
	{	return fixObject(new CustomBall()); }
};
static CustomBallObjectFactory theCustomBallObjectFactory;


CustomBall::CustomBall (void)
	: CircleObject("CustomBall", "CustomBall", "", 1.0, 1.0, 0.2)
{
	DEBUG5("CustomBall::CustomBall\n");

	theProps.setDefaultPropertiesString(
			Property::IMAGE_NAME_STRING + QString(":CustomBall/") +
			Property::RADIUS_STRING + ":0.1/" +
			Property::MASS_STRING +":1.0/" );
}

CustomBall::~CustomBall ( )
{
}

ViewObject* CustomBall::createViewObject()
{
	parseProperties();
	return CircleObject::createViewObject();
}

void  CustomBall::parseProperties(void)
{
	DEBUG5("CustomBall::parseProperties(void)\n");
	CircleObject::parseProperties();

	float myRadius;
	float myMass;
	theProps.property2Float(Property::RADIUS_STRING, &myRadius);
	theProps.property2Float(Property::MASS_STRING, &myMass);
	if (myRadius<=Position::minimalMove)
		myRadius=0.1;

	createBallShapeFixture(myRadius, myMass);
}