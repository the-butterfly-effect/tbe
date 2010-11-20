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

#include "TriggerExplosion.h"
#include "tbe_global.h"

//// the DetonatorBox class' ObjectFactory
class DetonatorBoxObjectFactory : public ObjectFactory
{
public:
	DetonatorBoxObjectFactory(void)
	{	announceObjectType("DetonatorBox", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new DetonatorBox()); }
};
static DetonatorBoxObjectFactory theDetonatorBoxObjectFactory;


const qreal DetonatorBox::ACTIVATED_TIME = 0.5;
const qreal DetonatorBox::RINGING_TIME   = 0.5;



DetonatorBox::DetonatorBox()
		:	RectObject( "DetonatorBox",
				"Push Here To Boom",
				"DetonatorBoxArmed;DetonatorBoxActivated;DetonatorBoxRinging;DetonatorBoxDone",
				0.35, 0.55, 4.0, 0.0)
{
}

DetonatorBox::~DetonatorBox()
{
}

void DetonatorBox::callbackStep (qreal aTimeStep, qreal aTotalTime)
{
}

DetonatorBox::States DetonatorBox::goToState(DetonatorBox::States aNewState)
{
	return ARMED;
}

void DetonatorBox::reportNormalImpulseLength(qreal anImpulseLength)
{
}

void DetonatorBox::reset(void)
{
}
