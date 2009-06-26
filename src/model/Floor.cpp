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

#include "Floor.h"
#include "tbe_global.h"
#include <cmath>
#include "DrawRamp.h"

//// this class' ObjectFactory
class FloorObjectFactory : public ObjectFactory
{
public:
	FloorObjectFactory(void)
	{	announceObjectType("Floor", this); }
	virtual BaseObject* createObject(void) const
	{	return new Floor(); }
};
static FloorObjectFactory theFactory;

// Constructors/Destructors
//  

Floor::Floor ( ) 
{
	DEBUG5("Floor::Floor\n");
	

	setTheGeomID( dCreateBox (getSpaceID(), 1.0, theSlabThickness, 1.0) );
	setTheBounciness(0.2);
	
	setTheHeight(theSlabThickness);
	setTheWidth(1.0);
	setAngle(0.0);
	
}

Floor::~Floor ( ) { }

//  
// Methods
//  


// Accessor methods
//  

void Floor::setTheWidth ( qreal new_var )
{
	// adjusting the width also implies that the slap changes rotation and size
	if (new_var <= 0.1)
		return;
	BaseObject::setTheWidth(new_var);
	adjustParameters();
}

// Other methods
//  

void Floor::adjustParameters(void)
{
	// width or height was just adjusted
	// let's recalculate everything
	
	dGeomBoxSetLengths(getTheGeomID(), getTheWidth(), theSlabThickness, 1.0 );
}

DrawObject*  Floor::createDrawObject(void)
{ return new DrawRamp(this); }
