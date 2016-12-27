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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef BASEJOINT_H
#define BASEJOINT_H

#include "AbstractObject.h"
#include "Box2D.h"

// forward declarations
class b2BodyPtr;
class b2RevoluteJoint;

/** AbstractJoint is the base class for all Joints
  * it concerns itself with figuring out the ground object and
  * receiving signals to delete physics objects.
  *
  * it also contains some common property parsing for all joints
  */
class AbstractJoint : public AbstractObject, public JointInterface
{
public:
    // empty constructor
    AbstractJoint(void);

    // virtual, empty destructor
    virtual ~AbstractJoint();

    /// Overridden to not try to display child joints.
    ViewItem* createViewItem(float aDefaultDepth=2.0) override;

    /// overridden from AbstractObject
    /// Generic implementation for all Joints - delete the joint.
    /// (most Joints will not have bodies)
    virtual void deletePhysicsObject(void) override;

    /// all joints have no mass, I'd hesitate to call them static, though...
    virtual b2BodyType getObjectType(void) const override
    {
        return b2_dynamicBody;
    }

    /// @returns true if a child joint, created as property of another object
    virtual bool isChildJoint(void)
    {
        return isChild;
    }

    /// overridden from AbstractObject
    /// @returns true if the Joint is created
    virtual bool isPhysicsObjectCreated(void) const override
    {
        return theJointPtr != nullptr;
    }

    /// called by World when the joint was "implicitly destructed"
    virtual void jointWasDeleted(void) override;

    /// when this member is called, this PivotPoint will be
    /// marked as child, i.e. it will not save itself
    void markAsChild(void);

    /// implemented from JointInterface
    virtual void physicsObjectStatus(JointStatus aStatus) override;

    /// called by the World class to set the Ground Body
    static void setGroundBodyPtr(b2Body *aPtr);

    virtual void updateOrigCenter(void) = 0;

    /// @returns the b2Body* for object anObject. if anObject doesn't have
    /// a b2Body (yet), we'll call createPhysicsObject() on the object to
    /// have it created.
    /// @param aRelPosition RELATIVE position on the object to find an b2Body
    ///                     for. This is relevant for composite objects.
    b2Body *getB2BodyPtrFor(AbstractObjectPtr anObject, const Position &aRelPosition);

    /// updates the ViewObject to the position of the underlying b2body
    /// (it won't update if the object is asleep if sim is running)
    /// @param isSimRunning  set to true if you want to use position/size from sim
    virtual void updateViewObject(bool isSimRunning) const override;

protected:
    /// if you have only one object, it is supposed to be static
    /// related to the world i.e. the ground body. This is where to get it.
    /// (see also setGroundBodyPtr() ).
    b2Body *getGroundBodyPtr(void);

    b2Joint *theJointPtr;

    /// Set to true if this joint is a child object of something else. Likely
    /// it will be a PivotPoint or TranslationGuide that is used as a property.
    bool isChild;
};

#endif // BASEJOINT_H
