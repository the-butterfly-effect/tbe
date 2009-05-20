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

#ifndef POSITION_H
#define POSITION_H

#include "ode/ode.h"

/**
  * class Position
  * 
  * This class abstracts the Position - x,y,angle
  */

class Position
{
public:
	
	// Constructors/Destructors
	
	/**
	 * Empty Constructor - set x (m), y(m), angle(radial)
	 */
	Position (dReal anX=0.0, dReal aY=0.0, dReal anAngle=0.0);
	
	// Public attributes
	
	/// x-coordinate: x=0 is lower left corner, positive is right. unit: meter
	dReal x;
	
	/// y-coordinate: y=0 is lower left corner, positive is up. unit: meter
	dReal y;
	
	/// angle coordinate: angle=0 is positive x direction, turning counterclockwise. unit: 2Pi for a full turn
	dReal angle;
};

#endif // POSITION_H
