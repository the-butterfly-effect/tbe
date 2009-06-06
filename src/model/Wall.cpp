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

#include "Wall.h"
#include "tbe_global.h"
#include <cmath>
#include "DrawRamp.h"

//// this class' ObjectFactory
class WallObjectFactory : public ObjectFactory
{
public:
	WallObjectFactory(void)
	{	announceObjectType("Wall", this); }
	virtual BaseObject* createObject(void) const
	{	return new Wall(); }
};
static WallObjectFactory theFactory;

// Constructors/Destructors
//  

Wall::Wall ( ) 
{
	DEBUG5("Wall::Wall\n");
	
	setTheGeomID( dCreateBox (getSpaceID(), theSlabThickness, 1.0, 1.0) );
	setTheBounciness(0.7);
	
	BaseObject::setTheWidth(theSlabThickness);
	BaseObject::setTheHeight(1.0);
	BaseObject::setAngle(0.0);
}

Wall::~Wall ( ) { }

//  
// Methods
//  


// Accessor methods
//  

void Wall::setTheHeight ( dReal new_var )
{
	//adjusting the height also implies that the slap changes rotation and size
	if (new_var <= 0.01)
		return;
	BaseObject::setTheHeight(new_var);
	dGeomBoxSetLengths(getTheGeomID(), theSlabThickness, new_var, 1.0 );
}


// Other methods
//  

DrawObject*  Wall::createDrawObject(void)
{ return new DrawObject(this); }
