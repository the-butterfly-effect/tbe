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

#include "BaseObject.h"

// Constructors/Destructors
//  

BaseObject::BaseObject ( ) 
{
	initAttributes();
}

BaseObject::~BaseObject ( ) 
{
	// destroy the Geometry
	dGeomSetData(theGeomID, NULL);
	dGeomDestroy(theGeomID);
	theGeomID = NULL;
	
	// destroy the Body
	dBodyDestroy(theBodyID);
	theBodyID = NULL;
	
	// mass will be destroyed in MovingObject
	
	// any joints will be destroyed in the implementation class
}

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void BaseObject::initAttributes ( ) 
{
	DEBUG5("BaseObject::initAttributes\n");
	theBodyID = dBodyCreate (theGlobalWorldID);

	theScale = 1.0;
	theWidth = 1.0;
	theHeight = 1.0;
	// don't need to initialise theCenter - it has a default constructor
}
