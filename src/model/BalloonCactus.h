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

#ifndef BALLOONCACTUS_H
#define BALLOONCACTUS_H

#include "AbstractBall.h"
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

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from PolyObject because this class wants to register for
	/// callbacks and needs to restart its state machine
	virtual void createPhysicsObject(void);

	/// let's mis-use deletePhysicsObject to reset our object state
	virtual void deletePhysicsObject(void);

	/// deletePhysicsObject() doesn't really delete the physics object
	/// anymore - but we need a true deleter here...
	void deletePhysicsObjectForReal(void);

	/// this enum defines the states of the balloon
	enum States
	{
		BALLOON,
		POPPING,
		POPPED,
		GONE
	};

	/// called by a Cactus or BedOfNails if the balloon is hit
	void stung(void);

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return theState; }

	/// overridden to make sure joints are not re-created outside BALLOON state
	virtual void notifyJoints(JointInterface::JointStatus aStatus);

protected:
	/// call this function to suggest a state change to the Balloon
	/// note that the Balloon can decide not to follow your state change,
	/// going from popped to balloon isn't supported (just like real life)
	/// @param aNewState the suggestion for a new state
	/// @returns the state after this function completes
	virtual States goToState(States aNewState);

	/// will replace the existing set of shapes by a smaller shape that
	/// fits the BalloonRest image. Do not call from within a Box2D callback
	void switchToSmallShape(void);

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

	virtual void callbackStepBalloon (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepPopped  (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepPopping (qreal aTimeStep, qreal aTotalTime);

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
	virtual void callBackSensor(const ContactInfo& aPoint);

	/** Overridden from PolyObject because we also have a sensor
	 *  here...
	 */
	virtual void fillShapeList(void);
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
	virtual void callBackSensor(const ContactInfo& aPoint);

	/** Just like Cactus, we need to redefine the shapes
	 */
	virtual void fillShapeList(void);
};


///---------------------------------------------------------------------------
///------------------------ CircularSaw --------------------------------------
///---------------------------------------------------------------------------

/** This class implements a CircularSaw (blade).
  */
class CircularSaw : public AbstractBall
{
public:
	CircularSaw();

	virtual ~CircularSaw();

	/// Overridden so we will get notified if something poppable hits our
	/// sharp points
	virtual void callBackSensor(const ContactInfo& aPoint);

	/** Overridden from AbstractBall because we also have to define
	 *  a sensor here...
	 */
	virtual void createBallShapeFixture(float aRadius, float aMass);

	virtual DrawObject* createDrawObject();

	/// we need to adjust the size based on the radius
	virtual void  parseProperties(void);
};


#endif // Balloon_H
