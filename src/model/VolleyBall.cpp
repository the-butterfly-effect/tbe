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

#include "VolleyBall.h"
#include "DrawObject.h"
#include "tbe_global.h"
#include "Box2D.h"


// this class' ObjectFactory
class VolleyBallObjectFactory : public ObjectFactory
{
public:
	VolleyBallObjectFactory(void)
	{	announceObjectType("VolleyBall", this); }
	virtual BaseObject* createObject(void) const
	{	return new VolleyBall(); }
};
static VolleyBallObjectFactory theFactory;

// Constructors/Destructors
//  

VolleyBall::VolleyBall ( )
{
	DEBUG5("VolleyBall::VolleyBall\n");
	// volleyball circumference is 66 cm -> 21 cm diameter
	const qreal myRadius = 0.105;
	// and a weight of 280 grams
	const qreal myMass = 0.280; // kg

	b2CircleDef* ballDef = new b2CircleDef();
	
	ballDef->radius = myRadius;

	ballDef->density = myMass/(PI*2*myRadius);

	// delete any shapes on the body
	// and create a new shape from the above polygon def
	theShapeList.push_back(ballDef);
	
	setTheWidth(2.0*myRadius);
	setTheHeight(2.0*myRadius);
	setTheBounciness(0.75);
}

VolleyBall::~VolleyBall ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

DrawObject*  VolleyBall::createDrawObject(void)
{ return new DrawObject(this, "VolleyBall"); }
