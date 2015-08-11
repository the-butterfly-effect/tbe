/* The Butterfly Effect
 * This file copyright (C) 2009,2011  Klaas van Gend
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

#ifndef COKEMENTOSBOTTLE_H
#define COKEMENTOSBOTTLE_H

#include "AbstractBall.h"
#include "PolyObject.h"
#include "World.h"

/** This class implements the flying Coke liquid.
 *  It is modelled in small bubbles, 10 grams each.
 */
class CokeSplatter : public AbstractBall
{
public:
	CokeSplatter();
	virtual ~CokeSplatter();

	/// returns true if the object should not surive a World::reset()
	/// overridden from BaseObject
	virtual bool isTemp() const
	{ return true; }

	/** sets all parameters of the splatter, attaches to World
	  * and creates the physical object and drawobject
	  * @param aWorldPtr
	  * @param aStartPos
	  * @param aVelocity
	  * @param aSplatterMass
	  */
	void setAll(World* aWorldPtr, const Position& aStartPos,
				qreal aVelocity, qreal aSplatterMass);

	/// overridden from BaseObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from BaseObject - if we have an impulse, we hit something
	  * and we'd better disband ourselves soon...
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength);

protected:
	const static qreal theRadius;
	bool hasRequestedRemoval;
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/** this class implements the famous coke+mentos trick:
  * insert a Mentos in a Coke bottle and it will blow a
  * huge fountain - until the bottle is empty.
  */
class CokeMentosBottle : public PolyObject, public SimStepCallbackInterface
{
public:
    CokeMentosBottle();

	enum BottleStatus
	{
		UNTRIGGERED,
		TRIGGERED,
		BLOWING,
		EMPTY
	};

	/// one of the two ways to trigger the blowing:
	void setBottleStatus(BottleStatus aNewStat);

	/// let's mis-use deletePhysicsObject to reset our object state
	virtual void deletePhysicsObject(void);

	/// @returns the current bottle state
	BottleStatus getBottleStatus(void)
	{ return theBottleStatus; }

	/// overridden from BaseObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual int getImageIndex(void)
	{ return theBottleStatus; }

	/// overridden from BaseObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from BaseObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength);

	/// overridden from BaseObject because this class wants to register for callbacks
	virtual void createPhysicsObject(void);

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	/** generate a new Coke splatter
	 *  @param aSequenceNr the sequence number of the splatter
	 */
	void newSplatter(unsigned int aSequenceNr);

private:
	// Private things

	/// adjusts the object's mass based on theCokeAmount variable
	void updateMass(void);


	// Things from RectObject that need adjustments:

	/// overridden from RectObject to remove the functionality
	virtual void adjustParameters(void)
	{ ; };


private:
	BottleStatus theBottleStatus;

	/// the amount of liquid still in the bottle
	/// 2.0l at start, reducing fast whilst blowing.
	/// unit: kg or l (who cares)
	/// (note: using the MASS property, you can adjust this)
	float theCokeAmount;

	/// the weight of an empty bottle in [kg]
	static const double theBottleMass;

	/// the number of splatters splat.
	/// used in the calculation of the exit velocity of the splatter
	int theSplatterCount;
	
	/// starts ticking once the bottle is triggered
	int theCountdown;

	/// constant that describes how much the coke splatter acts on the bottle
	float theThrust;


};

#endif // COKEMENTOSBOTTLE_H
