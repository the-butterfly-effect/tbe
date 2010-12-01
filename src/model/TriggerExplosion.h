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
#include "PolyObject.h"
#include "World.h"

class DetonatorBoxHandle;


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
	virtual ~DetonatorBox();

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

	/// overridden to be able to create the handle
	virtual void createPhysicsObject(void);

	/// overridden from BaseObject to allow for the handle
	virtual void deletePhysicsObject(void);

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return theState; }

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject in order to also move the handle
	virtual void setOrigCenter ( Position new_var );

	/// overridden from RectObject because this class wants to register for
	/// callbacks and needs to reset its state machine
	virtual void reset(void);

	/// called by theDetonatorBoxHandle when triggered
	void setTriggered(void);

protected:
	/// call this function to suggest a state change to the DetonatorBox
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
	virtual States goToState(States aNewState);

	/// will replace the existing set of shapes by a smaller shape that
	/// fits the BalloonRest image. Do not call from within a Box2D callback
//	void switchToSmallShape(void);


private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

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
	DetonatorBoxHandle* theHandleObjectPtr;

private:
	// disable copy constructor / assignment operator
	DetonatorBox(const DetonatorBox& aBORefToCopy);
	DetonatorBox& operator = (const DetonatorBox& aBORefToCopy);

	friend class DetonatorBoxHandle;
};


/** specific class to handle the nocollision (towards DetonatorBox itself)
  * and no serialization
  * (because it is part of DetonatorBox and shouldn't exist by itself)
  */
class DetonatorBoxHandle : public RectObject, public SimStepCallbackInterface
{
private:
	/// @param aDBox pointer to a DetonatorBox, the only object allowed to create a Handle
	DetonatorBoxHandle(DetonatorBox* aDBox, const Position& aPos);

public:
	virtual ~DetonatorBoxHandle();

	/// overridden to allow setting a custom ZValue
	virtual DrawObject* createDrawObject();

	/// overridden from BaseObject to allow for the special joints
	void createPhysicsObject(void);

	/// overridden from BaseObject to allow for the special joints
	virtual void deletePhysicsObject(void);

	qreal getDistance(void);

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from RectObject because this class wants to register for
	/// callbacks
	virtual void reset(void);

	friend class DetonatorBox;

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	DetonatorBox* theDBoxPtr;
	b2PrismaticJoint* theJointPtr;

private:
	// disable copy constructor / assignment operator
	DetonatorBoxHandle(const DetonatorBoxHandle& aBORefToCopy);
	DetonatorBoxHandle& operator = (const DetonatorBoxHandle& aBORefToCopy);
};


/** this class implements the Dynamite for an explosion
 *  it will be triggered by the DetonatorBox class
 */
class Dynamite : public PolyObject, public SimStepCallbackInterface
{
public:
	Dynamite();
	virtual ~Dynamite();

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from RectObject because this class wants to register for
	/// callbacks and needs to reset its state machine
	virtual void reset(void);

	/// this enum defines the states of the detonator
	/// apart from WAITING->ACTIVE, all states are time-triggered
	enum States
	{
		WAITING,   // cell phone dark, nothing happening
		ACTIVE,    // cell phone lighted, "incoming call"
		RINGING,   // cell phone lighted, plus radio waves
		BOOM,	   // explosion happening
		GONE,	   // main object gone (explosion particles may still fly)
	};

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return theState; }

protected:
	/// call this function to suggest a state change to the Dynamite
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
	virtual States goToState(States aNewState);

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	/// true when the cell phone has been rung
	bool isRinging;

	/// the state variable
	States theState;

	/// time that the the active state started
	qreal theActiveStartTime;

	/// time the object should stay in RINGING state
	const static qreal RINGING_TIME;
};

#endif // TRIGGEREXPLOSION_H
