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

#ifndef WALL_H
#define WALL_H

#include "ImmovableObject.h"
#include <qobject.h>

/**
  * class Floor
  * 
  * This class defines a floor. Essentially a ramp is a slab (small box)
  * It is only resizable in width. 
  */

class Wall : public ImmovableObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	Wall ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Wall ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Wall");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a wall piece: vertical of course.");
	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return VERTICALRESIZE;	}

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( qreal new_var );

	virtual DrawObject* createDrawObject();
	
private:
	static const qreal theSlabThickness = 0.03; // m
};

#endif // WALL_H
