/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#ifndef COKEMENTOSBOTTLE_H
#define COKEMENTOSBOTTLE_H

#include "RectObject.h"
#include "World.h"

/** this class implements the flying coke
 *  it is modelled in small bubbles, 10 grams each
 */
class CokeSplatter : public RectObject
{
public:
	CokeSplatter();
	virtual ~CokeSplatter();

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getName ( ) const
	{
		// don't translate this one - not visible to user, ever...
		return "CokeSplatter";
	}


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

	/// reset() has no effect on a CokeSplatter
	/// overridden from RectObject
	virtual void reset(void)
	{ ; }

	/// called if Object has registered a sensor share
	/// CokeSplatter needs to know if it has hit another object
	///  - because that implies that we should delete ourselves soon...
	/// overridden from SensorInterface
	virtual void callBackSensor(b2ContactPoint* aCPPtr);

protected:
	/// overridden from RectObject to remove the functionality
	virtual void adjustParameters(void)
	{ ; }

	const static qreal theRadius = 0.04;
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/** this class implements the famous coke+mentos trick:
  * insert a mentos in a coke bottle and it will blow a
  * huge fountain - until the bottle is empty.
  */
class CokeMentosBottle : public RectObject, public SimStepCallbackInterface
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

	/// returns true if the object can be rotated by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual bool isRotatable ( ) const
	{	return true;	}

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// one of the two ways to trigger the blowing:
	void setBottleStatus(BottleStatus aNewStat);

	/// one of the two ways to trigger the blowing:
	BottleStatus getBottleStatus(void)
	{ return theBottleStatus; }


	/// overridden from BaseObject because this class wants to register for callbacks
	virtual void reset(void);

	/// called if Object has registered a sensor share
	/// CokeMentosBottle needs to know if it has hit another object
	///  - because that implies that we might need to start blowing
	/// overridden from SensorInterface
	virtual void callBackSensor(b2ContactPoint* aCPPtr);

	/// overriden from BaseObject
	virtual DrawObject* createDrawObject();

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
	static const double theBottleMass = 0.2;

	/// the number of splatters splat.
	/// used in the calculation of the exit velocity of the splatter
	int theSplatterCount;
	
	/// true when the bottle's sensor is touching something
	bool hasContact;

	/// starts ticking once the bottle is triggered
	int theCountdown;

	/// used to calculate the velocity change - which is used to see if the bottle is hit
	b2Vec2 thePreviousVelocity;

	/// used to calculate the angular velocity change - which is used to see if the bottle is hit
	float thePreviousAngVelocity;

	/// constant that describes how much the coke splatter acts on the bottle
	qreal theThrust;


};

#endif // COKEMENTOSBOTTLE_H
