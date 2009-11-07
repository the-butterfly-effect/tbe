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

#include "BowlingBall.h"
#include "DrawObject.h"
#include "tbe_global.h"
#include "Box2D.h"



// this class' ObjectFactory
class BowlingBallObjectFactory : public ObjectFactory
{
public:
	BowlingBallObjectFactory(void)
	{	announceObjectType("Bowling Ball", this); }
	virtual BaseObject* createObject(void) const
	{	return new AbstractBall("Bowling Ball", "Your average bowling ball - heavy, "
				"round and willing to roll", "Bowling_Ball", 0.11, 6.0, 0.1 ); }
};
static BowlingBallObjectFactory theBFactory;



// this class' ObjectFactory
class VolleyBallObjectFactory : public ObjectFactory
{
public:
	VolleyBallObjectFactory(void)
	{	announceObjectType("Volleyball", this); }
	virtual BaseObject* createObject(void) const
	{	return new AbstractBall("Volley Ball", "Your average volleyball - it's light, "
				"soft and fairly bouncy.", "VolleyBall", 0.105, 0.280, 0.8); }
};
static VolleyBallObjectFactory theVFactory;





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
{ return new DrawObject(this, theBallImage); }


