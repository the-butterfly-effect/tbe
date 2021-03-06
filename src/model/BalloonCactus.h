/* The Butterfly Effect
 * This file copyright (C) 2010,2012  Klaas van Gend
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

#ifndef BALLOONCACTUS_H
#define BALLOONCACTUS_H

#include "CircleObjects.h"
#include "PolyObject.h"
#include "World.h"


//
//  This header file lists 3 classes:
//    * Balloon
//    * Cactus
//    * BedOfNails
//

/** this class implements a Balloon
  *
  */
class Balloon : public PolyObject, public SimStepCallbackInterface
{
public:
    Balloon();

    virtual ~Balloon();

    /// overridden because a Balloon can 'die' through external reasons
    void causeWounded(WhyWounded aReason) override;

    /// overridden from PolyObject because this class wants to register for
    /// callbacks and needs to restart its state machine
    virtual void createPhysicsObject(void) override;

    /// let's mis-use deletePhysicsObject to reset our object state
    virtual void deletePhysicsObject(void) override;

    /// deletePhysicsObject() doesn't really delete the physics object
    /// anymore - but we need a true deleter here...
    void deletePhysicsObjectForReal(void);

    /// this enum defines the states of the balloon
    enum States {
        BALLOON,
        POPPING,
        POPPED,
        GONE
    };

    /// overridden from AbstractObject to allow representation of the states
    /// @returns: returns a numerical index similar to the state
    virtual unsigned int getImageIndex(void) const override
    {
        return theState;
    }

    /// overridden to make sure joints are not re-created outside BALLOON state
    virtual void notifyJoints(JointInterface::JointStatus aStatus) override;

protected:
    /// call this function to suggest a state change to the Balloon
    /// note that the Balloon can decide not to follow your state change,
    /// going from popped to balloon isn't supported (just like real life)
    /// @param aNewState the suggestion for a new state
    /// @returns the state after this function completes
    States goToState(States aNewState);

    /// will replace the existing set of shapes by a smaller shape that
    /// fits the BalloonRest image. Do not call from within a Box2D callback
    void switchToSmallShape(void);

public:
    // the following two members are part of the normal impulse reporting

    /// overridden from AbstractObject - we want reports on NormalImpulse
    virtual bool isInterestedInNormalImpulse(void) override
    {
        return true;
    }

    /** overridden from AbstractObject - we want to receive
      * reports on the normal impulse.
      * @param anImpulseLength length of the normal impulse vector
      */
    virtual void reportNormalImpulseLength(qreal anImpulseLength,
                                           AbstractObject *anOtherObjectPtr) override;

private:
    /// implemented from SimStepCallbackInterface
    virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

    void callbackStepBalloon (qreal aTimeStep, qreal aTotalTime);
    void callbackStepPopped  (qreal aTimeStep, qreal aTotalTime);
    void callbackStepPopping (qreal aTimeStep, qreal aTotalTime);

private:
    // Private things

    /// the state variable
    States theState;

    qreal thePoppingTimeStart;

    const static qreal POPPING_TIME;
    const static qreal POPPED_MASS;
    const static qreal POPPED_TIME;
};


///---------------------------------------------------------------------------
///------------------------- Cactus ------------------------------------------
///---------------------------------------------------------------------------

/** This class implements a Cactus.
  * For now, the most important feature of a cactus is that it can pop a Balloon.
  * (In the future, we might want to add the ability to break the pot or the cactus)
  */
class Cactus : public PolyObject
{
public:
    Cactus();

    virtual ~Cactus();

    /// Overridden so we will get notified if something poppable hits our
    /// sharp points
    void callBackSensor(const ContactInfo &aPoint) override;

    /** Overridden from PolyObject because we also have a sensor
     *  here...
     */
    void fillShapeList(void) override;
};


///---------------------------------------------------------------------------
///------------------------- BedOfNails --------------------------------------
///---------------------------------------------------------------------------

/** This class implements a bed of nails.
  * For now, the most important feature is that it can pop a Balloon.
  * (i.e. it is a streched, one-sided cactus)
  */
class BedOfNails : public PolyObject
{
public:
    BedOfNails();

    virtual ~BedOfNails();

    /// Overridden so we will get notified if something poppable hits our
    /// sharp points
    void callBackSensor(const ContactInfo &aPoint) override;

    /** Just like Cactus, we need to redefine the shapes
     */
    void fillShapeList(void) override;
};


///---------------------------------------------------------------------------
///------------------------ CircularSaw --------------------------------------
///---------------------------------------------------------------------------

/** This class implements a CircularSaw (blade).
  */
class CircularSaw : public CircleObject
{
public:
    CircularSaw();

    virtual ~CircularSaw();

    /// Overridden so we will get notified if something poppable hits our
    /// sharp points
    void callBackSensor(const ContactInfo &aPoint) override;

    /** Overridden from AbstractBall because we also have to define
     *  a sensor here...
     */
    void createBallShapeFixture(float aRadius, float aMass) override;

    /// we need to adjust the size based on the radius
    void  parseProperties(void) override;
};


#endif // Balloon_H
