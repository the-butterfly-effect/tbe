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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "RectObject.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawObject.h"
#include "Property.h"

static const char* DEFAULT_RECTOBJECT_NAME = "RectObject";

const qreal RectObject::ASPECT_RATIO = 10.0;


//// this class' ObjectFactory
class RectObjectFactory : public ObjectFactory
{
public:
	RectObjectFactory(void)
	{	announceObjectType(DEFAULT_RECTOBJECT_NAME, this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new RectObject()); }
};
static RectObjectFactory theRectObjectFactory;


/** the AbstractRectObjectFactory
 *  note that it is slightly more complex than usual, because it is generalised
 *  to create any type of rectobject. Below the declaration, there will be several
 *  global instances each identifying one rectobject type
 */
class AbstractRectObjectFactory : public ObjectFactory
{
public:
	AbstractRectObjectFactory(
		const QString& anInternalName,
		const QString& aDisplayName,
		const QString& aTooltip,
		const QString& anImageName,
		qreal aWidth,
		qreal aHeight,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theWidth(aWidth), theHeight(aHeight),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual BaseObject* createObject(void) const
	{	return fixObject(new RectObject(theDisplayName, theTooltip,
										theImageName, theWidth, theHeight,
										theMass, theBounciness)); }
private:
		QString theDisplayName;
		QString theTooltip;
		QString theImageName;
		qreal theWidth;
		qreal theHeight;
		qreal theMass;
		qreal theBounciness;
};


static AbstractRectObjectFactory theDomRedFactory("DominoRed",
	QObject::tr("Domino (Red)"),
	QObject::tr("The famous plastic red domino stone"),
	"DominoRed", 0.1, 0.5, 2.5, 0.1 );

static AbstractRectObjectFactory theDomBlueFactory("DominoBlue",
	QObject::tr("Domino (Blue)"),
	QObject::tr("The famous plastic blue domino stone"),
	"DominoBlue", 0.1, 0.5, 2.5, 0.1 );

static AbstractRectObjectFactory theDomGreenFactory("DominoGreen",
	QObject::tr("Domino (Green)"),
	QObject::tr("The famous plastic green domino stone"),
	"DominoGreen", 0.1, 0.5, 2.5, 0.1 );


// Constructors/Destructors
//

RectObject::RectObject ( ) : BaseObject(), theNameString(DEFAULT_RECTOBJECT_NAME)
{
	DEBUG5("RectObject::RectObject\n");

	// because this object is very flexible and many parameters can be set through
	// the Properties, do not assume too much here...

	// also: keep in mind that child objects may set some things automatically
	initAttributes();
}

RectObject::RectObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness )
	: theNameString(aDisplayName), theToolTipString(aTooltip)
{
	theProps.setProperty(Property::IMAGE_NAME_STRING, aImageName);
	setTheWidth(aWidth);
	setTheHeight(aHeight);
	theProps.setDefaultPropertiesString(
		QString("%1:%2/").arg(Property::MASS_STRING).arg(QString::number(aMass)));
	setTheBounciness(aBounciness);
	initAttributes();
}

RectObject::~RectObject ( ) { }

//
// Methods
//


// Accessor methods
//

void RectObject::setTheWidth ( qreal new_var )
{
	BaseObject::setTheWidth(new_var);
	DEBUG5("RectObject::setTheWidth (%f)\n", getTheWidth());
	adjustParameters();
}

void RectObject::setTheHeight ( qreal new_var )
{
	BaseObject::setTheHeight(new_var);
	DEBUG5("RectObject::setTheHeight (%f)\n", getTheHeight());
	adjustParameters();
}


// Other methods
//

void RectObject::adjustParameters(void)
{

	clearShapeList();

	if (getTheWidth()/getTheHeight() > ASPECT_RATIO)
	{
		adjustWideParametersPart();
	}
	else
	{
		if (getTheWidth()/getTheHeight() < 1/ASPECT_RATIO)
		{
			adjustTallParametersPart();
		}
		else
		{
			DEBUG5("RectObject::adjustParameters wxh=%fx%f\n", getTheWidth(),getTheHeight());
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
	}

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}

void RectObject::adjustTallParametersPart(void)
{
	// we know that height/width > ASPECT_RATIO
	// so use multiple shapes to fix that
	int myNrOfElements = ceil(getTheHeight()/(ASPECT_RATIO*getTheWidth()));
	// also, calculate the density correctly - each shape has same density
	float myMass = 0.0;
	theProps.property2Float(Property::MASS_STRING, &myMass);
	qreal myDensity = myMass / (getTheWidth()*getTheHeight());

	qreal myBaseElemHeight = ASPECT_RATIO*getTheWidth();
	qreal myDoneHeight = 0.0;
	for (int i=0; i<myNrOfElements; i++)
	{
		b2PolygonShape* myBoxShape = new b2PolygonShape();

		// make sure *not* to have a small last part - i.e. the last two parts
		// average their height.
		float myElemHeight=myBaseElemHeight;
		if (getTheHeight()-myDoneHeight < 2.0*myBaseElemHeight)
		{
			if (getTheHeight()-myDoneHeight > 1.0*myBaseElemHeight)
			{
				myElemHeight = (getTheHeight()-myDoneHeight)/2.0;
			}
			else
			{
				myElemHeight = getTheHeight()-myDoneHeight;
			}
		}
		DEBUG5("elem % 2d: %fx%f\n", i, getTheWidth(), myElemHeight);
		myBoxShape->SetAsBox  	( getTheWidth()/2.0, myElemHeight/2.0,
							  b2Vec2(0.0, -getTheHeight()/2.0+myDoneHeight+0.5*myElemHeight), 0);
		b2FixtureDef* myBoxDef = new b2FixtureDef();
		myBoxDef->shape   = myBoxShape;
		myBoxDef->density = myDensity;
		myBoxDef->userData = this;
		setFriction(myBoxDef);
		theShapeList.push_back(myBoxDef);
		myDoneHeight += myElemHeight;
	}
}

void RectObject::adjustWideParametersPart(void)
{
	// we know that width/height > ASPECT_RATIO
	// so use multiple shapes to fix that
	int myNrOfElements = ceil(getTheWidth()/(ASPECT_RATIO*getTheHeight()));
	// also, calculate the density correctly - each shape has same density
	float myMass = 0.0;
	theProps.property2Float(Property::MASS_STRING, &myMass);
	qreal myDensity = myMass / (getTheWidth()*getTheHeight());

	qreal myBaseElemWidth = ASPECT_RATIO*getTheHeight();
	qreal myDoneWidth = 0.0;
	for (int i=0; i<myNrOfElements; i++)
	{
		b2PolygonShape* myBoxShape = new b2PolygonShape();

		// make sure *not* to have a small last part - i.e. the last two parts
		// average their width.
		float myElemWidth=myBaseElemWidth;
		if (getTheWidth()-myDoneWidth < 2.0*myBaseElemWidth)
		{
			if (getTheWidth()-myDoneWidth > 1.0*myBaseElemWidth)
			{
				myElemWidth = (getTheWidth()-myDoneWidth)/2.0;
			}
			else
			{
				myElemWidth = getTheWidth()-myDoneWidth;
			}
		}
		DEBUG5("elem % 2d: %fx%f\n", i, myElemWidth, getTheHeight());
		myBoxShape->SetAsBox( myElemWidth/2.0, getTheHeight()/2.0,
							  b2Vec2(-getTheWidth()/2.0+myDoneWidth+0.5*myElemWidth, 0.0), 0);
		b2FixtureDef* myBoxDef = new b2FixtureDef();
		myBoxDef->shape   = myBoxShape;
		myBoxDef->density = myDensity;
		myBoxDef->userData = this;
		setFriction(myBoxDef);
		theShapeList.push_back(myBoxDef);
		myDoneWidth += myElemWidth;
	}

}


DrawObject*  RectObject::createDrawObject(void)
{
	adjustParameters();
	return BaseObject::createDrawObject();
}

b2BodyType RectObject::getObjectType(void) const
{
	float myMass;
	if (theProps.property2Float(Property::MASS_STRING, &myMass))
		return b2_dynamicBody;
	return b2_staticBody;
}

void RectObject::initAttributes ( )
{
	theProps.setDefaultPropertiesString(
		Property::FRICTION_STRING    + QString(":/") +
		Property::RESIZABLE_STRING   + QString(":") + Property::NONE_STRING + "/" +
		Property::DESCRIPTION_STRING + QString(":/") );
	resizableInfo = NORESIZING;
}


void  RectObject::parseProperties(void)
{
	// first parse everything that BaseObject already knows about
	BaseObject::parseProperties();

	theProps.property2String(Property::OBJECT_NAME_STRING,&theNameString);

	QString myString;
	if (theProps.property2String(Property::RESIZABLE_STRING, &myString))
	{
		// we do not check for none, that's the default
		resizableInfo = NORESIZING;
		if (myString == Property::HORIZONTAL_STRING)
			resizableInfo = HORIZONTALRESIZE;
		if (myString == Property::VERTICAL_STRING)
			resizableInfo = VERTICALRESIZE;
		if (myString == Property::TOTALRESIZE_STRING)
			resizableInfo = TOTALRESIZE;
	}
	theProps.property2String(Property::DESCRIPTION_STRING, &theToolTipString);
	adjustParameters();
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
