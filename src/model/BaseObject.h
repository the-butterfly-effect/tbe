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
#include "Property.h"

#include "b2Body.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QSet>

// Forward Declarations
class DrawObject;
class ObjectFactory;
class World;
class ShapeList;
class b2FixtureDef;
class b2ShapeDef;
class b2Shape;
class b2World;
class BaseObjectSerializer;
class ContactInfo;

//   ************************************************
//   *                                              *
//   * NOTE: the ObjectFactory class declaration is *
//   *       below the BaseObject declaration       *
//   *                                              *
//   ************************************************

/** This interface is implemented by all joints that attach to objects
  * it is used to attach and for notification when the physics objects
  * have been removed
  */
class JointInterface
{
public:
	/// empty virtual destructor
	virtual ~JointInterface() {;}

	enum JointStatus
	{
		CREATED = 1,
		POSUPDATE,
		DELETED
	};

	/** called by the BaseObject(-derivate) (and/or UndoXXXCommand)
	  * to annouce that its physics object(s) have been re-created.
	  */
	virtual	void physicsObjectStatus(JointStatus aStatus) = 0;

	/// called by World when the joint was "implicitly destructed"
	virtual void jointWasDeleted(void) = 0;
};


class SensorInterface
{
public:
	/// empty virtual destructor
	virtual ~SensorInterface() {;}

	/// called if Object has registered a sensor share
	/// the default is to do completely nothing - you'll have to override
	virtual void callBackSensor(const ContactInfo&)
	{ return; }
};


/** Interface to indicate a BaseObject child class is interested in
 *  information on impulses acting on its body.
 *
 *  Don't forget to override the isInterested*() if you want the report*() !!!
 *
 * I don't expect any class will inherit this class directly - BaseObject
 * already does that. Just override what you need.
 */
class ImpulseInterface
{
public:
	/// empty virtual destructor
	virtual ~ImpulseInterface() {;}

	/// please override and return true if you want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return false; }

	/// please override and return true if you want reports on NormalImpulse
	virtual bool isInterestedInTangentImpulse(void)
	{ return false; }

	/** will be called by World to report normal impulse length
	  * note that an object resting on top of another already has a (constant)
	  * normal impulse vector.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(UNUSED_ARG qreal anImpulseLength) {};

	/** will be called by World to report tangential impulse length
	  * note that an object resting on top of another has no tangential
	  * impulse vector.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportTangentImpulse(UNUSED_ARG qreal anImpulse) {};
};



// ABOUT BODIES, SHAPES, JOINTS AND THE BASEOBJECT CLASS
//
//   In Box2D, we have Worlds, Bodies, Fixtures, Shapes and Joints:
//
//	 * World    - the "space" all bodies live in
//	 * Body     - the concept of any object in the world.
//                there are three types of bodies:
//                  * dynamic bodies
//					* static bodies
//                  * kinetic bodies (not used in TBE)
//				  This is described through the getObjectType() member
//                NOTE:  Box2D will automatically set mass to 1.0 if a
//                dynamic body has zero mass
//                NOTE2: only for static bodies, our code is allowed to do
//                movement by hand - otherwise movement is always handled by
//                the Box2D solver!!!
//   * Fixture  - describes how the shapes together form a body. concepts like
//                relative position, bounciness, friction and mass are
//                described here
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

class BaseObject : public SensorInterface, public ImpulseInterface
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
	
	/** returns the Name of the object, for users.
	 *  If a non-US locale is specified, the returned string
	 *  will already be translated
	 */
	virtual const QString getName ( ) const = 0;

	/// @returns the internal (CamelCased) object name
	QString getInternalName( ) const
	{ return theInternalName; }

	const QString& getID (void) const
	{	return theID; }

	void setID (const QString& anID)
	{	theID = anID; }

	/// child objects must specify what type of body they are
	/// @returns a value from the b2BodyType enum, valid options
	///          within TBE are b2_dynamicBody and b2_staticBody
	///          if unsure (e.g. joints) return b2_staticBody
	virtual b2BodyType getObjectType(void) const = 0;

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const = 0;

	/// returns true if the object can be moved by the user
	virtual bool isMovable ( ) const;
	
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
	
	/// returns true if the object should not survive a World::reset()
	virtual bool isTemp() const
	{ return false; }

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

	/** Adds the Joint to the list.
	 *  If this object runs createPhysObject, it will notify all Joints
	 */
	void addJoint(JointInterface* aJoint)
	{	theJointList.insert(aJoint); }

	/** Creates the drawObject, sets ZValue to 2 (or to whatever the
	  * the ZValue property is set to) and returns a pointer to it.
	  * @returns pointer to DrawObject
	  */
	virtual DrawObject* createDrawObject();

	/// create a physics object (i.e. B2Body) and add all shapes to it
	/// if a B2Body already exists, it is deleted first.
	/// the object will be positioned at theCenter.
	virtual void createPhysicsObject(void);

	/// create a physics object (i.e. B2Body) and add all shapes to it
	/// if a B2Body already exists, it is deleted first.
	/// @param aPosition the position of the center of the object.
	virtual void createPhysicsObject(Position aPosition);

	/// null the current Physicsobject (note that they are no longer
	/// deleted in normal game operation - if you need that behaviour
	/// (like e.g. CokeSplatter) you need to override that in its destructor.
	virtual void deletePhysicsObject(void);

	virtual bool isPhysicsObjectCreated(void) const
		{ return theB2BodyPtr!=NULL; }
	
	const BaseObjectSerializer* getSerializer(void) const;

	/** announce to all registered joints that the physicsobject has been
	  * created/deleted.
	  * @param aStatus indicates if the B2Body was created or deleted
	  */
	virtual void notifyJoints(JointInterface::JointStatus aStatus);

	/** parse all properties
	  * NOTE: BaseObject only understands PivotPoint and Bounciness
	  */
	virtual void  parseProperties(void);

	PropertyList theProps;

	/// returns the image index, will be used by DrawObject for animation
	/// @returns: the default implementation always returns 0 - no animation.
	virtual int getImageIndex(void)
	{ return 0; }

private:
	// Private attributes
	//
	
	/// the position of the center of the object when not in simulation.
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

protected:
	/** true if the user can move this object
	 *  note that this has nothing to do with MovingObject or ImmovableObject
	 *  it has to do with the level design - as such, Level can modify this setting
	 */
	bool theIsMovable;

	typedef QSet<JointInterface*>  JointList;
	JointList theJointList;

	typedef QList<b2FixtureDef*> ShapeList;
	ShapeList theShapeList;

	friend class DrawPolyObject;

	void clearShapeList();
	
	b2Body* theB2BodyPtr;
	b2BodyDef* theB2BodyDefPtr;
	
	b2World* getB2WorldPtr(void) const;
	
	/// pointer to a DrawObject that will draw this object
	DrawObject* theDrawObjectPtr;

	/** string with this object's ID (a random, untranslated string
	 *  used to identify individual objects in a level)
	 *  will be set during init by the serializer (or not)
	 */
	QString theID;

	/// the internal name (usually set by the ObjectFactory)
	QString theInternalName;

	friend class BaseObjectSerializer;

	// All Joints need access to the B2BodyPtr.
	friend class BaseJoint;

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
	virtual void setOrigCenter ( Position new_var );

	/**
	 * Get the Position of the object.
	 * This is the "original" center, i.e. where the object will
	 * be when not in simulation.
	 * @return the value of theCenter
	 */
	virtual Position getOrigCenter ( ) const
		{ return theCenter; }

	/**
	 * Get the current Position of the object.
	 * If there is an actual Physics model, it will return the position from
	 * the physics model. If there is not, it will return getOrigCenter().
	 *
	 * @return the value of the current Center
	 */
	virtual Position getTempCenter ( ) const;


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

	/// make sure to override for objects that continously change size during
	/// the simulation
	virtual qreal getTempWidth() const
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

	/// make sure to override for objects that continously change size during
	/// the simulation
	virtual qreal getTempHeight() const
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

protected:
	void setAngle(qreal anAngle)
		{ theCenter.angle = anAngle; }
	
	/** sets the ZValue for theDrawObject to
	 *  the value set in Property::ZVALUE_STRING or
	 *  or (only if the above property doesn't exist) to @param aDefaultValue
	 */
	void setDrawObjectZValue(float aDefaultValue);

private:

	virtual void initAttributes ( ) ;

	// disable copy constructor / assignment operator
	BaseObject(const BaseObject& aBORefToCopy);
	BaseObject& operator = (const BaseObject& aBORefToCopy);

	friend class ObjectFactory;


	// TODO/FIXME: this is getting ugly...
	friend class UndoObjectChange;		// the undo for all related to objects
	friend class UndoDeleteCommand;
	friend class UndoInsertCommand;
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
	/// empty virtual destructor
	virtual ~ObjectFactory() {;}

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
	
	/// setup e.g. theInternalName of the object
	BaseObject* fixObject(BaseObject*) const;

	QString getFactoryName(void) const
	{ return theFactoryName; }

	/** create an ObjectFactoryList with pointers to all known ObjectFactories.
	 *  it is up to the caller (usually ToolBoxItemListModel) to delete the list
	 *  - do not delete the contents of the list.
	 */
	static ObjectFactoryList* getAllFactories(void);
	friend class ToolBox;

	/** pure virtual function that creates an object of the type the factory is for
	 *  must be implemented for each real factory.
	 * @return pointer to a newly instantiated object
	 */
	virtual BaseObject* createObject(void) const = 0;
	friend class TBItem;
private:
	QString theFactoryName;
};

#endif // BASEOBJECT_H
