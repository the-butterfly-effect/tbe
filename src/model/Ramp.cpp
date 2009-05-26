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

// Constructors/Destructors
//  

Ramp::Ramp ( ) 
{
	DEBUG5("Ramp::Ramp\n");
	
	// the RAMP is defined as a bar from top left to bottom right.
	// for the whole of the width of the block - which is 1.0 for now
	// and the whole heigth of the block - which happens also to be 1.0
	
	setTheGeomID( dCreateBox (getSpaceID(), SQRT2, 0.03, 1.0) );
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


// Other methods
//  


