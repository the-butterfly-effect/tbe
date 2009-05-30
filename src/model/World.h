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

#include <QList>
#include "ode/ode.h"
#include "tbe_global.h"

// Forward Definitions:
class BaseObject;
class MainWindow;
class DrawWorld;

/**
  * class World
  *
  * the class holding all BaseObjects and is responsible for the simulation 
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

	
	/** adds object to the World.
	 *  @param anObjectPtr
	 *  @return true if success - false if object already present
	 */
	bool addObject(BaseObject* anObjectPtr);
    
	/** creates the corresponding DrawWorld and asks it to
	 *  sets itself up as the GraphicsScene
	 *  it will add all known BaseObjects to the Scene
	 */
    void createScene(MainWindow* myMainPtr);
	
	/// keep the scene, set all objects back in original position
	void reset (void);
	
	/** take one step in the simulation loop 
	 * 
	 * @return the time step taken in the simulation step
	 */
	dReal simStep (void);

	
public:
	// Public attribute accessor methods

private:
	/** this is called by dSpaceCollide when two objects in space are
	 *  potentially colliding.
	 *  if dSpaceCollide is used correctly, theDataPtr actually contains
	 *  the address of the real instance of World - this is a static member!
	 *  the actual code is in nearCallbackReal().
	 *  @param theDataPtr - pointer to a World instance
	 *  @param o1 first geometry that collides
	 *  @param o2 second geometry that collides
	 */
	static void nearCallbackStatic (void* theDataPtr, dGeomID o1, dGeomID o2);
	
	/** this is called by dSpaceCollide when two objects in space are
	 *  potentially colliding - see also the nearCallbackStatic() member
	 *  @param o1 first geometry that collides
	 *  @param o2 second geometry that collides
	 */
	void nearCallbackReal(dGeomID o1, dGeomID o2);

	/** this member tries to figure out the bounciness of the geometry
	 * 
	 * @param b1 the geometry to query for bounciness
	 * @return the bounciness
	 */
	static dReal getBounce(dGeomID b1);

	void initAttributes ( ) ;
	
private:	
	// Private attributes
	//

	typedef QList<BaseObject*> BaseObjectPtrList;
	
	/// the list of all objects managed by this World 
	BaseObjectPtrList theObjectPtrList;
	
	/// pointer to the associated VIEW class of World
	DrawWorld* theDrawWorldPtr;

	// The three attributes that make World tick: 
	// 
	
	dWorldID theGlobalWorldID;
	dSpaceID theGlobalSpaceID;
	dJointGroupID contactgroup1;
	
	/// the time taking in each time step
	static const dReal deltaTime = 0.005;

};

#endif // WORLD_H
