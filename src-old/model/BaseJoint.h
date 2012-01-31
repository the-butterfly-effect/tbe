/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#ifndef BASEJOINT_H
#define BASEJOINT_H

#include "BaseObject.h"

// forward declarations
class b2BodyPtr;
class b2RevoluteJoint;

/** BaseJoint is the base class for all Joints
  * it concerns itself with figuring out the ground object and
  * receiving signals to delete physics objects.
  *
  * it also contains some common property parsing for all joints
  */
class BaseJoint : public BaseObject, public JointInterface
{
public:
	// empty constructor
	BaseJoint(void);

	// virtual, empty destructor
	virtual ~BaseJoint();

	virtual DrawObject* createDrawObject();

	/// overridden from BaseObject
	/// Generic implementation for all Joints - delete the joint.
	/// (most Joints will not have bodies)
	virtual void deletePhysicsObject(void);

	/// all joints have no mass, I'd hesitate to call them static, though...
	virtual b2BodyType getObjectType(void) const
	{	return b2_dynamicBody; }

	/** Get the current center position of the object.
	 *  As joints do not have a center, this one usually returns the start position.
	 *  FIXME/TODO: This is not entirely correct as joints may actually move.
	 *
	 * @return the value of theCenter
	 */
	virtual Position getTempCenter ( ) const
	{ return getOrigCenter(); }

	/// overridden from BaseObject
	/// @returns true if the Joint is created
	virtual bool isPhysicsObjectCreated(void) const
	{ return theJointPtr!=NULL; }

	/// called by World when the joint was "implicitly destructed"
	virtual void jointWasDeleted(void);

	/// when this member is called, this PivotPoint will be
	/// marked as child, i.e. it will not save itself
	void markAsChild(void);

	/// implemented from JointInterface
	virtual	void physicsObjectStatus(JointStatus aStatus);

	/// called by the World class to set the Ground Body
	static void setGroundBodyPtr(b2Body* aPtr);

	virtual void updateOrigCenter(void) = 0;

	/// @returns the b2Body* for object anObject. if anObject doesn't have
	/// a b2Body (yet), we'll call createPhysicsObject() on the object to
	/// have it created.
	/// @param aRelPosition RELATIVE position on the object to find an b2Body
	///                     for. This is relevant for composite objects.
	b2Body* getB2BodyPtrFor(BaseObject* anObject, const Position& aRelPosition);
//	b2Body* getB2BodyPtrFor(BaseObject* anObject);

protected:
	/// if you have only one object, it is supposed to be static
	/// related to the world i.e. the ground body. This is where to get it.
	/// (see also setGroundBodyPtr() ).
	b2Body* getGroundBodyPtr(void);

	b2RevoluteJoint* theJointPtr;
};

#endif // BASEJOINT_H
