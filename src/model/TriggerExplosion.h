/* The Butterfly Effect
 * This file copyright (C) 2010,2013,2014  Klaas van Gend
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

#ifndef TRIGGEREXPLOSION_H
#define TRIGGEREXPLOSION_H

#include "CircleObjects.h"
#include "ObjectFactory.h"
#include "PolyObject.h"
#include "RectObject.h"
#include "World.h"

#include <QString>
#include <QStringList>

class DetonatorBox;

class ExplosionSplatter;

class Dynamite;

/** specific class to handle the nocollision (towards DetonatorBox itself)
  * and no serialization
  * This class only exists as a child object of DetonatorBox.
  */
class DetonatorBoxHandle : public RectObject, public SimStepCallbackInterface
{
private:
    DetonatorBoxHandle() = delete;

public:
    /// @param aDBox pointer to a DetonatorBox, the only object allowed to create a Handle
    /// (note that creation is nowadays offloaded to std::make_shared)
    DetonatorBoxHandle(DetonatorBox *aDBox);
    virtual ~DetonatorBoxHandle() override;

    /// Overridden from RectObject to allow setting a custom ZValue
    ViewObjectPtr createViewObject(float aDefaultDepth) override;

    /// overridden from AbstractObject to allow for the special joints
    virtual void createPhysicsObject(void) override;

    /// overridden from AbstractObject to allow for the special joints
    virtual void deletePhysicsObject(void) override;

    qreal getDistance(void);

    friend class DetonatorBox;

private:
    /// implemented from SimStepCallbackInterface
    virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

    DetonatorBox *theDBoxPtr;
    b2PrismaticJoint *theJointPtr;

private:
    // disable copy constructor / assignment operator
    DetonatorBoxHandle(const DetonatorBoxHandle &aBORefToCopy) = delete;
    DetonatorBoxHandle &operator = (const DetonatorBoxHandle &aBORefToCopy) = delete;
};



/** this class implements the DetonatorBox for an explosion
 *  it has a handle object at the top that can be pressed,
 *  once pressed deep enough, it will send a trigger.
 *
 *  Note that we cheat on reality big time here. The original boxes were
 *  noting more than a dynamo - producing enough current for a spark.
 *  In our demo, we just hooked those wires up to a mobile phone...
 *  Suuuuure that will work ;-)
 */
class DetonatorBox : public RectObject, public SimStepCallbackInterface
{
public:
    DetonatorBox();
    virtual ~DetonatorBox() override;

    /// (overridden from AbstractObject to remove extra sharedptrs)
    virtual void clearObjectReferences() override;

    /// this enum defines the states of the detonator
    enum States {
        ARMED,     // handle up, cell display dark
        ACTIVATED, // handle down, cell display lighting up
        RINGING,   // radio waves visible, display lighted. at the beginning
        // of this state we actually signal all bombs
        // We might want to turn this into multiple states
        // (for animation) later
        DONE       // end state, handle down and cell dark...
    };

    ViewObjectPtr createViewObject(float aDefaultDepth = 2.0) override;

    /// overridden from RectObject to be able to create the handle and
    /// because this class wants to register for
    /// callbacks and needs to restart its state machine
    virtual void createPhysicsObject(void) override;

    /// overridden from AbstractObject to allow for the handle
    virtual void deletePhysicsObject(void) override;

    /// @returns List of all Dynamite phone numbers in scene, no duplicates.
    /// @note Always adds an empty string at the end, to allow deselection.
    QStringList getAllPhoneNumbers(void);

    /// @returns the currently set phone number
    /// this can be an empty string!
    QString getCurrentPhoneNumber(void) const;

    /// @returns "(empty)"
    QString getEmptyString() const;

    /// overridden from AbstractObject to allow representation of the states
    /// @returns: returns a numerical index similar to the state
    virtual unsigned int getImageIndex(void) const override
    {
        return theState;
    }

    /// overridden from RectObject to make sure
    /// we can display the phone number
    virtual const QString getToolTip ( ) const override;

    /// (used by the Handle) returns the ZValue of the DBox,
    /// so the handle can display itself below
    qreal getZValue(void);

    /// overridden from AbstractObject in order to create the handle
    virtual void registerChildObjects (void) override;

    /// overridden from AbstractObject in order to also move the handle
    virtual void setOrigCenter ( const Position &aNewPos ) override;

    /// Set the phone number to dial when triggered
    /// Technically speaking, any ID would do here...
    /// @param aPhoneNumber string with an existing ID
    void setPhoneNumber(const QString &aPhoneNumber)
    {
        thePhoneNumber = aPhoneNumber;
    }

    /// called by theDetonatorBoxHandle when triggered
    void setTriggered(void);

    /// overridden to also trigger updates to the handle if sim is not running
    virtual void updateViewObject(bool isSimRunning) const override;

protected:
    /// call this function to suggest a state change to the DetonatorBox
    /// @param aNewState the suggestion for a new state
    /// @returns the state after this function completes
    States goToState(States aNewState);

    /// call this member to signal listening devices of the trigger
    void notifyExplosions();

private:
    /// implemented from SimStepCallbackInterface
    virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

    /// the state variable
    States theState;

    /// is the box triggered?
    bool isTriggered;

    /// time that the handle came down, i.e. device got triggered
    qreal theActivationStartTime;

    /// time the object should stay in ACTIVATED state
    const static qreal ACTIVATED_TIME;

    /// time the object should stay in RINGING state
    const static qreal RINGING_TIME;

    /// offset of center of the handle to the center of the box
    const static Vector HANDLEOFFSET;

    /// pointer to the handle (separate object)
    AbstractObjectPtr theHandle;

    /// the phone number to "dial" when triggered
    QString thePhoneNumber;

private:
    // disable copy constructor / assignment operator
    DetonatorBox(const DetonatorBox &aBORefToCopy);
    DetonatorBox &operator = (const DetonatorBox &aBORefToCopy);

    friend class DetonatorBoxHandle;
};


/** this class implements the Dynamite for an explosion
 *  it will be triggered by the DetonatorBox class
 */
class Dynamite : public PolyObject, public SimStepCallbackInterface
{
public:
    Dynamite();
    virtual ~Dynamite();

    /// (overridden from AbstractObject to remove extra sharedptrs)
    virtual void clearObjectReferences() override;

    /// overridden from PolyObject because this class wants to register for
    /// callbacks and needs to restart its state machine
    virtual void createPhysicsObject(void) override;

    /// let's mis-use deletePhysicsObject to reset our object state
    virtual void deletePhysicsObject(void) override;

    /// this enum defines the states of the detonator
    /// apart from WAITING->ACTIVE, all states are time-triggered
    enum States {
        WAITING,   // cell phone dark, nothing happening
        ACTIVE,    // cell phone lighted, "incoming call"
        RINGING,   // cell phone lighted, plus radio waves
        BOOM,      // explosion happening
        GONE,      // main object gone (explosion particles may still fly)
    };

    /// overridden from AbstractObject to allow representation of the states
    /// @returns: returns a numerical index similar to the state
    virtual unsigned int getImageIndex(void) const override
    {
        return theState;
    }

    /// overridden from PolyObject to make sure
    /// we can display the phone number
    virtual const QString getToolTip ( ) const override;

    /// requests removal of a splatter from the list
    void removeMe(AbstractObjectPtr aDeadSplatterPtr);

    /// called by DetonatorBox - triggers a state change from WAITING to ACTIVE
    void trigger(void)
    {
        theTrigger = true;
    }

protected:
    /// call this function to suggest a state change to the Dynamite
    /// @param aNewState the suggestion for a new state
    /// @returns the state after this function completes
    virtual States goToState(States aNewState);

private:
    /// implemented from SimStepCallbackInterface
    virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

    /// deletePhysicsObject() doesn't really delete the physics object
    /// anymore - but we need a true deleter here...
    void deletePhysicsObjectForReal(void);

    /// creates all ExplosionSplatters
    void explode(void);

    /// part of callbackStep for the BOOM and GONE states, this manages
    /// the weights of all remaining particles
    /// @param aTimeStep    TODO
    void manageParticles(float aDeltaTime);

    /// the state variable
    States theState;

    /// set to true when activated
    bool theTrigger;

    /// time that the the ACTIVE state started
    qreal theActiveStartTime;

    /// time that the BOOM state started
    qreal theBoomStartTime;

    /// time the object should stay in RINGING state
    const static qreal RINGING_TIME;

    /// mass of the dynamite
    const static qreal DYNAMITE_MASS;

    typedef QList<AbstractObjectPtr> SplatterList;
    SplatterList theSplatterList;
};


/** this class implements the flying debries of the Explosion
 *  it is modelled in small bubbles, 10 grams each
 *  once bubbles are disappearing, we'll increase the other ones in weight
 */
class ExplosionSplatter : public CircleObject
{
public:
    ExplosionSplatter();
    virtual ~ExplosionSplatter();

    /// @returns true if the object should not surive a World::deletePhysicsWorld()
    /// overridden from AbstractObject
    virtual bool isTemp() const override
    {
        return true;
    }

    /// overridden from AbstractObject - we want reports on NormalImpulse
    virtual bool isInterestedInNormalImpulse(void) override
    {
        return true;
    }

    /** overridden from AbstractObject - if we have an impulse, we hit something
      * and we'd probably better disband ourselves soon...
      * @param anImpulseLength length of the normal impulse vector
      */
    void reportNormalImpulseLength(qreal anImpulseLength,
                                   AbstractObject *anOtherObjectPtr) override;

    /** sets all parameters of the splatter, attaches to World
      * and creates the physical object and drawobject
      * @param aWorldPtr
      * @param aStartPos
      * @param aVelocity
      * @param aSplatterMass
      * @param aDynamitePtr    pointer to the parent Dynamite - for feedback
      */
    void setAll(World *aWorldPtr, const Position &aStartPos,
                qreal aVelocity, qreal aSplatterMass, Dynamite *aDynamitePtr);

    /** sets the splattermass to whatever value needed.
      * @param aSplatterMass the new mass of the splatter
      */
    void setMass( qreal aSplatterMass );

    const static qreal theRadius;

    /// (See Box2D manual, "groupIndex" of a shape)
    /// a negative group index means that all shapes of that group will never
    /// ever collide - very useful for our little ExplosionSplatters
    const static int   COLLISION_GROUP_INDEX;

    Vector theStartVelocityVector;

protected:
    Dynamite   *theDynamitePtr;

    /// Counts the number of bounces.
    int         theBounceCounter;
};



#endif // TRIGGEREXPLOSION_H
