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

#include "AbstractBall.h"
#include "DrawObject.h"
#include "tbe_global.h"
#include "Box2D.h"


/** the AbstractBall's ObjectFactory
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

	virtual BaseObject* createObject(void) const
	{	return new AbstractBall(theDisplayName, theTooltip, theImageName,
								theRadius, theMass, theBounciness); }
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
static BallObjectFactory theBBFactory("Bowling Ball",
	QObject::tr("Bowling Ball"),
	QObject::tr("Your average bowling ball - heavy, round and willing to roll"),
	"Bowling_Ball", 0.11, 6.0, 0.1 );

// we are lazy and do not model the air, we assume it to be uniform in mass
static BallObjectFactory theVBFactory("Volleyball",
	QObject::tr("Volley Ball"),
	QObject::tr("Your average volleyball - it's light, soft and fairly bouncy."),
	"VolleyBall", 0.105, 0.280, 0.8);


// the official standards say that a tennis ball dropped from 100 inch should bounce 53-58 inch.
// thanks to http://en.wikipedia.org/wiki/Tennis_ball
// we are lazy and do not model the air, we assume it to be uniform in mass
static BallObjectFactory theTBFactory("Tennis Ball",
	QObject::tr("Tennis Ball"),
	QObject::tr("A tennis ball is small, hairy and fairly bouncy."),
	"TennisBall", 0.033, 0.058, 0.56);


// Constructors/Destructors
//  

AbstractBall::AbstractBall (const QString& aName,
				  const QString& aTooltip,
				  const QString& anImageName,
				  qreal aRadius,
				  qreal aMass,
				  qreal aBounciness)
	: theBallName(aName), theBallTooltip(aTooltip), theBallImage(anImageName)
{
	DEBUG5("AbstractBall::AbstractBall\n");

	b2CircleDef* ballDef = new b2CircleDef();
	ballDef->radius = aRadius;
	ballDef->density = aMass/(PI*aRadius*aRadius);
	theShapeList.push_back(ballDef);

	setTheWidth(2.0*aRadius);
	setTheHeight(2.0*aRadius);
	setTheBounciness(aBounciness);
}

AbstractBall::~AbstractBall ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

DrawObject*  AbstractBall::createDrawObject(void)
{
	DrawObject* myObjPtr = new DrawObject(this, theBallImage);
	myObjPtr->setZValue(10.0);
	return myObjPtr;
}


