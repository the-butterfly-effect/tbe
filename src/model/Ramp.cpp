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

#include "Ramp.h"
#include "tbe_global.h"
#include "DrawRamp.h"
#include "Box2D.h"

//// this class' ObjectFactory
class RampObjectFactory : public ObjectFactory
{
public:
	RampObjectFactory(void)
	{	announceObjectType("Ramp", this); }
	virtual BaseObject* createObject(void) const
	{	return new Ramp(); }
};
static RampObjectFactory theFactory;

// Constructors/Destructors
//  

Ramp::Ramp ( ) : BaseObject()
{
	DEBUG5("Ramp::Ramp\n");
	
	// the RAMP is defined as a bar from top left to bottom right.
	// for the whole of the width of the block - which is 1.0 for now
	// and the whole heigth of the block - which happens also to be 1.0
	
	setTheBounciness(0.2);
	adjustParameters();
}

Ramp::~Ramp ( ) { }

//  
// Methods
//  


// Accessor methods
//  

void Ramp::setTheWidth ( qreal new_var )
{
	BaseObject::setTheWidth(new_var);
	DEBUG5("Ramp::setTheWidth (%f)\n", getTheWidth());
	adjustParameters();
}

void Ramp::setTheHeight ( qreal new_var )
{
	BaseObject::setTheHeight(new_var);
	DEBUG5("Ramp::setTheHeight (%f)\n", getTheHeight());
	adjustParameters();
}


// Other methods
//  

void Ramp::adjustParameters(void)
{
	// note that the angle property is totally ignored here
	// Ramps cannot rotate.
	qreal myHW = getTheWidth()/2.0;
	qreal myHH = getTheHeight()/2.0;

	DEBUG5("Ramp::adjustParameters wxh=%fx%f\n", myHW, myHH);
	
	b2PolygonDef* rampDef = new b2PolygonDef();
	// make sure to make the vertex run counter clockwise
	rampDef->vertexCount = 4;
	rampDef->vertices[0].Set(-myHW, myHH);
	rampDef->vertices[1].Set(-myHW, myHH-4*theSlabThickness);
	rampDef->vertices[2].Set(+myHW,-myHH);
	rampDef->vertices[3].Set(+myHW,-myHH+theSlabThickness);

	// ramp is immovable -> no mass -> no density
	rampDef->density = 0.0;
	
	// delete any shapes on the body
	// and create a new shape from the above polygon def
	clearShapeList();
	theShapeList.push_back(rampDef);

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}

DrawObject*  Ramp::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawRamp(this);
	return theDrawObjectPtr;
}
