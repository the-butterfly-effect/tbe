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

#ifndef RATBREAD_H
#define RATBREAD_H

#include "RectObject.h"
#include "World.h"

#include <QStringList>

/** this class implements the Rat:
  * a Rat will walk to the closest bread it sees.
  * It will:
  *   * sniff in the air before it runs
  *   * if another bread comes closer, it will sniff again
  *   * run up/down ramps (up to 90 degrees!)
  * It will not:
  *   * jump across spaces, it will just fall down "like a lemming"
  */
class Rat : public RectObject, public SimStepCallbackInterface
{
public:
	Rat();

	virtual ~Rat();

	// note: the rat is more like
	enum RatStates
	{
		// only move from SLEEPING to SNIFFING if there
		// is something close enough, i.e. 0-4 m.
		SLEEPING,
		  SLEEPING1 = SLEEPING,
		  SLEEPING2,
		// two stages for sniffing in the air
		SNIFFING,
		  SNIFFINGLEFT = SNIFFING,
		  SNIFFINGRIGHT,
		// three stages for walking animations
		WALKING,
		  WALKING1 = WALKING,
		  WALKING2,
		  WALKING3,
		// nomming = eating in cheezburger language (see icanhazcheezburger.com)
		NOMMING,
		  NOMMING1 = NOMMING,
		  NOMMING2
	};

	virtual void reset(void);

protected:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	/// call this member to iterate through all Food items
	/// (food items have a property "Food" set to true)
	/// in addition to returning a pointer to the closest food object,
	/// it will also set theClosestFoodPtr
	/// @returns pointer to closest food item
	BaseObject* findClosestFood();

	/// returns a vector from the current position to the Food center
	Vector getVectorToFood(BaseObject* aFoodPtr) const;

	/// switch to state aNewState (if possible)
	/// @param  aNewState the suggested new state
	/// @returns the state after the goTo completes - that's very likely aNewState
	virtual RatStates goToState(RatStates aNewState);

private:
	virtual void callbackStepSleeping (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepSniffing (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepWalking (qreal aTimeStep, qreal aTotalTime);
	virtual void callbackStepNomming (qreal aTimeStep, qreal aTotalTime);



	qreal theStartTimeCurrentState;
	BaseObject* theClosestFoodPtr;

	RatStates theCurrentState;
};

#endif // RATBREAD_H
