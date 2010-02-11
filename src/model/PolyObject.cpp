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

#include "PolyObject.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawObject.h"
#include "Property.h"

static const char* DEFAULT_POLYOBJECT_NAME = "PolyObject";


//// this class' ObjectFactory
class PolyObjectFactory : public ObjectFactory
{
public:
	PolyObjectFactory(void)
	{	announceObjectType(DEFAULT_POLYOBJECT_NAME, this); }
	virtual BaseObject* createObject(void) const
	{	return new PolyObject(); }
};
static PolyObjectFactory theRectObjectFactory;


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

	virtual BaseObject* createObject(void) const
	{	return new PolyObject(theDisplayName, theTooltip, theImageName, theOutline,
								theWidth, theHeight, theMass, theBounciness); }
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


static AbstractPolyObjectFactory theNewBowlingPinFactory(
	"BowlingPin2",
	QObject::tr("BowlingPin2"),
	QObject::tr("Bowling pins are meant to be run "
				"over - and most people prefer to do that using "
				"a Bowling Ball.""The famous plastic red domino stone"),
	"Bowling_Pin",
	"(0,0.17)=(-0.06,0)=(-0.03,-0.17)=(0.03,-0.17)=(0.06,0)",
	0.12, 0.34, 1.5, 0.4 );


// Constructors/Destructors
//

PolyObject::PolyObject ( ) : BaseObject(), theNameString(DEFAULT_POLYOBJECT_NAME)
{
	DEBUG5("PolyObject::PolyObject\n");

	// because this object is very flexible and many parameters can be set through
	// the Properties, do not assume too much here...

	// also: keep in mind that child objects may set some things automatically
	rotatableInfo = false;
}

PolyObject::PolyObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				const QString& anOutline,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness )
	: theNameString(aDisplayName), theToolTipString(aTooltip)
{
	theProps.setProperty(Property::IMAGE_NAME_STRING, aImageName);
	setTheWidth(aWidth);
	setTheHeight(aHeight);
	theProps.setProperty(Property::POLYGONS_STRING, anOutline);
	theProps.setProperty(Property::MASS_STRING, QString::number(aMass));
	setTheBounciness(aBounciness);
}

PolyObject::~PolyObject ( ) { }

//
// Methods
//


// Accessor methods
//

// Other methods
//

void PolyObject::parseProperties(void)
{

	BaseObject::parseProperties();

	clearShapeList();

	QString myPolygons;
	theProps.propertyToString(Property::POLYGONS_STRING, &myPolygons);
	QStringList myPolygonList = myPolygons.split(";");

	QStringList::iterator i = myPolygonList.begin();
	while (i!=myPolygonList.end())
	{
		b2PolygonDef* myPolyDef = new b2PolygonDef();

		QStringList myCoordList = (*i).split("=");
		int j = 0;
		myPolyDef->vertexCount = myCoordList.count();
		for (; j<myCoordList.count(); j++)
		{
			assert (j < b2_maxPolygonVertices);
			Vector myCoord;
			assert (myCoord.fromString(myCoordList.at(j)) == true);
			myPolyDef->vertices[j]=myCoord.toB2Vec2();
		}

		// get mass:  no mass -> no density -> no motion
		float myMass;
		if (theProps.propertyToFloat(Property::MASS_STRING, &myMass))
			myPolyDef->density = myMass / getTheWidth()*getTheHeight();
		myPolyDef->userData = this;
		setFriction(myPolyDef);
		theShapeList.push_back(myPolyDef);
		++i;
	}

	createPhysicsObject();

}

DrawObject*  PolyObject::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	theDrawObjectPtr = new DrawObject(this, theProps.getProperty(Property::IMAGE_NAME_STRING));
	return theDrawObjectPtr;
}


void  PolyObject::setFriction(b2PolygonDef* aBoxDef)
{
	// only set friction if it is special
	if (theProps.getProperty(Property::FRICTION_STRING).isEmpty())
		return;

	bool isOK = false;
	double myFriction = theProps.getProperty(Property::FRICTION_STRING).toDouble(&isOK);
	if (isOK)
		aBoxDef->friction = myFriction;
	else
		assert(false);
}
