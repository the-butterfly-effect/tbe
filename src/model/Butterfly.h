/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include "RectObject.h"
#include "World.h"


/** this class implements the Butterfly:
  * a Butterfly always flies to a flower - if it sees one
  */
class Butterfly : public RectObject, public SimStepCallbackInterface
{
public:
	Butterfly();

	virtual ~Butterfly();

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getName ( ) const
	{	return QObject::tr("Butterfly"); }

	/// overridden from AbstractObject because this class wants to register for callbacks
	virtual void createPhysicsObject(void);

	enum ButterflyStatus
	{
		STILL,				// not implemented yet
		FLAP_OPEN,
		FLAP_HALF,
		DEAD				// not implemented yet
	};

	/// @returns the state of the butterfly state machine
	ButterflyStatus getState(void) const
	{	return theButterflyState; }

	/// overridden from AbstractObject to allow representation of the states
	/// @returns: returns a numerical index similar to the state
	unsigned int getImageIndex(void) const override
	{ return getState(); }

	/** sets up the Butterfly to fly to a Flower
	  * or flap idly if there is no flower
	  */
	void goToFlower(void);


public:
	// the following two members are part of the normal impulse reporting

	/// overridden from AbstractObject - we want reports on NormalImpulse
	bool isInterestedInNormalImpulse(void) override
	{ return true; }

	/** overridden from AbstractObject - we want to receive
	  * reports on the normal impulse.
	  * @param anImpulseLength length of the normal impulse vector
	  */
	void reportNormalImpulseLength(qreal anImpulseLength) override;

protected:
	/// suggest a new state of the butterfly state machine
	void setState(ButterflyStatus aNewStateSuggestion);

private:
	/// implemented from SimStepCallbackInterface
	void callbackStep (qreal aTimeStep, qreal aTotalTime) override;

private:
	// Private things

	// Things from RectObject that need adjustments:

private:
	/// the weight of the butterfly (100 grams)
	static const double theButterflyMass;

	/// central variable of the Butterfly state machine
	ButterflyStatus theButterflyState;

	/// FIXME: TODO
	signed int theCountdown;

	/// if moving to a flower, this is the target position for the butterfly
	Position theTargetPos;
};

#endif // BUTTERFLY_H
