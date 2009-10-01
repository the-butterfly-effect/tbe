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

#include "tbe_global.h"
#include "Position.h"

#include <assert.h>
#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

// Forward Declarations
class DrawObject;
class ObjectFactory;
class World;
class ShapeList;
class b2Body;
class b2BodyDef;
class b2ContactPoint;
class b2World;
class b2ShapeDef;
class b2Shape;
class BaseObjectSerializer;

//   ************************************************
//   *                                              *
//   * NOTE: the ObjectFactory class declaration is *
//   *       below the BaseObject declaration       *
//   *                                              *
//   ************************************************


class SensorInterface
{
public:
	/// called if Object has registered a sensor share
	/// the default is to do completely nothing - you'll have to override
	virtual void callBackSensor(b2ContactPoint*)
	{ return; }
};


// ABOUT BODIES, SHAPES, JOINTS AND THE BASEOBJECT CLASS
//
//   In Box2D, we have Worlds, Bodies, Shapes and Joints:
//
//	 * World    - the "space" all bodies live in
//	 * Body     - the concept of an object in the world. 
//	 *            Mass is linked to Body, no mass implied a rigid, static body
//	 * Shape    - the shape of a body - a series of convex polygons or circles
//	 *            It is possible to use more than one shape in a body
//	 * Joint    - the link between two bodies, constraining relative movement
//
//	At instance creation, the bodydef and the shapedef will be created
//	At instance creation, also the body and shapes will be created
//
//	Upon deletion, body and shape are deleted but the defs are kept
//	Upon undelete, body and shape are re-created based on the defs 
//

/**
  * class BaseObject
  *
  * abstract base class for everything in the simulation model 
  */

class BaseObject : public SensorInterface
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

	/** returns true if the object is sleeping
	 *  sleeping objects do not cost calculation time
	 *  DrawObject uses this as a debugging aid
	 *  
	 * @return true if B2Body actually exists *and* is sleeping, false otherwise 
	 */
	bool isSleeping() const;
	
	/// returns true if the object should not surive a World::reset()
	virtual bool isTemp() const
	{ return false; }

	/// resets the object into the start position/situation
	virtual void reset(void);

	/** removes itself from the physics model, World and DrawWorld
	 */
	bool deregister(void);

	/** adds itself (again) to physics model and world
	 *  if missing, it will create the corresponding DrawObject
	 *  and (re)registers it with the DrawWorld
	 */
	bool reregister(void);

	/** the two STATIC members in this class should only be accessible to the
	 *  World class
	 */
	class ForWorldOnly
	{
	private:
		friend class World;
	
		/// static member to set the world ID where objects should be in
		static void setTheB2WorldPtr(b2World* aPtr);
	};

	// TODO: FIXME: rogue pointer here!!!
	World* theWorldPtr;

	virtual DrawObject* createDrawObject();

	virtual void createPhysicsObject(void);
	virtual void deletePhysicsObject(void);
	virtual bool isPhysicsObjectCreated(void) const
		{ return theB2BodyPtr!=NULL; }
	
	const BaseObjectSerializer* getSerializer(void) const;

private:
	// Private attributes
	//
	
	/** the position of the center of the object. Note that this attribute
	 *  is not updated during simulations, so we don't need to reset it
	 */
	Position theCenter;
	
	/// width of the object. If object is resizable, this can be changed by user 
	qreal theWidth;
	
	/// height of object. If object is resizable, this can be changed by user
	qreal theHeight;
	
	/** indicates how object should bounce - two objects of 0.0 will stick 
	*  together, whereas two objects of 1.0 will bounce without loosing
	*  energy. Bounciness over 1.0 will resulting in ever larger bounces
	*  (not really realistic, right?)
	*/
	qreal theBounciness;

	/** true if the user can move this object
	 *  note that this has nothing to do with MovingObject or ImmovableObject
	 *  it has to do with the level design - as such, Level can modify this setting
	 */
	bool theIsMovable;

	
protected:
	typedef QList<b2ShapeDef*> ShapeList;
	ShapeList theShapeList;

	void clearShapeList();
	
	b2Body* theB2BodyPtr;
	b2BodyDef* theB2BodyDefPtr;
	
	b2World* getB2WorldPtr(void) const;
	
	/// pointer to a DrawObject that will draw this object
	DrawObject* theDrawObjectPtr;

	typedef QMap<QString,QString> PropertyMap;
	PropertyMap theProperties;
	
	friend class BaseObjectSerializer;

public:
	// public attribute accessor methods

	/**
	 * Sets whether the object can be moved by the user
	 * @param aNewState true if movable, false if not
	 */
	void setIsMovable(bool aNewState)
		{ theIsMovable = aNewState; }
	
	/**
	 * Set the value of theCenter - this is the "original" center,
	 * i.e. where the object will return to after a "reset".
	 * @param new_var the new value of theCenter
	 */
	void setOrigCenter ( Position new_var )
	{
		theCenter.x = new_var.x;
		theCenter.y = new_var.y;
		if (isRotatable())
			theCenter.angle = new_var.angle;
	}

	/**
	 * Set the value of theCenter - this is the "temporary" center,
	 * i.e. used during mouse movements only
	 * @param new_var the new value of the center of the body
	 */
	void setTempCenter ( Position new_var );

	/**
	 * Get the Position of the object.
	 * This is the "original" center, i.e. where the object will
	 * return to after a "reset". 
	 * @return the value of theCenter
	 */
	Position getOrigCenter ( ) const
		{ return theCenter; }

	/**
	 * Get the current Position of the object.
	 * This is the current center, i.e. where the object is now.
	 * During/After simulations or during mouse drag of the object, this 
	 * may be different from the getOrigCenter above.
	 *  
	 * @return the value of theCenter
	 */
	Position getTempCenter ( ) const;

	/**
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( qreal new_var )
		{ if (new_var>0.01) theWidth = new_var; }

	/**
	 * Get the value of theWidth
	 * @return the value of theWidth
	 */
	qreal getTheWidth ( ) const
		{ return theWidth; }

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( qreal new_var )
		{ if (new_var>0.01) theHeight = new_var;	}

	/**
	 * Get the value of theHeight
	 * @return the value of theHeight
	 */
	qreal getTheHeight ( ) const
		{ return theHeight; }

	/**
	 * Set the value of theBounciness 
	 *   (0.0 = stick, 1.0 = full elastic bounce)
	 * @param new_var the new value of theBounciness
	 */
	void setTheBounciness ( qreal new_var )
		{ theBounciness = new_var; }

	/**
	 * Get the value of theBounciness
	 *   (0.0 = stick, 1.0 = full elastic bounce)
	 * @return the value of theBounciness
	 */
	qreal getTheBounciness ( )				 
		{ return theBounciness; }

	/// set property aKey to aValue
	virtual void  setProperty(const QString& aKey, const QString& aValue)
		{ theProperties[aKey] = aValue; }

	/// @returns the value for property aKey
	/// - or an empty string if it does not exist
	QString getProperty(const QString& aKey)
	{ return theProperties.value(aKey, ""); }

protected:
	void setAngle(qreal anAngle)
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
public:
	typedef QList<const ObjectFactory*> ObjectFactoryList;

protected:
	/** create the object aName and return a pointer to it
	 * @param aName
	 * @param aPostion
	 * @param aWidth    width of the object in meter - default 1.0
	 * @param anHeight  height of the object in meter - default 1.0
	 * @return a valid pointer to a newly created object or NULL if not found
	 */
	static BaseObject* createObject(
			const QString& aName, 
			const Position aPostion, 
			const qreal aWidth = 1.0,
			const qreal anHeight = 1.0);
	
	static void announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr);
	
	/** create an ObjectFactoryList with pointers to all known ObjectFactories.
	 *  it is up to the caller (usually ToolBoxItemListModel) to delete the list
	 *  - do not delete the contents of the list.
	 */
	static ObjectFactoryList* getAllFactories(void);
	friend class ToolBoxItemListModel;

	/** pure virtual function that creates an object of the type the factory is for
	 *  must be implemented for each real factory.
	 * @return pointer to a newly instantiated object
	 */
	virtual BaseObject* createObject(void) const = 0;
};

#endif // BASEOBJECT_H
