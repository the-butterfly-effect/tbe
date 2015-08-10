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

#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "PolyObject.h"
#include "Property.h"

#include <QtCore/QStringList>

static const char* DEFAULT_POLYOBJECT_NAME = "PolyObject";


//// this class' ObjectFactory
class PolyObjectFactory : public ObjectFactory
{
public:
	PolyObjectFactory(void)
	{	announceObjectType(DEFAULT_POLYOBJECT_NAME, this); }
	virtual AbstractObject* createObject(void) const
	{	return fixObject(new
						 PolyObject(DEFAULT_POLYOBJECT_NAME, "", DEFAULT_POLYOBJECT_NAME,
								   "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
									1.0, 1.0, 0, 0.3)); }
};
static PolyObjectFactory thePolyObjectFactory;


/** the AbstractPolyObjectFactory
 *  Note that it is slightly more complex than usual, because it is generalised
 *  to create any type of PolyObject. Below the declaration, there will be several
 *  global instances each identifying one rectobject type
 */
class AbstractPolyObjectFactory : public ObjectFactory
{
public:
	AbstractPolyObjectFactory(
		const QString& anInternalName,
		const QString& aDisplayName,
		const QString& aTooltip,
		const QString& anImageName,
		const QString& anOutline,
		qreal aWidth,
		qreal aHeight,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theOutline(anOutline),
			  theWidth(aWidth), theHeight(aHeight),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual AbstractObject* createObject(void) const
	{	return fixObject(new PolyObject(theDisplayName, theTooltip,
										theImageName, theOutline,
										theWidth, theHeight, theMass,
										theBounciness)); }
private:
		QString theDisplayName;
		QString theTooltip;
		QString theImageName;
		QString theOutline;
		qreal theWidth;
		qreal theHeight;
		qreal theMass;
		qreal theBounciness;
};

static AbstractPolyObjectFactory theBowlingPinFactory(
	"BowlingPin",
	QObject::tr("Bowling Pin"),
	QObject::tr("Bowling pins are meant to be run "
				"over - and most people prefer to do that using "
				"a Bowling Ball."),
	"BowlingPin",
	"(0.02,0.17)=(-0.02,0.17)=(-0.045,0.14)=(-0.04,0.065)=(0.0,0.04)"
	"=(0.04,0.065)=(0.045,0.14);"
	"(-0.04,0.02)=(-0.06,-0.04)=(-0.06,-0.11)=(-0.035,-0.17)"
	"=(0.035,-0.17)=(0.06,-0.11)=(0.06,-0.04)=(0.04,0.02)",
	0.12, 0.34, 1.5, 0.4 );

static AbstractPolyObjectFactory theSkyhookFactory(
	"Skyhook",
	QObject::tr("Skyhook"),
	QObject::tr("A skyhook just hangs in the air. And you can hang a "
				"lot of weight on it!"),
	"Skyhook",
	"(-0.03,-0.07)=(0.01,-0.11)=(0.05,-0.11)=(0.1,-0.05)=(0.1,-0.02)"
	"=(0.08,0.00)=(-0.02,-0.03);(-0.10,0.12)",
	0.20, 0.23, 0.0, 0.4 );

static AbstractPolyObjectFactory theWeightFactory(
	"Weight",
	QObject::tr("Weight"),
	QObject::tr("A serious mass. As heavy as it looks!"),
	"Weight",
	"(-0.20,-0.20)=(0.20,-0.20)=(0.06,0.20)=(-0.06,0.20)",
	0.40, 0.40, 10.0, 0.3 );

static AbstractPolyObjectFactory theLeftRampFactory(
	"LeftRamp",
	QObject::tr("Left Ramp"),
	QObject::tr("This is a ramp. The left is lower than the right, so things slide to the left."),
	"LeftRamp",
		"(-0.5,-0.46)=(-0.5,-0.5)=(0.13,-0.5)=(0.5,0.5)",
	1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theRightRampFactory(
	"RightRamp",
	QObject::tr("Right Ramp"),
	QObject::tr("This is a ramp. The left is higher than the right, so things slide to the right."),
	"RightRamp",
		"(-0.5,0.5)=(-0.13,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
	1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theLeftWedgeFactory(
	"LeftWedge",
	QObject::tr("Left Wedge"),
	QObject::tr("This is a wedge. The left is lower than the right, so things slide to the left."),
	"birch-wedge-left",
		"(-0.5,-0.46)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
	1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theRightWedgeFactory(
	"RightWedge",
	QObject::tr("Right Wedge"),
	QObject::tr("This is a wedge. The left is higher than the right, so things slide to the right."),
	"birch-wedge-right",
		"(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
	1.0, 1.0, 0.0, 0.2 );

// the quarter arc is based on five similarly-sized segments, it is more-or-less smooth
// on both the inside and the outside - you can use both if you want :-)
static AbstractPolyObjectFactory the40QuarterArcFactory(
	"QuarterArc40",
	QObject::tr("Quarter Arc Small"),
	QObject::tr("This is a quarter arc. Or ninety degrees, or 1.57 radians if you want."),
	"QuarterArc",
	"(0.100,-.200)=(0.200,-.200)=(0.180,-.076)=(0.085,-.107);"
	"(0.085,-.107)=(0.180,-.076)=(0.124,0.035)=(0.043,-.024);"
	"(0.043,-.024)=(0.124,0.035)=(0.035,0.124)=(-.024,0.043);"
	"(-.024,0.043)=(0.035,0.124)=(-.076,0.180)=(-.107,0.085);"
	"(-.107,0.085)=(-.076,0.180)=(-.200,0.200)=(-.200,0.100)",
	0.4, 0.4, 0.0, 0.1 );

// the Large quarter arc is based on nine similarly-sized segments, it is more-or-less smooth
// on both the inside and the outside - you can use both if you want :-)
static AbstractPolyObjectFactory the80QuarterArcFactory(
	"QuarterArc80",
	QObject::tr("Quarter Arc Large"),
	QObject::tr("This is a quarter arc. Or ninety degrees, or 1.57 radians if you want."),
	"QuarterArc80",
	"( 0.300,-0.400)=( 0.400,-0.400)=( 0.388,-0.261)=( 0.289,-0.278);"
	"( 0.289,-0.278)=( 0.388,-0.261)=( 0.352,-0.126)=( 0.258,-0.161);"
	"( 0.258,-0.161)=( 0.352,-0.126)=( 0.293,-0.000)=( 0.206,-0.050);"
	"( 0.206,-0.050)=( 0.293, 0.000)=( 0.213, 0.114)=( 0.136, 0.050);"
	"( 0.136, 0.050)=( 0.213, 0.114)=( 0.114, 0.213)=( 0.050, 0.136);"
	"( 0.050, 0.136)=( 0.114, 0.213)=( 0.000, 0.293)=(-0.050, 0.206);"
	"(-0.050, 0.206)=( 0.000, 0.293)=(-0.126, 0.352)=(-0.161, 0.258);"
	"(-0.161, 0.258)=(-0.126, 0.352)=(-0.261, 0.388)=(-0.278, 0.289);"
	"(-0.278, 0.289)=(-0.261, 0.388)=(-0.400, 0.400)=(-0.400, 0.300)",
	0.8, 0.8, 0.0, 0.1 );

// Constructors/Destructors
//

PolyObject::PolyObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				const QString& anOutline,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness )
    : theNameString(aDisplayName)
{
	DEBUG5("PolyObject::PolyObject(%s, %f, %f)", ASCII(aDisplayName), aWidth, aHeight);
    theToolTip = aTooltip;
	theProps.setDefaultPropertiesString(
		Property::FRICTION_STRING + QString(":/") +
		Property::IMAGE_NAME_STRING + QString(":") + aImageName + QString("/") +
		Property::MASS_STRING + ":" + QString::number(aMass) + QString("/") +
		Property::POLYGONS_STRING + QString(":") + anOutline + QString("/")
		);

	setTheBounciness(aBounciness);
	setTheWidth(aWidth, false);
	setTheHeight(aHeight, true);
}


PolyObject::~PolyObject ( )
{
    DEBUG5("PolyObject::~PolyObject()");
}


void PolyObject::fillShapeList(void)
{
	DEBUG5ENTRY;

	float myMass;
	theProps.property2Float(Property::MASS_STRING, &myMass);

	QString myPolygons;
	theProps.property2String(Property::POLYGONS_STRING, &myPolygons);
	QStringList myPolygonList = myPolygons.split(";",QString::SkipEmptyParts);

	assert(theAABB.isInitialised());
	if (theAABB.isInitialised()==false)
		return;
	Vector myScale(getTheWidth()/theAABB.getOrigWidth(), getTheHeight()/theAABB.getOrigHeight());

	QStringList::iterator i = myPolygonList.begin();
	while (i!=myPolygonList.end())
	{
		b2PolygonShape* myPolyShape = new b2PolygonShape();

		QStringList myCoordList = (*i).split("=",QString::SkipEmptyParts);
		// if we find a shape with less than three vertexes, let's ignore it
		// this is very convenient if you need to adapt the AABB :-)
		if (myCoordList.count()>=3)
		{
			b2Vec2 myVertices[b2_maxPolygonVertices];
			for (int j=0; j<myCoordList.count(); j++)
			{
				assert (j < b2_maxPolygonVertices);
				Vector myCoord;
				bool isDone = myCoord.fromString(myCoordList.at(j));
				assert (isDone == true);
				UNUSED_VAR(isDone);
				Vector myScaledCoord = myScale*myCoord;
				myVertices[j]=myScaledCoord.toB2Vec2();
			}
			myPolyShape->Set(myVertices, myCoordList.count());

			// get mass:  no mass -> no density -> no motion
			b2FixtureDef* myFixtureDef = new b2FixtureDef();
			myFixtureDef->shape   = myPolyShape;
			if (myMass != 0.0)
				myFixtureDef->density = myMass / (getTheWidth()*getTheHeight());
			myFixtureDef->userData = this;
			setFriction(myFixtureDef);
			theShapeList.push_back(myFixtureDef);
		}
        else
            delete myPolyShape;
		++i;
	}
}

b2BodyType PolyObject::getObjectType(void) const
{
	float myMass;
	theProps.property2Float(Property::MASS_STRING, &myMass);
	if (myMass > 0.001)
		return b2_dynamicBody;
	return b2_staticBody;
}

void PolyObject::parseProperties(void)
{
	AbstractObject::parseProperties();

	QString myPolygons;
	theProps.property2String(Property::POLYGONS_STRING, &myPolygons);
	theAABB = AABB(myPolygons);

	clearShapeList();
	fillShapeList();
}

void  PolyObject::setFriction(b2FixtureDef* aFixtureDef)
{
	// only set friction if it is special
	if (theProps.getPropertyNoDefault(Property::FRICTION_STRING).isEmpty())
		return;

	float myFriction = 0;
	if (theProps.property2Float(Property::FRICTION_STRING, &myFriction))
		aFixtureDef->friction = myFriction;
	else
		assert(false);
}




PolyObject::AABB::AABB(void) :
		theOrigWidth(UNDEFINED), theOrigHeight(UNDEFINED)
{
}

PolyObject::AABB::AABB(QString& aPolygonDefs) :
		theOrigWidth(UNDEFINED), theOrigHeight(UNDEFINED)
{
	DEBUG5("PolyObject::AABB::AABB(%s)", ASCII(aPolygonDefs));
	float myMinX = 0.0f;
	float myMaxX = 0.0f;
	float myMinY = 0.0f;
	float myMaxY = 0.0f;

	if (aPolygonDefs.isEmpty())
	{
        DEBUG5("ignoring empty polygon def");
		return;
	}

	QStringList myPolygonList = aPolygonDefs.split(";",QString::SkipEmptyParts);
	QStringList::iterator i = myPolygonList.begin();
	while (i!=myPolygonList.end())
	{
		QStringList myCoordList = (*i).split("=",QString::SkipEmptyParts);
		for (int j=0; j<myCoordList.count(); j++)
		{
			Vector myCoord;
			bool isDone = myCoord.fromString(myCoordList.at(j));
			assert (isDone == true);
			UNUSED_VAR(isDone);

			if (myMinX > myCoord.dx)
				myMinX = myCoord.dx;
			if (myMinY > myCoord.dy)
				myMinY = myCoord.dy;
			if (myMaxX < myCoord.dx)
				myMaxX = myCoord.dx;
			if (myMaxY < myCoord.dy)
				myMaxY = myCoord.dy;
		}
		++i;
	}

	assert (myMaxX != myMinX);
	assert (myMaxY != myMinY);

	theOrigWidth  = myMaxX - myMinX;
	theOrigHeight = myMaxY - myMinY;
	DEBUG5("  AABB for this PolyObject: %fx%f", theOrigWidth, theOrigHeight);
}

float PolyObject::AABB::getOrigWidth()
{
	assert(isInitialised());
	return theOrigWidth;
}

float PolyObject::AABB::getOrigHeight()
{
	assert(isInitialised());
	return theOrigHeight;
}

bool PolyObject::AABB::isInitialised()
{
	return theOrigHeight != UNDEFINED;
}
