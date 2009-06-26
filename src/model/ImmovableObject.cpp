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

#include "ImmovableObject.h"
#include <assert.h>
#include "tbe_global.h"

// Constructors/Destructors
//  

ImmovableObject::ImmovableObject ( ) 
{
	DEBUG5("ImmovableObject::ImmovableObject\n");
	// BaseObject just allocated a BodyID - let's remove it
	dBodyDestroy(theBodyID);
	theBodyID = NULL;
}

ImmovableObject::~ImmovableObject ( ) 
{
	// nothing to do here
}

//  
// Methods - sorted alphabetically
//  

void ImmovableObject::reset ( ) 
{
	Position myCenter = getOrigCenter();
	DEBUG5("BaseObject::reset() geom pos for '%s' to (%f,%f)@%f\n", 
			getName().toAscii().constData(), myCenter.x, myCenter.y, myCenter.angle);
	dGeomSetPosition(theGeomID, myCenter.x, myCenter.y, 0.0);
	dMatrix3 R;
	dRFromAxisAndAngle (R, 0.0, 0.0, 1.0, myCenter.angle);
	dGeomSetRotation(theGeomID, R);
}
