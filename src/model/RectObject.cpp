/* The Butterfly Effect
 * This file copyright (C) 2009,2012  Klaas van Gend
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

#include "RectObject.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ViewObject.h"
#include "Property.h"

static const char* DEFAULT_RECTOBJECT_NAME = "RectObject";

const qreal RectObject::ASPECT_RATIO = 10.0;


//// this class' ObjectFactory
class RectObjectFactory : public ObjectFactory
{
public:
	RectObjectFactory(void)
	{	announceObjectType(DEFAULT_RECTOBJECT_NAME, this); }
	AbstractObject* createObject(void) const override
	{	return fixObject(new RectObject()); }
};
static RectObjectFactory theRectObjectFactory;




// ---------------------------------predefined rectangular objects:
// order of the arguments below:
//              anInternalName, (do not translate)
//              aDisplayName,
//              aTooltip,
// anImageName,   aWidth,aHeight,  aMass,  aBounciness

// birch wood: 600 kg/m^3, i.e. a beam of 10cm x 10cm x 1.2m equals 7.2kg
static AbstractRectObjectFactory theBirchBarFactory("BirchBar",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Wooden Bar"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Birch is a type of wood.\nBirch wood beams move and float."),
	"birch_bar", 1.0, 0.1, 7.2, 0.15 );

static AbstractRectObjectFactory theDomRedFactory("DominoRed",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Domino (Red)"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "The famous red plastic domino stone."),
	"DominoRed", 0.1, 0.5, 2.5, 0.1 );

static AbstractRectObjectFactory theDomBlueFactory("DominoBlue",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Domino (Blue)"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "The famous blue plastic domino stone."),
	"DominoBlue", 0.1, 0.5, 2.5, 0.1 );

static AbstractRectObjectFactory theDomGreenFactory("DominoGreen",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Domino (Green)"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "The famous green plastic domino stone."),
	"DominoGreen", 0.1, 0.5, 2.5, 0.1 );

static AbstractRectObjectFactory theFloorFactory("Floor",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Floor"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "This is the floor. It is attached to the scene\nand can't be moved, penetrated or destroyed."),
	"used_wood_bar", 1.0, 0.1, 0.0, 0.1 );

// see http://www.saginawpipe.com/steel_i_beams.htm
// a 4"x4" beam weighs 13 pounds per feet, i.e. a 1.4m beam weighs 27kg.
static AbstractRectObjectFactory theSteelHBeamFactory("IBeam",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Steel I-Beam"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "This is a steel I-beam. Steel I-beams are large and heavy\nand useful to build bridges and other constructions."),
	"i-beam", 1.4, 0.1, 27.0, 0.0 );

static AbstractRectObjectFactory theWallFactory("Wall",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Wall"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "This is a brick wall. It is attached to the scene\nand can't be moved, penetrated or destroyed."),
	"oldbrick", 0.2, 1.0, 0.0, 0.05 );

// Note that this hammer is kind-of heavy, a normal hammer would be around 400 grams, only
static AbstractRectObjectFactory theHammerFactory("Hammer",
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Hammer"),
	QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "This is a hammer which has been attached\nto the scene at the end of its handle.\nThe hammer can be used to apply a force\nto some of the heavier objects."),
	"hammer", 0.45,0.18, 2.0, 0.0,
	"Friction:0.0/PivotPoint:(0.2,0)/");

static AbstractRectObjectFactory theColaCrateFactory("ColaCrate",
    QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "Cola Crate"),
    QT_TRANSLATE_NOOP("AbstractRectObjectFactory", "A crate of 12 filled cola bottles.\nIt's very heavy and hard to push around."),
    "cola-crate", 0.85, 0.6, 18.0, 0.1,
    "Friction:0.1/");

// Constructors/Destructors
//

RectObject::RectObject ( ) : AbstractObject(), theNameString(DEFAULT_RECTOBJECT_NAME)
{
	DEBUG5("RectObject::RectObject");

	// because this object is very flexible and many parameters can be set through
	// the Properties, do not assume too much here...

	// also: keep in mind that child objects may set some things automatically
	initRectAttributes();
}

RectObject::RectObject(const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness,
				const char *aPropertiesText)
    : AbstractObject(aTooltip, aImageName,
                     aWidth, aHeight, aMass, aBounciness,
                     aPropertiesText), theNameString (aDisplayName)
{
	// This is the enhanced constructor for RectObjects.
	// When called, the ImageName is the default for that type of RectObject.
	// So we set it as a default property (which means it is not saved).
	Q_ASSERT(aDisplayName!=DEFAULT_RECTOBJECT_NAME);

    initRectAttributes();
}

RectObject::~RectObject ( )
{
}

//
// Methods
//


// Accessor methods
//


// Other methods
//

b2BodyType RectObject::getObjectType(void) const
{
	float myMass;
	if (theProps.property2Float(Property::MASS_STRING, &myMass))
		return b2_dynamicBody;
	return b2_staticBody;
}

void RectObject::initRectAttributes ( )
{
	theProps.setDefaultPropertiesString(
		Property::FRICTION_STRING    + QString(":/") +
		Property::RESIZABLE_STRING   + QString(":") + Property::NONE_STRING + "/" );
}


void  RectObject::parseProperties(void)
{
	// first parse everything that AbstractObject already knows about
	AbstractObject::parseProperties();

	theProps.property2String(Property::OBJECT_NAME_STRING,&theNameString);

	clearShapeList();
	b2PolygonShape* myBoxShape = new b2PolygonShape();
	myBoxShape->SetAsBox(getTheWidth()/2.0, getTheHeight()/2.0);

	// get mass:  if no mass set, we'll make this a b2_staticBody
	b2FixtureDef* myBoxDef = new b2FixtureDef();
	float myMass;
	if (theProps.property2Float(Property::MASS_STRING, &myMass))
		myBoxDef->density = myMass / (getTheWidth()*getTheHeight());
	myBoxDef->shape   = myBoxShape;
	myBoxDef->userData = this;
	setFriction(myBoxDef);
	theShapeList.push_back(myBoxDef);
}

void  RectObject::setFriction(b2FixtureDef* aFixtureDef)
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
