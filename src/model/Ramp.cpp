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

Ramp::Ramp ( ) 
{
	DEBUG5("Ramp::Ramp\n");
	
	// the RAMP is defined as a bar from top left to bottom right.
	// for the whole of the width of the block - which is 1.0 for now
	// and the whole heigth of the block - which happens also to be 1.0
	
	setTheBounciness(0.2);
	
	setTheWidth(1.0);
	setTheHeight(1.0);
	setAngle(-PI/4.0);
	
}

Ramp::~Ramp ( ) { }

//  
// Methods
//  


// Accessor methods
//  

void Ramp::setTheWidth ( qreal new_var )
{
	// adjusting the width also implies that the slap changes rotation and size
	if (new_var <= 0.01)
		return;
	BaseObject::setTheWidth(new_var);
	adjustParameters();
}

void Ramp::setTheHeight ( qreal new_var )
{
	//adjusting the height also implies that the slap changes rotation and size
	if (new_var <= 0.01)
		return;
	BaseObject::setTheHeight(new_var);
	adjustParameters();	
}


// Other methods
//  

void Ramp::adjustParameters(void)
{
	// note that the angle is not used at all here !!!
	qreal myHW = getTheWidth()/2.0;
	qreal myHH = getTheHeight()/2.0;
	
	b2PolygonDef* rampDef = new b2PolygonDef();
	rampDef->vertexCount = 4;
	rampDef->vertices[0].Set(-myHW, +myHH);
	rampDef->vertices[1].Set(+myHW, -myHH+theSlabThickness);
	rampDef->vertices[2].Set(+myHW, -myHH);
	rampDef->vertices[3].Set(-myHW, +myHH-theSlabThickness);
	// ramp is immovable -> no mass -> no density 
	rampDef->density = 0.0;
	
	// delete any shapes on the body
	// and create a new shape from the above polygon def
	clearShapeList();
	theShapeList.push_back(rampDef);
}

DrawObject*  Ramp::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	theDrawObjectPtr = new DrawRamp(this);
	return theDrawObjectPtr;
}
