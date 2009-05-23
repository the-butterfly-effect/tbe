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

#ifndef WORLD_H
#define WORLD_H

#include <assert.h>
#include <qstring.h>
#include "ode/ode.h"
#include "tbe_global.h"

#include "BaseObject.h"

/**
  * class World
  *
  * the class holding all BaseObjects and doing the simulation 
  */

class World
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	World ( );

	/**
	 * Empty Destructor
	 */
	virtual ~World ( );

	
	void start (void);
	void stop  (void);
	void reset (void);
	

	/// simulation loop
	void simLoop (void);

private:	
	/// this is called by dSpaceCollide when two objects in space are
	/// potentially colliding.
	static void nearCallback (void *data, dGeomID o1, dGeomID o2);
	

	static dReal getBounce(dBodyID b1);
	
	// Public attribute accessor methods
	//  

private:
	// Private attributes
	//
	
public:
	// Public attribute accessor methods

private:

	void initAttributes ( ) ;

};

#endif // BASEOBJECT_H
