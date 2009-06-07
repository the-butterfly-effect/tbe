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

#include <QString>
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
	 * Simple Constructor
	 */
	World (void);

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
	 *  Note that due to a limitation in OpenDE, each step is fixed time 
	 * 
	 * @return the time step taken in the simulation step in seconds 
	 *         (most likely only a fraction of a second)
	 */
	dReal simStep (void);

	
public:
    /** returns the name of the Level that created the World
     */
    const QString& getName(void) const
		{ return theLevelName; } 

    void setLevelName(const QString& aName)
		{ theLevelName = aName; }
    
    // Public attribute accessor methods

	void setTheWorldSize(dReal aWidth, dReal aHeight)
		{ theWorldWidth=aWidth; theWorldHeight=aHeight; }
	dReal getTheWorldWidth(void)
		{ return theWorldWidth; }
	dReal getTheWorldHeight(void)
		{ return theWorldHeight; }

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
	
	QString theLevelName;

	dReal	theWorldWidth;
	dReal	theWorldHeight;
};

#endif // WORLD_H
