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

#ifndef FLOWER_H
#define FLOWER_H

#include "RectObject.h"

/**
  * class Flower
  * 
  * This class defines a Flower.
  * It is nothing special, except that a Flower can retrieveits position through
  * two static global functions...
  */

class Flower : public RectObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	Flower ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Flower ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Flower");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a Flower. Flowers attract Butterflies.");
	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// @returns a pointer to a flower
	static Flower* getFlowerPtr(void);

	friend class Butterfly;
};

#endif // Flower_H
