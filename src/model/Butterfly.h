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
	{
		return QObject::tr("Butterfly");
	}

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject because this class wants to register for callbacks
	virtual void reset(void);

	/// called if Object has registered a sensor share
	/// Butterfly needs to know if it has hit another object
	/// overridden from SensorInterface
	virtual void callBackSensor(b2ContactPoint* aCPPtr);

	/// overriden from BaseObject
	/// because we need to instantiate a special DrawButterfly class
	virtual DrawObject* createDrawObject();

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

private:
	// Private things

	// Things from RectObject that need adjustments:

	/// overridden from RectObject to remove the functionality
	virtual void adjustParameters(void)
	{ ; };


private:

	/// the weight of the butterfly (10 grams)
	static const double theButterflyMass = 0.01;

	/// true when the bottle's sensor is touching something
	bool hasContact;
};

#endif // BUTTERFLY_H
