/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#ifndef PINGUS_H
#define PINGUS_H

#include "CircleObjects.h"
#include "World.h"


/** This class implements a Pingus, the small penguin from the game 'Pingus'.
  * We're borrowing this nice penguin to make a few nice puzzle levels that
  * have a familiar feeling. See http://pingus.seul.org/ .
  *
  * Contrary to 'popular belief', Pingus is not PolyObject, but a CircleObject.
  * There are special testing levels available in the testing/Pingus directory.
  *
  * This Pingus starts in the mode "Falling".
  * There are also specialized Pingus classes, see below.
  */
class Pingus : public CircleObject, public SimStepCallbackInterface
{
public:
	Pingus();

	virtual ~Pingus();

    /// overridden because a Pingus can 'die' through external reasons
    void causeWounded(WhyWounded aReason) override;

    ViewObjectPtr  createViewObject(float aDefaultDepth) override;

	/// Overridden from CircleObject because this class wants to register for
	/// callbacks and needs to restart its state machine.
	void createPhysicsObject() override;

	/// Let's mis-use deletePhysicsObject to reset our object state.
	void deletePhysicsObject() override;

	/// DeletePhysicsObject() doesn't really delete the physics object
	/// anymore - but we need a true deleter here...
	void deletePhysicsObjectForReal();

	/// This enum defines the states of the Pingus.
	/// It's (not by accident) the same as the row number in the image.
	enum States
	{
		WALKINGLEFT = 0,
		WALKINGRIGHT,
		FALLING,
		SLIDELEFT,
		SLIDERIGHT,
		SPLATTING,
		WAITING,
        EXITINGLEFT,    // exit-ing, not exciting
        EXITINGRIGHT,
        DIDEXIT,        // end state after EXITING* is complete
        SLEEPING,
		DEAD	// keep this one last!
	};

	static const unsigned int FramesPerState[];

	/// Overridden from AbstractObject to allow representation of the states.
	/// @returns: returns a numerical index similar to the state
	virtual unsigned int getImageIndex() const override
	{ return theState; }

    /// Called by PingusExit class when touching a Pingus, denotes that
    /// the Pingus should start (or continue - it could be triggered multiple)
    /// to leave the world happily.
    void startYourExit();

    /// called by GoalEscapedPingusCounter
    /// @returns the number of (still) alive Pingus
    /// @note escaped pingus are still considered 'alive', just not on this world
    static int getAlivePingusCount()
    { return theAliveCount; }

    /// called by GoalEscapedPingusCounter
	/// @returns the number of Pingus that have started exiting the world
	static int getEscapedPingusCount()
	{ return theEscapedCount; }

private:
	/// Call this function to suggest a state change to the Pingus.
	/// @note this member can decide not to follow your state change,
	///       going from dead to Pingus isn't supported (just like real life).
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
    States goToState(States aNewState);

public:
	// the following two members are part of the normal impulse reporting

	/// Overridden from AbstractObject - we want reports on NormalImpulse
	bool isInterestedInNormalImpulse() override
	{ return true; }

	/** Overridden from AbstractObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
    void reportNormalImpulseLength(qreal anImpulseLength,
                                   AbstractObject* anOtherObjectPtr) override;

    /// called by World at the start of every simulation
    static void resetPingusCount();

protected:
	/// implemented from SimStepCallbackInterface
	void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

    virtual void callbackStepFalling (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepSliding (qreal aTimeStep, qreal aTotalTime);
    virtual void callbackStepSplatting (qreal aTimeStep, qreal aTotalTime); // also handles Exiting state
    virtual void callbackStepWaiting (qreal aTimeStep, qreal aTotalTime); // also handles Sleeping state
	virtual void callbackStepWalking (qreal aTimeStep, qreal aTotalTime);

	/// Internal function to set all parameters to initial values (again)
	virtual void resetParameters();

	/// Update the animation frame of the ViewPingus
	void updateViewPingus();

	// "Private" things

	/// The state variable
	States theState;

	/// Within a state, there are several animation frames,
	/// this is the index into the list.
	unsigned int theAnimationFrameIndex;

	/// We need to keep track whether the splatting is done.
	qreal theSplattingTimeStart;
	/// We need to keep track of falling start for the animation.
	qreal theFallingTimeStart;
	/// We need to keep track of waiting start for the animation
	/// *and* for doing checks whether walking is possible again.
	qreal theWaitingTimeStart;

	/// Keep the last normal impulse length reported, for determining
	/// whether we are falling (very low impulse) or not.
	qreal theLastNormalImpulseReported;

	static int theEscapedCount;
    static int theAliveCount;
};


///---------------------------------------------------------------------------
///------------------------- SleepingPingus -----------------------------------
///---------------------------------------------------------------------------


/// The SleepingPingus acts like a normal Pingus, except that he starts out
/// as a sleeping Pingus, he's not trying to get moving, you need to "help"
/// him. If he ever gets stuck (e.g. wedgedagainst the bottom of a ramp)
/// he's probably going to wait and stay awake, though.
class SleepingPingus : public Pingus
{
public:
    SleepingPingus();

    virtual ~SleepingPingus();

protected:
	void resetParameters() override;
};


///---------------------------------------------------------------------------
///------------------------- PingusExit --------------------------------------
///---------------------------------------------------------------------------

/** This class implements the exit for a Pingus, i.e. his home.
  * Upon touching a Pingus, it will notify the Pingus to exit
  */
class PingusExit : public AbstractObject
{
public:
    PingusExit();

    virtual ~PingusExit();

    /// Overridden so we can figure out if we're hit by a Pingus
    void callBackSensor(const ContactInfo& aPoint) override;
    /// returns the Name of the object.

    // PingusExit doesn't have a user visible name
    const QString getName ( ) const override
    {	return "";	}

    /// child objects must specify what type of body they are
    /// @returns b2_staticBody if this object has no mass
    ///          or b2_dynamicBody if its mass is larger than 0.001 kg
    b2BodyType getObjectType(void) const override
    {	return b2_staticBody; }
};

#endif // PINGUS_H
