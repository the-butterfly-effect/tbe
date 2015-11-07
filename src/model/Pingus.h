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

#include "PolyObject.h"
#include "World.h"


/** This class implements a Pingus, the small penguin from the
  * game 'Pingus'. We're borrowing this nice penguin to make a
  * few nice puzzle levels that have a familiar feeling.
  */
class Pingus : public PolyObject, public SimStepCallbackInterface
{
public:
	Pingus();

	virtual ~Pingus();

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const override
	{	return NORESIZING;	}

	/// overridden from PolyObject because this class wants to register for
	/// callbacks and needs to restart its state machine
	virtual void createPhysicsObject(void) override;

	/// let's mis-use deletePhysicsObject to reset our object state
	virtual void deletePhysicsObject(void) override;

	/// deletePhysicsObject() doesn't really delete the physics object
	/// anymore - but we need a true deleter here...
	void deletePhysicsObjectForReal(void);

	/// This enum defines the states of the Pingus
	/// It's (not by accident) the same as the row number in the image
	enum States
	{
		WALKINGLEFT = 0,
		WALKINGRIGHT,
		FALLING,
		SLIDELEFT,
		SLIDERIGHT,
		SPLATTING,
		DEAD
	};

	static const int FramesPerState[];

	/// overridden from AbstractObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual unsigned int getImageIndex(void) const override
	{ return theState; }

protected:
	/// call this function to suggest a state change to the Pingus
	/// note that the Pingus can decide not to follow your state change,
	/// going from popped to Pingus isn't supported (just like real life)
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
    States goToState(States aNewState);

	/// will replace the existing set of shapes by a smaller shape that
	/// fits the Splatting and Dead images.
	/// Do not call from within a Box2D callback
	void switchToSmallShape(void);

public:
	// the following two members are part of the normal impulse reporting

	/// overridden from AbstractObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void) override
	{ return true; }

	/** overridden from AbstractObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength) override;

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

	void callbackStepSliding (qreal aTimeStep, qreal aTotalTime);
	void callbackStepSplatting (qreal aTimeStep, qreal aTotalTime);
	void callbackStepWalking (qreal aTimeStep, qreal aTotalTime);

private:
	// Private things

	/// the state variable
	States theState;

	/// Within a state, there are several animation frames, this is the index into the list
	int theAnimationFrameIndex;

	/// we need to keep track whether the splatting is done
	qreal theSplattingTimeStart;
};


#endif // PINGUS_H
