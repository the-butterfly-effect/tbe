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

#include "PolyObject.h"
#include "World.h"


/** this class implements a Balloon
  *
  */
class Balloon : public PolyObject, public SimStepCallbackInterface
{
public:
	Balloon();

	virtual ~Balloon();

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject because this class wants to register for callbacks
	virtual void reset(void);


	/// overridden to prevent adjusting width/height
	virtual void setTheHeight ( qreal new_var );

	/// overridden to prevent adjusting width/height
	virtual void setTheWidth ( qreal new_var );



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

private:
	// Private things

	Position thePreviousPosition;
};

class Cactus : public PolyObject
{
public:
	Cactus();

	virtual ~Cactus();

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}
};

class BedOfNails : public Cactus
{
public:
	BedOfNails();

	virtual ~BedOfNails();

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getName ( ) const
	{	return QObject::tr("BedOfNails"); }

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Do not touch a bed of nails - it stings!"); }

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}
};


#endif // Balloon_H
