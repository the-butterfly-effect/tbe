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

#ifndef TRIGGEREXPLOSION_H
#define TRIGGEREXPLOSION_H

#include "RectObject.h"
#include "World.h"

/** this class implements the DetonatorBox for an explosion
 *  it has a sensor at the top that can be pressed,
 *  once pressed, it will send a trigger.
 *
 *  Note that we cheat on reality big time here. The original boxes were
 *  noting more than a dynamo - producing enough current for a spark.
 *  In our demo, we just hooked those wires up to a mobile phone...
 *  Suuuuure that will work ;-)
 */
class DetonatorBox : public RectObject
{
public:
	DetonatorBox();
	virtual ~DetonatorBox();

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from RectObject because this class wants to register for
	/// callbacks and needs to reset its state machine
	virtual void reset(void);

	/// this enum defines the states of the detonator
	enum States
	{
		ARMED,	   // handle up, cell display dark
		ACTIVATED, // handle down, cell display lighting up
		RINGING,   // radio waves visible, display lighted. at the beginning
				   // of this state we actually signal all bombs
				   // We might want to turn this into multiple states
				   // (for animation) later
		DONE       // end state, handle down and cell dark...
	};

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return theState; }

protected:
	/// call this function to suggest a state change to the DetonatorBox
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
	virtual States goToState(States aNewState);

	/// will replace the existing set of shapes by a smaller shape that
	/// fits the BalloonRest image. Do not call from within a Box2D callback
//	void switchToSmallShape(void);

public:
	// the following two members are part of the normal impulse reporting

	/// overridden from BaseObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from BaseObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength);

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	/// the state variable
	States theState;

	/// time that the handle came down, i.e. device got triggered
	qreal theActivationStartTime;

	/// time the object should stay in ACTIVATED state
	const static qreal ACTIVATED_TIME;

	/// time the object should stay in RINGING state
	const static qreal RINGING_TIME;
};



#endif // TRIGGEREXPLOSION_H
