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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "PolyObject.h"
#include "Property.h"

#include <QtCore/QStringList>

static const char *DEFAULT_POLYOBJECT_NAME = "PolyObject";


//// this class' ObjectFactory
class PolyObjectFactory : public ObjectFactory
{
public:
    PolyObjectFactory(void)
    {
        announceObjectType(DEFAULT_POLYOBJECT_NAME, this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new
                         PolyObject(DEFAULT_POLYOBJECT_NAME, "", DEFAULT_POLYOBJECT_NAME,
                                    "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
                                    1.0, 1.0, 0, 0.3));
    }
};
static PolyObjectFactory thePolyObjectFactory;


static AbstractPolyObjectFactory theBowlingPinFactory(
    "BowlingPin",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Bowling Pin"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Bowling pins are meant to be run "
                      "over—most\npeople prefer to do that using "
                      "a bowling ball."),
    "BowlingPin",
    "(0.02,0.17)=(-0.02,0.17)=(-0.045,0.14)=(-0.04,0.065)=(0.0,0.04)"
    "=(0.04,0.065)=(0.045,0.14);"
    "(-0.04,0.02)=(-0.06,-0.04)=(-0.06,-0.11)=(-0.035,-0.17)"
    "=(0.035,-0.17)=(0.06,-0.11)=(0.06,-0.04)=(0.04,0.02)",
    0.12, 0.34, 1.5, 0.4 );

static AbstractPolyObjectFactory theSkyhookFactory(
    "Skyhook",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Skyhook"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "A skyhook just hangs in the air. And you can hang a "
                      "lot of weight on it!"),
    "Skyhook",
    "(-0.03,-0.07)=(0.01,-0.11)=(0.05,-0.11)=(0.1,-0.05)=(0.1,-0.02)"
    "=(0.08,0.00)=(-0.02,-0.03);(-0.10,0.12)",
    0.20, 0.23, 0.0, 0.4, Property::ZVALUE_STRING + QString(":1.0") );

static AbstractPolyObjectFactory theWeightFactory(
    "Weight",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Weight"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "A serious mass. It is very heavy."),
    "Weight",
    "(-0.20,-0.20)=(0.20,-0.20)=(0.06,0.20)=(-0.06,0.20)",
    0.40, 0.40, 10.0, 0.3 );

static AbstractPolyObjectFactory theLeftRampFactory(
    "LeftRamp",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Left Ramp"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "This is a ramp.\nThe left is lower than the right, so things slide to the left."),
    "LeftRamp",
    "(-0.5,-0.46)=(-0.5,-0.5)=(-0.3,-0.5)=(0.4,0.2)=(0.5,0.4)=(0.5,0.5)",
    1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theRightRampFactory(
    "RightRamp",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Right Ramp"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "This is a ramp.\nThe left is higher than the right, so things slide to the right."),
    "RightRamp",
    "(-0.5,0.5)=(-0.5,0.4)=(-0.4,0.2)=(0.3,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
    1.0, 1.0, 0.0, 0.2 );

static AbstractPolyObjectFactory theLeftWedgeFactory(
    "LeftWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Left Birch Wedge"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Birch wedges can be moved and are rather heavy.\nThe left is lower than the right, so things slide to the left."),
    "birch-wedge-left",
    "(-0.5,-0.46)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
    1.0, 1.0, 2.0, 0.2 );

static AbstractPolyObjectFactory theRightWedgeFactory(
    "RightWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Right Birch Wedge"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Birch wedges can be moved and are rather heavy.\nThe left is higher than the right, so things slide to the right."),
    "birch-wedge-right",
    "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
    1.0, 1.0, 2.0, 0.2 );

static AbstractPolyObjectFactory theLeftStyrofoamWedgeFactory(
    "LeftStyrofoamWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Left Styrofoam Wedge"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Styrofoam wedges are light and bouncy."),
    "styrofoam-left",
    "(-0.5,-0.46)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
    1.0, 1.0, 0.5, 0.6 );

static AbstractPolyObjectFactory theRightStyrofoamWedgeFactory(
    "RightStyrofoamWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Right Styrofoam Wedge"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Styrofoam wedges are light and bouncy."),
    "styrofoam-right",
    "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
    1.0, 1.0, 0.5, 0.6 );

static AbstractPolyObjectFactory theLeftFixedWedgeFactory(
    "LeftFixedWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Left Inclined Plane"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "It’s a fixed obstacle with a tilted surface.\nThe left is lower than the right, so things slide to the left."),
    "usedwood-wedge-left",
    "(-0.5,-0.46)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,0.5)",
    1.0, 1.0, 0.0, 0.1 );

static AbstractPolyObjectFactory theRightFixedWedgeFactory(
    "RightFixedWedge",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Right Inclined Plane"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "It’s a fixed obstacle with a tilted surface.\nThe left is higher than the right, so things slide to the right."),
    "usedwood-wedge-right",
    "(-0.5,0.5)=(-0.5,-0.5)=(0.5,-0.5)=(0.5,-0.46)",
    1.0, 1.0, 0.0, 0.1 );

// the quarter arc is based on five similarly-sized segments, it is more-or-less smooth
// on both the inside and the outside - you can use both if you want :-)
static AbstractPolyObjectFactory the40QuarterArcFactory(
    "QuarterArc40",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Small Quarter Arc"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "A quarter arc is a fixed obstacle\nand useful to divert things."),
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
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Large Quarter Arc"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "A quarter arc is a fixed obstacle\nand useful to divert things."),
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


static AbstractPolyObjectFactory theToyChestFactory(
    "ToyChest",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Toy Chest"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Most people use a chest to keep things."),
    "toychest",
    "(-0.47,-0.82)=(-0.4,-0.82)=(-0.4,-0.10)=(-0.47,-0.08);"
    "(0.3,-0.82)=(0.4,-0.82)=(0.4,-0.08)=(0.3,-0.10);"
    "(0.4,-0.08)=(0.45,0.0)=(0.45,0.18)=(0.38,0.45)=(0.2,0.75)=(0.12,0.80)=(0.04,0.82);"
    "(-0.4,-0.82)=(0.3,-0.82)=(0.3,-0.73)=(-0.4,-0.73)",
    1.0, 1.7, 0.0, 0.05, Property::ZVALUE_STRING + QString(":3.0") );

static AbstractPolyObjectFactory theGoalFactory(
    "Goal",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Goal"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "A goal is a metal frame holding a net.\nIt is open on one side."),
    "goal",
    "(-0.386,0.490)=(-0.386,0.435)=(0.209,0.435)=(0.194,0.470);"
    "(0.160,0.410)=(0.40,-0.488)=(0.45,-0.479)=(0.230,0.430);",
    0.4, 0.7, 0.0, 0.2, Property::ZVALUE_STRING + QString(":4.5") );

static AbstractPolyObjectFactory theCardboardBoxFactory(
    "CardboardBox",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Cardboard Box"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Cardboard boxes are used to carry small and light things around."),
    "CardboardBox",
    "(-0.40,-0.4)=(-0.35,-0.4)=(-0.35,0.40)=(-0.40,0.40);"
    "(0.35,-0.4)=(0.4,-0.4)=(0.4,0.40)=(0.35,0.40);"
    "(-0.35,-0.4)=(-0.35,-0.35)=(0.35,-0.35)=(0.35,-0.4)",
    0.9, 0.78809, 0.5, 0.06, Property::ZVALUE_STRING + QString(":4.0") );

static AbstractPolyObjectFactory theSmallSeesawFactory(
    "SeesawSmall",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Small Seesaw"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "If this seesaw is pushed on one side,\nthe other side moves in the other direction."),
    "seesaw-tiny",
    "(-1.25,0.15)=(-1.25,0.01)=(1.25,0.01)=(1.25,0.15);"
    "(-1.25,0.01)=(-1.25,-0.15)=(-1.18,-0.15)=(-0.85,0.01);"
    "(1.25,0.01)=(0.85,0.01)==(1.18,-0.15)=(1.25,-0.15)",
    2.5, 0.30, 5.0, 0.4, "PivotPoint:(0.0,0.05)" );

static AbstractPolyObjectFactory theRotatingBarFactory(
    "RotatingBar",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Rotating Bar"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "This wooden bar rotates around its center."),
    "rotating_bar",
    "(-.5,0.05)=(-.5,-0.05)=(.5,-0.05)=(.5,0.05)",
    1.0, 0.12, 4.0, 0.4, "PivotPoint:(0.0,0.00)" );

static AbstractPolyObjectFactory theDaffodilFactory(
    "Daffodil",
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory", "Daffodil"),
    QT_TRANSLATE_NOOP("AbstractPolyObjectFactory",
                      "Daffodil (Narcissus Flappus Attractus):\nButterflies want to fly there."),
    "Anonymous_daffodil_klaas",
    "(-0.5,0.347)=(-0.5,0.12)=(-0.141,0.019)=(0.388,0.061)=(0.5,0.37)=(0.022,0.5)=(-0.45,0.391);"
    "(0,-0.5)",
    0.4, 0.9, 0.0, 0.1, "Friction:0.05/ZValue:1.2" );

// Constructors/Destructors
//

PolyObject::PolyObject( const QString &aDisplayName,
                        const QString &aTooltip,
                        const QString &aImageName,
                        const QString &anOutline,
                        qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness,
                        const QString &aDefaultPropertiesString)
    : AbstractObject( aTooltip, aImageName,
                      aWidth, aHeight, aMass, aBounciness,
                      aDefaultPropertiesString), theNameString (aDisplayName)
{
    DEBUG5("PolyObject::PolyObject(%s, %f, %f)", ASCII(aDisplayName), aWidth, aHeight);
    theToolTip = aTooltip;
    theProps.setDefaultPropertiesString(
        Property::POLYGONS_STRING + QString(":") + anOutline + QString("/") +
        aDefaultPropertiesString + QString("/") );

    // Make mass-related attributes for the generic object since those are hidden otherwise
    if (aDisplayName == DEFAULT_POLYOBJECT_NAME) {
        theProps.setDefaultPropertiesString(
            Property::MASS_STRING + QString(":/") +
            Property::TRANSLATIONGUIDE_STRING + QString(":/") +
            Property::PIVOTPOINT_STRING + QString(":/"));
    }
}


PolyObject::~PolyObject ( )
{
    DEBUG5("PolyObject::~PolyObject()");
}


void PolyObject::fillShapeList(void)
{
    DEBUG5ENTRY;

    float myMass = 0.0;
    bool hasMass = theProps.property2Float(Property::MASS_STRING, &myMass);

    QString myPolygons;
    theProps.property2String(Property::POLYGONS_STRING, &myPolygons);
    QStringList myPolygonList = myPolygons.split(";", QString::SkipEmptyParts);

    assert(theAABB.isInitialised());
    if (theAABB.isInitialised() == false)
        return;
    Vector myScale(getTheWidth() / theAABB.getOrigWidth(), getTheHeight() / theAABB.getOrigHeight());

    QStringList::iterator i = myPolygonList.begin();
    while (i != myPolygonList.end()) {
        b2PolygonShape *myPolyShape = new b2PolygonShape;

        QStringList myCoordList = (*i).split("=", QString::SkipEmptyParts);
        // if we find a shape with less than three vertexes, let's ignore it
        // this is very convenient if you need to adapt the AABB :-)
        if (myCoordList.count() >= 3) {
            b2Vec2 myVertices[b2_maxPolygonVertices];
            for (int j = 0; j < myCoordList.count(); j++) {
                assert (j < b2_maxPolygonVertices);
                Vector myCoord;
                bool isDone = myCoord.fromString(myCoordList.at(j));
                assert (isDone == true);
                UNUSED_VAR(isDone);
                Vector myScaledCoord = myScale * myCoord;
                myVertices[j] = myScaledCoord.toB2Vec2();
            }
            myPolyShape->Set(myVertices, myCoordList.count());

            // get mass:  no mass -> no density -> no motion
            b2FixtureDef *myFixtureDef = new b2FixtureDef();
            myFixtureDef->shape   = myPolyShape;
            if (hasMass)
                myFixtureDef->density = myMass / (getTheWidth() * getTheHeight());
            myFixtureDef->userData = this;
            setFriction(myFixtureDef);
            theShapeList.push_back(myFixtureDef);
        } else
            delete myPolyShape;
        ++i;
    }
}

b2BodyType PolyObject::getObjectType(void) const
{
    float myMass;
    if (theProps.property2Float(Property::MASS_STRING, &myMass))
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

void  PolyObject::setFriction(b2FixtureDef *aFixtureDef)
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

PolyObject::AABB::AABB(QString &aPolygonDefs) :
    theOrigWidth(UNDEFINED), theOrigHeight(UNDEFINED)
{
    DEBUG5("PolyObject::AABB::AABB(%s)", ASCII(aPolygonDefs));
    float myMinX = 0.0f;
    float myMaxX = 0.0f;
    float myMinY = 0.0f;
    float myMaxY = 0.0f;

    if (aPolygonDefs.isEmpty()) {
        DEBUG5("ignoring empty polygon def");
        return;
    }

    QStringList myPolygonList = aPolygonDefs.split(";", QString::SkipEmptyParts);
    QStringList::iterator i = myPolygonList.begin();
    while (i != myPolygonList.end()) {
        QStringList myCoordList = (*i).split("=", QString::SkipEmptyParts);
        for (int j = 0; j < myCoordList.count(); j++) {
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
