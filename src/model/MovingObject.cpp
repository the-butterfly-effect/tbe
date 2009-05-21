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

#include "MovingObject.h"
#include <assert.h>

// Constructors/Destructors
//  

MovingObject::MovingObject ( ) 
{
	dMassSetZero(&theMass);
}

MovingObject::~MovingObject ( ) 
{
	// nothing to do here
}

//  
// Methods - sorted alphabetically
//  

void MovingObject::adjustMass(dReal newmass)
{
	assert(theMass.mass!=0);
	dMassAdjust (&theMass, newmass);
}

void MovingObject::setMassBox (dReal total_mass, dReal lx, dReal ly)
{
	dMassSetBoxTotal (&theMass, total_mass, lx, ly, 0.0);
}

void MovingObject::setMassCapsule (dReal total_mass, int direction, dReal radius, dReal length)
{
	dMassSetCapsuleTotal (&theMass, total_mass, direction, radius, length);
}

void MovingObject::setMassSphere (dReal total_mass, dReal radius)
{
	dMassSetSphereTotal (&theMass, total_mass, radius);
}

void MovingObject::setMassTrimesh(dReal total_mass, dGeomID g)
{
	dMassSetTrimesh (&theMass, 1.0, g);
	dMassAdjust (&theMass, total_mass);
}
