/* The Butterfly Effect
 * This file copyright (C) 2009,2011,2012  Klaas van Gend
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

#ifndef COLAMENTOSBOTTLE_H
#define COLAMENTOSBOTTLE_H

#include "CircleObjects.h"
#include "PolyObject.h"
#include "World.h"

/** This class implements the flying Cola liquid.
 *  It is modelled in small bubbles, 10 grams each.
 */
class ColaSplatter : public CircleObject
{
public:
	ColaSplatter();
	virtual ~ColaSplatter();

	/// @returns true if the object should not surive a World::deletePhysicsWorld()
	/// overridden from AbstractObject
	virtual bool isTemp() const
	{ return true; }

    /** sets all parameters of the splatter
	  * @param aWorldPtr
	  * @param aStartPos
	  * @param aVelocity
	  * @param aSplatterMass
	  */
	void setAll(World* aWorldPtr, const Position& aStartPos,
				qreal aVelocity, qreal aSplatterMass);

	/// overridden from AbstractObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from AbstractObject - if we have an impulse, we hit something
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


/** this class implements the famous cola+mint trick:
  * insert a Mint in a Cola bottle and it will blow a
  * huge fountain - until the bottle is empty.
  */
class ColaMintBottle : public PolyObject, public SimStepCallbackInterface
{
public:
	ColaMintBottle();

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

	/// overridden from AbstractObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	virtual unsigned int getImageIndex(void) const
	{ return theBottleStatus; }

	/// overridden from AbstractObject - we want reports on NormalImpulse
	virtual bool isInterestedInNormalImpulse(void)
	{ return true; }

	/** overridden from AbstractObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	virtual void reportNormalImpulseLength(qreal anImpulseLength);

	/// overridden from AbstractObject because this class wants to register for callbacks
	virtual void createPhysicsObject(void);

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	/** generate a new Cola splatter
	 *  @param aSequenceNr the sequence number of the splatter
	 */
	void newSplatter(unsigned int aSequenceNr);

private:
	// Private things

	/// adjusts the object's mass based on theColaAmount variable
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
	float theColaAmount;

	/// the weight of an empty bottle in [kg]
	static const double theBottleMass;

	/// the number of splatters splat.
	/// used in the calculation of the exit velocity of the splatter
	int theSplatterCount;

	/// starts ticking once the bottle is triggered
	int theCountdown;

	/// constant that describes how much the ColaSplatter acts on the bottle
	float theThrust;


};

#endif // COLAMENTOSBOTTLE_H
