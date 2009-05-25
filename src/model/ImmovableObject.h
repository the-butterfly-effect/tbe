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

#ifndef IMMOVABLEOBJECT_H
#define IMMOVABLEOBJECT_H
#include "BaseObject.h"

#include <qstring.h>
#include "ode/ode.h"

/**
  * class ImmovableObject
  * 
  * This class inherits from BaseObject, but because it does not implement 
  * any of the pure virtual members of BaseObject, 
  * it is an abstract class itself
  * 
  * the class itself "fixes" BaseObject so we can create objects without body.
  * Such objects are 100% immovable - OpenDE rule: no body, no motion
  */

class ImmovableObject : virtual public BaseObject
{
public:
	
	// Constructors/Destructors
	//
	
	/**
	 * Empty Constructor
	 */
	// TODO: figure parameters out
	ImmovableObject (  );
	
	/**
	 * Empty Destructor
	 */
	virtual ~ImmovableObject ( );

	// Getters and Setters
	// 
	
	/// resets the object into the start position/situation
	virtual void reset(void);
	
	/**
	 * Set the value of theGeomID
	 * @param new_var the new value of theGeomID
	 */
	virtual void setTheGeomID ( dGeomID new_var )				
	{
		assert(theGeomID==0);
		theGeomID = new_var;
		
		// immovable objects do not have a body
		/*dGeomSetBody (theGeomID, theBodyID);*/
	}
};

#endif // IMMOVABLEOBJECT_H
