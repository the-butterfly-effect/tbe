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

#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H
#include "BaseObject.h"

#include <qstring.h>
#include "ode/ode.h"

/**
  * class MovingObject
  * 
  * This class inherits from BaseObject, but because it does not implement 
  * any of the pure virtual members of BaseObject, 
  * it is an abstract class itself
  * 
  * MovingObject adds two concepts to BaseObject:
  *   * Mass
  *   * Fixation to the Z=0 plane 
  *     TODO: force-feedback from joint not implemented!!!
  */

class MovingObject : virtual public BaseObject
{
public:
	
	// Constructors/Destructors
	//
	
	/**
	 * Empty Constructor
	 */
	MovingObject (  );
	
	/**
	 * Empty Destructor
	 */
	virtual ~MovingObject ( );

	// Getters and Setters
	// 
	
	/**
	 * Get the value of theMass
	 * @return the value of theMass in kg
	 */
	dReal getTheMass(void)
	{
		return theMass.mass;
	}

protected:
	enum Direction
	{
		AlongXAxis = 1,
		AlongYAxis = 2,
		AlongZAxis = 3
	};
	
	/** sets the object's mass assuming rectangle geometry
	 * @param total_mass in kg
	 * @param lx length in X axis
	 * @param ly length in Y axis
	 */
	void setMassBox (dReal total_mass, dReal lx, dReal ly);

	/** sets the object's mass assuming capsule geometry
	 *  note that this one does have a direction  
	 * @param total_mass in kg
	 * @param direction (see Direction enum)
	 * @param radius in meter
	 * @param length (not counting caps) in meter
	 */
	void setMassCapsule (dReal total_mass, Direction direction, dReal radius, dReal length);

	/** sets the object's mass assuming cylinder geometry
	 *  note that this one does have direction
	 * @param total_mass in kg
	 * @param direction (see Direction enum)
	 * @param radius in meter
	 * @param length in meter
	 */
	void setMassCylinder (dReal total_mass, Direction direction, dReal radius, dReal length);

	/// sets the object's mass in kg, assumes sphere form with radius in meter
	void setMassSphere (dReal total_mass, dReal radius);

	void setMassTrimesh(dReal total_mass, dGeomID g);
	
	/// modifies the EXISTING mass to be newmass - in kg
	void adjustMass(dReal newmass);

protected:
	dMass theMass;
	dJointID thePlane2DJoint;
};

#endif // MOVINGOBJECT_H
