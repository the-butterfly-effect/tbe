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

#ifndef BOWLINGBALL_H
#define BOWLINGBALL_H

#include "MovingObject.h"
#include <qobject.h>

/**
  * class BowlingBall
  * 
  * it implements a normal bowling ball - heavy and big
  * we've decided not to try to model the holes for grip :-)
  * and we're very lazy - we've decide to model the ball as uniform massive
  */

class BowlingBall : public MovingObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	BowlingBall ( );

	/**
	 * Empty Destructor
	 */
	virtual ~BowlingBall ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Bowling Ball");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("Your average bowling ball - heavy, "
				"round and willing to roll");
	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns true if the object can be resized by the user
	virtual bool isResizable ( ) const
	{	return false;	}

};

#endif // BOWLINGBALL_H
