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

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <assert.h>
#include <QString>
#include "ode/ode.h"
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class DrawObject;
class ObjectFactory;
class World;


//   ************************************************
//   *                                              *
//   * NOTE: the ObjectFactory class declaration is *
//   *       below the BaseObject declaration       *
//   *                                              *
//   ************************************************


/* ABOUT BODIES, GEOMETRIES, MASSES AND THE BASEOBJECT CLASS
   In OpenDE, we have Worlds, Bodies, Geometries, Joints and Masses
	 * World    - the "space" all bodies live in
	 * Body     - the concept of an object in the world
	 * Mass     - the mass of the total body
	 * Geometry - the dimensions of a body
	 * Joint    - the link between two bodies, constraining their relative movement
   The Geometry is very relevant to collision detection: no geometry, no collision
	 It is possible to have multiple Geometries on a single Body - useful for making more complex shaped objects
   The Body is very relevant to motion: no body, no movement
	 The ImmovableObject class uses this trick: it defines a geom without a body
   The Mass is very relevant to acceleration/deceleration Note that a mass can have a different shape than a geometry!
	 Mass is created and managed in the MovingObject class

   When a BaseObject is register()ed, the body, all geoms and the mass are created
   When a BaseObject is deregister()ed, the body, all geoms and the mass have to be deleted
*/

/**
  * class BaseObject
  *
  * abstract base class for everything in the simulation model 
  */

class BaseObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	BaseObject ( );

	/**
	 * Empty Destructor
	 */
	virtual ~BaseObject ( );

	// Public attribute accessor methods
	//  

	enum SizeDirections
	{
		NORESIZING = 0,
		HORIZONTALRESIZE = 1,
		VERTICALRESIZE = 2,
		TOTALRESIZE = 3
	};
	
	/// returns the Name of the object.
	virtual const QString getName ( ) const = 0;

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const = 0;

	/// returns true if the object can be moved by the user
	virtual bool isMovable ( ) const
		{ return theIsMovable; }
	
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const = 0;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const = 0;

	/// resets the object into the start position/situation
	virtual void reset(void);

//	bool deregister(void);
//	bool register(void);

	/** the two STATIC members in this class should only be accessible to the
	 *  World class
	 */
	class ForWorldOnly
	{
	private:
		friend class World;
	
		/// static member to set the world ID where objects should be in
		static void setTheWorldID(dWorldID anID);
		
		/// static member to set the space ID where objects should be in
		static void setTheSpaceID(dSpaceID anID);
	};

	// TODO: FIXME: rogue pointer here!!!
	World* theWorldPtr;

	virtual DrawObject* createDrawObject();
		
private:
	// Private attributes
	//
	
	/** the position of the center of the object. Note that this attribute
	 *  is not updated during simulations, so we don't need to reset it
	 */
	Position theCenter;
	
	/// width of the object. If object is resizable, this can be changed by user 
	dReal theWidth;
	
	/// height of object. If object is resizable, this can be changed by user
	dReal theHeight;
	
	/** indicates how object should bounce - two objects of 0.0 will stick 
	*  together, whereas two objects of 1.0 will bounce without loosing
	*  energy. Bounciness over 1.0 will resulting in ever larger bounces
	*  (not really realistic, right?)
	*/
	dReal theBounciness;

	/** true if the user can move this object
	 *  note that this has nothing to do with MovingObject or ImmovableObject
	 *  it has to do with the level design - as such, Level can modify this setting
	 */
	bool theIsMovable;
	
protected:
	dBodyID theBodyID;
	dGeomID theGeomID;

	dWorldID getWorldID(void);
	dSpaceID getSpaceID(void);
	
	/// pointer to a DrawObject that will draw this object
	DrawObject* theDrawObjectPtr;
	
public:
	// public attribute accessor methods

	/**
	 * Sets whether the object can be moved by the user
	 * @param aNewState true if movable, false if not
	 */
	void setIsMovable(bool aNewState)
		{ theIsMovable = aNewState; }
	
	/**
	 * Set the value of theCenter
	 * @param new_var the new value of theCenter
	 */
	void setTheCenter ( Position new_var )
	{
		theCenter.x = new_var.x;
		theCenter.y = new_var.y;
		if (isRotatable())
			theCenter.angle = new_var.angle;
	}

	/**
	 * Get the value of theCenter
	 * @return the value of theCenter
	 */
	Position getTheCenter ( )
		{ return theCenter; }

	/**
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( dReal new_var )
		{ if (new_var>0.01) theWidth = new_var; }

	/**
	 * Get the value of theWidth
	 * @return the value of theWidth
	 */
	dReal getTheWidth ( )
		{ return theWidth; }

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( dReal new_var )
		{ if (new_var>0.01) theHeight = new_var;	}

	/**
	 * Get the value of theHeight
	 * @return the value of theHeight
	 */
	dReal getTheHeight ( )
		{ return theHeight; }

	/**
	 * Set the value of theBodyID
	 * @param new_var the new value of theBodyID
	 */
	void setTheBodyID ( dBodyID new_var )
		{ theBodyID = new_var; }

	/**
	 * Get the value of theBodyID
	 * @return the value of theBodyID
	 */
	virtual dBodyID getTheBodyID ( )
		{ return theBodyID; }

	/**
	 * Set the value of theBounciness 
	 *   (0.0 = stick, 1.0 = full elastic bounce)
	 * @param new_var the new value of theBounciness
	 */
	void setTheBounciness ( dReal new_var )
		{ theBounciness = new_var; }

	/**
	 * Get the value of theBounciness
	 *   (0.0 = stick, 1.0 = full elastic bounce)
	 * @return the value of theBounciness
	 */
	dReal getTheBounciness ( )				 
		{ return theBounciness; }

	/**
	 * Set the value of theGeomID
	 * @param new_var the new value of theGeomID
	 */
	virtual void setTheGeomID ( dGeomID new_var );

	/**
	 * Get the value of theGeomID
	 * @return the value of theGeomID
	 */
	dGeomID getTheGeomID ( )
		{ return theGeomID; }

protected:
	void setAngle(dReal anAngle)
		{ theCenter.angle = anAngle; }
	
private:

	void initAttributes ( ) ;

};

//////////////////////////////////////////////////////////////////////////////

/**
 *  class ObjectFactory
 *  
 *  This class will be implemented for each "final" Object type.
 *  There should be a static instance in each Object's cpp file. 
 *  At system start it will announce the type to Level and do the real Object creation.
 */
class ObjectFactory
{
	// there's nothing public here - nobody should call anything in this class
	// directly.
	
protected:
	/** create the object aName and return a pointer to it
	 * @param aName
	 * @param aPostion
	 * @param aWidth    width of the object in meter - default 1.0
	 * @param anHeight  height of the object in meter - default 1.0
	 * @return a valid pointer to a newly created object or NULL if not found
	 */
	BaseObject* createObject(
			const QString& aName, 
			const Position aPostion, 
			const dReal aWidth = 1.0,
			const dReal anHeight = 1.0) const;
	
	static void announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr);
	
	/** pure virtual function that creates an object of the type the factory is for
	 *  must be implemented for each real factory.
	 * @return pointer to a newly instantiated object
	 */
	virtual BaseObject* createObject(void) const = 0;
};

#endif // BASEOBJECT_H
