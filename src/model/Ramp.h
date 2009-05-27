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

#ifndef RAMP_H
#define RAMP_H

#include "ImmovableObject.h"
#include <qobject.h>

/**
  * class Ramp
  * 
  * This class defines a ramp. Essentially a ramp is a slab (small box)
  * in a diagonal way. Using width and height, you can resize it. 
  */

class Ramp : public ImmovableObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	Ramp ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Ramp ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Ramp");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a ramp.");
	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return TOTALRESIZE;	}

	/**
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( dReal new_var );

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( dReal new_var );
	
	
private:
	/// TODO: lalalala
	void adjustParameters(void);
	
	static const dReal theSlabThickness = 0.03; // m
};

#endif // RAMP_H
