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

#include "BowlingPin.h"
#include "tbe_global.h"

// Constructors/Destructors
//  

BowlingPin::BowlingPin ( ) 
{
	DEBUG5("BowlingPin::BowlingPin\n");
	// set the bowling pin to be a cylinder
	// radius = 0.06 m
	// total length of the pin is 0.38m - i.e. "length" is 0.24m.
	// the weight of a pin is standardized at 1.5 kg.
	setTheGeomID( dCreateCylinder (getSpaceID(), 0.06, 0.12) );
	setMassCylinder(1.5, AlongYAxis, 0.06, 0.12);
	setTheBounciness(0.8);
}

BowlingPin::~BowlingPin ( )
{
}

//  
// Methods
//  
