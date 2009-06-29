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

/**
  * class MovingObject
  * 
  * This class inherits from BaseObject, but because it does not implement 
  * any of the pure virtual members of BaseObject, 
  * it is an abstract class itself
  * 
  * MovingObject adds one concept to BaseObject:
  *   * Mass
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
	 * Get the total mass of the object in kg
	 * @return the value of theMass in kg
	 */
	qreal getTotalMass(void);

protected:
	/// modifies the EXISTING total mass across all shapes to be newmass - in kg
	void setTotalMass(qreal newmass);

};

#endif // MOVINGOBJECT_H
