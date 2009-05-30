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
#include "tbe_global.h"

// Constructors/Destructors
//  

MovingObject::MovingObject ( ) 
{
	DEBUG5("MovingObject::MovingObject\n");
	dMassSetZero(&theMass);

	thePlane2DJoint = dJointCreatePlane2D (getWorldID(), 0);
    dJointAttach (thePlane2DJoint, theBodyID, 0);
}

MovingObject::~MovingObject ( ) 
{
	dJointDestroy(thePlane2DJoint);
}

//  
// Methods - sorted alphabetically
//  

void MovingObject::adjustMass(dReal newmass)
{
	assert(theMass.mass!=0);
	dMassAdjust (&theMass, newmass);
	// we assume that the mass has already been assigned to a body here
}

void MovingObject::setMassBox (dReal total_mass, dReal lx, dReal ly)
{
	dMassSetBoxTotal (&theMass, total_mass, lx, ly, 0.0);
	dBodySetMass (theBodyID, &theMass);
}

void MovingObject::setMassCapsule (
		dReal total_mass, 
		Direction direction, 
		dReal radius, 
		dReal length)
{
	dMassSetCapsuleTotal (&theMass, total_mass, direction, radius, length);
	dBodySetMass (theBodyID, &theMass);
}

void MovingObject::setMassCylinder (
		dReal total_mass, 
		Direction direction, 
		dReal radius, 
		dReal length)
{
	dMassSetCylinderTotal (&theMass, total_mass, direction, radius, length);
	dBodySetMass (theBodyID, &theMass);
}

void MovingObject::setMassSphere (dReal total_mass, dReal radius)
{
	dMassSetSphereTotal (&theMass, total_mass, radius);
	dBodySetMass (theBodyID, &theMass);
}

void MovingObject::setMassTrimesh(dReal total_mass, dGeomID g)
{
	dMassSetTrimesh (&theMass, 1.0, g);
	dMassAdjust (&theMass, total_mass);
	dBodySetMass (theBodyID, &theMass);
}
