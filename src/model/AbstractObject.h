/* The Butterfly Effect
 * This file copyright (C) 2009,2011,2014 Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QtCore/QtGlobal>
#include "LocalString.h"
#include "PivotPointPtr.h"
#include "Position.h"
#include "Property.h"
#include "TranslationGuidePtr.h"

#include "Box2D.h"

#include <set>

// Forward Declarations
class ViewObject;
class ObjectFactory;
class World;
class ShapeList;
struct b2FixtureDef;
class b2ShapeDef;
class b2Shape;
class b2World;
class AbstractObjectSerializer;
struct ContactInfo;


//   ************************************************
//   *                                              *
//   * NOTE: the ObjectFactory class declaration is *
//   *       below the AbstractObject declaration   *
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

    /** called by the AbstractObject(-derivate) (and/or UndoXXXCommand)
      * to annouce that its physics object(s) have been re-created.
      */
    // TODO/FIXME: rename member if possible to start with a verb
    virtual	void physicsObjectStatus(JointStatus aStatus) = 0;

    /// called by World when the joint was "implicitly destructed"
    virtual void jointWasDeleted(void) = 0;
};

typedef std::shared_ptr<JointInterface> JointInterfacePtr;


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


/** Interface to indicate a AbstractObject child class is interested in
 *  information on impulses acting on its body.
 *
 *  Don't forget to override the isInterested*() if you want the report*() !!!
 *
 * I don't expect any class will inherit this class directly - AbstractObject
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

#include "AbstractObjectPtr.h"

/**
  * class AbstractObject
  *
  * abstract base class for everything in the simulation model
  */

class AbstractObject : public SensorInterface, public ImpulseInterface
{
public:
	AbstractObject();

	virtual ~AbstractObject();

	enum SizeDirections
	{
		NORESIZING = 0,
		HORIZONTALRESIZE = 1,
		VERTICALRESIZE = 2,
		TOTALRESIZE = 3
	};

	/// (static const) minimum dimension of any (part of) object.
	static const float MINIMUM_DIMENSION;

	//
	// Public Getters and Setters
	// (sorted alphabetically)

	/// @returns Pointer to the B2Body for the relative position asked for.
	///          Might return NULL if no body or if outside body (see warning)
	/// @param   Relative position (to the center of the object) to look for
	/// @warning Because the default BaseObject just returns its pointer
	///          without any checking, don't expect this member to fail if
	///          aRelPosition points outside the object's body.
	/// @note    Overriding this member probably makes sense for composite
	///          objects only
	virtual b2Body* getB2BodyPtrForPosition(UNUSED_ARG const Position& aRelPosition)
	{ return theB2BodyPtr; }

	/// @returns the ID (which is used to identify objects in Goals and links)
	const QString& getID (void) const
	{	return theID; }

	/// Override this member to allow for animations.
	/// @returns: returns a numerical index of what image to use.
	virtual unsigned int getImageIndex(void) const
	{ return 0; }

	/// @returns the internal (CamelCased) object name - untranslated!
	QString getInternalName( ) const
	{ return theInternalName; }

    /** returns the Name of the object, for users.
     *  If a non-US locale is specified, the returned string
     *  will already be translated
     */
    virtual const QString getName ( ) const = 0;

	/// child objects must specify what type of body they are
	/// @returns a value from the b2BodyType enum, valid options
	///          within TBE are b2_dynamicBody and b2_staticBody
	///          if unsure (e.g. joints) return b2_staticBody
	virtual b2BodyType getObjectType(void) const = 0;

	// TODO/FIXME: rename to getCenterPos()
	/// Get the Position of the object.
	/// This is the "original" center, i.e. where the object will
	/// be when not in simulation.
	/// @returns the value of theCenter
	virtual Position getOrigCenter ( ) const
		{ return theCenter; }

    /// @returns a pointer to a serializer for this object.
    /// This member is used during saving...
    const AbstractObjectSerializer* getSerializer(void) const;

    /**
	 * Get the current Position of the object.
	 * If there is an actual Physics model, it will return the position from
	 * the physics model. If there is not, it will return getOrigCenter().
	 *
	 * @return the value of the current Center
	 */
	virtual Position getTempCenter ( ) const;

	/// make sure to override for objects that continously change size during
	/// the simulation
	virtual qreal getTempHeight() const
	{ return theHeight; }

	/// make sure to override for objects that continously change size
	/// during the simulation (e.g. Spring)
	virtual qreal getTempWidth() const
	{ return theWidth; }

	/// Get the value of theBounciness
	/// (0.0 = stick, 1.0 = full elastic bounce)
	/// @return the value of theBounciness
	qreal getTheBounciness ( ) const
		{ return theBounciness; }

	/// Get the value of theHeight
	/// @return the value of theHeight
	qreal getTheHeight ( ) const
	{ return theHeight; }

	/// Get the value of theWidth
	/// @return the value of theWidth
	qreal getTheWidth ( ) const
	{ return theWidth; }

    AbstractObjectPtr getThisPtr() const
    { return AbstractObjectPtr(theThisPtr); }

	/// @returns the Tooltip of the object.
    virtual const QString getToolTip ( ) const;

	/// @returns true if the object can be moved by the user
	virtual bool isMovable ( ) const;

	/// @returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const // TODO/FIXME = 0;
	{	return resizableInfo;	}

	/// @returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const;

    /// @returns true if the object should not survive a World::deletePhysicsWorld()
    virtual bool isTemp() const
    { return false; }

	/// sets the ID (which is used to identify objects in Goals and links)
	void setID (const QString& anID)
	{	theID = anID; }

	// TODO/FIXME: rename to setCenterPos()
	/// Set the value of theCenter - this is the "original" center,
	/// i.e. where the object will return to after a "reset".
	/// @param new_var the new value of theCenter
	virtual void setOrigCenter ( const Position& aNewPos )
	{ theCenter = aNewPos; }

	/// static member to set the world ID where objects should be in
	static void setTheB2WorldPtr(b2World* aPtr);

	/// Set the value of theBounciness
	/// (0.0 = stick, 1.0 = full elastic bounce)
	/// @param new_var the new value of theBounciness
	void setTheBounciness ( qreal new_var )
	{ theBounciness = new_var; }

	/// Set the value of theHeight
	/// @param new_var the new value of theHeight
	virtual void setTheHeight ( qreal new_var, bool mustRunParseProperties = true );

	/// Set the value of theWidth
	/// @param new_var the new value of theWidth
	virtual void setTheWidth ( qreal new_var, bool mustRunParseProperties = true );


	//
	// other public members
	//

	/** Adds the Joint to the list.
	 *  If this object runs createPhysObject, it will notify all Joints
	 */
    void addJoint(JointInterfacePtr aJointPtr)
    {	theJointList.insert(aJointPtr); }

	/// create a physics object (i.e. B2Body) and add all shapes to it
	/// if a B2Body already exists, it is deleted first.
	/// the object will be positioned at theCenter.
	virtual void createPhysicsObject(void);

	/// create a physics object (i.e. B2Body) and add all shapes to it
	/// if a B2Body already exists, it is deleted first.
	/// @param aPosition the position of the center of the object.
	virtual void createPhysicsObject(const Position& aPosition);

    /** Creates the ViewObject, finds associated images,
      * sets ZValue and returns a pointer to it.
      * @param   aDefaultDepth, ZValue depth in view if not set as property,
      *          the higher the value the more likely it is drawn on top
      * @returns pointer to ViewObject
      * @note: a ZValue set in a property always overrides aDefaultDepth
      */
    virtual ViewObject* createViewObject(float aDefaultDepth = 2.0);
    void deleteViewObject();

	/// updates the ViewObject to the position of the underlying b2body
	/// (it won't update if the object is asleep if sim is running)
	/// @param isSimRunning  set to true if you want to use position/size from sim
	virtual void updateViewObject(bool isSimRunning) const;

	/// null the current Physicsobject (note that they are no longer
	/// deleted in normal game operation - if you need that behaviour
	/// (like e.g. ColaSplatter) you need to override that in its destructor.
	virtual void deletePhysicsObject(void);

	virtual bool isPhysicsObjectCreated(void) const
		{ return theB2BodyPtr!=NULL; }

	/** Parse all properties (again).
	  * @note AbstractObject understands PivotPoint, TranslationGuides,
	  *       NoCollision and Bounciness.
	  * @note Any implementation of parseProperties should expect multiple calls.
	  */
	virtual void  parseProperties(void);

	/** Called from World upon registration, allows to register
	  * child objects.
	  */
	virtual void registerChildObjects (void) {};

	/** sets the ZValue for theViewObject to
	 *  the value set in Property::ZVALUE_STRING or
	 *  or (only if the above property doesn't exist) to @param aDefaultValue
	 */
	void setViewObjectZValue(float aDefaultValue);

protected:

	b2Body* theB2BodyPtr;
	b2BodyDef* theB2BodyDefPtr;

    /// store a tooltip string
	LocalString theToolTip;

	/// the properties of the object instance
	PropertyList theProps;

	/// pointer to a ViewObject that will draw this object
	ViewObject* theViewObjectPtr;

	typedef QList<b2FixtureDef*> ShapeList;
	/// list holding all the shapes associated with this object
	/// @note this list can change during the simulation, but only
	///       if then createPhysicsObject() is called again,
	///       as this list is then tied to the new b2Body
	ShapeList theShapeList;

    typedef std::set<JointInterfacePtr>  JointList;
	/// list holding all joints
	/// @note during an update of the b2Body, these joints will receive
	///       notifications (see notifyJoints()).
	JointList theJointList;

    PivotPointPtr theChildPivotPointPtr;
    TranslationGuidePtr theChildTranslationGuidePtr;

protected:
	/// remove all shapes from the list
	void clearShapeList();

	b2World* getB2WorldPtr(void) const;

	/** announce to all registered joints that the physicsobject has been
	  * created/deleted.
	  * @param aStatus indicates if the B2Body was created or deleted
	  */
        virtual void notifyJoints(JointInterface::JointStatus aStatus);

private:
	// Private attributes of the Object
	//

	/** indicates how object should bounce - two objects of 0.0 will stick
	*  together, whereas two objects of 1.0 will bounce without loosing
	*  energy. Bounciness over 1.0 will resulting in ever larger bounces
	*  (not really realistic, right?)
	*/
	qreal theBounciness;

	/// the position of the center of the object when not in simulation.
	Position theCenter;

	/// static height of object (i.e. at rest before simulation starts)
	qreal theHeight;

	/** String with this object's ID (a random, untranslated string
	 *  used to identify individual objects in a level).
	 *  This ID is e.g. used to link objects and set goals.
	 */
	QString theID;

	/// the internal name (usually set by the ObjectFactory)
	QString theInternalName;

	/** true if the user can move this object
	 *  Note that this has nothing to do with MovingObject or ImmovableObject
	 *  it has to do with the level design - as such, Level can modify this setting
	 */
	bool theIsMovable;

    // TODO/FIXME: debugging code - should go away at some point
    friend class ViewPostIt;
    friend class EditObjectDialog;

	/// static width of the object (i.e. at rest before simulation starts)
	qreal theWidth;

	// TODO: FIXME: rogue pointer here!!!
public:
	World* theWorldPtr;
private:

	// TODO/FIXME: this one wasn't present in AbstractObject
	// - but it was in RectObject...
	// TODO/FIXME: also rename
	SizeDirections resizableInfo;

	// required to make sure the Serializer and the factory can touch everything
	friend class AbstractObjectSerializer;
	friend class ObjectFactory;
	friend class AbstractJoint;


private:
	// disable copy constructor / assignment operator
	AbstractObject(const AbstractObject& aBORefToCopy);
	AbstractObject& operator = (const AbstractObject& aBORefToCopy);

    AbstractObjectWeakPtr theThisPtr;
};

#endif // ABSTRACTOBJECT_H
