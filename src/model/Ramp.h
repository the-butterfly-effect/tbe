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

#include "RectObject.h"

/**
  * class RightRamp
  * 
  * This class defines a ramp. Essentially a ramp is a slab (small box)
  * in a diagonal way. Using width and height, you can resize it. 
  */

class RightRamp : public RectObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	RightRamp ( );

	/**
	 * Empty Destructor
	 */
	virtual ~RightRamp ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("RightRamp");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a ramp. The left is higher than the right, so things slide to the right.");
	}

	/// overridden from RectObject: returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return true;	}

	/** overridden from RectObject:
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( qreal new_var );

	/** overridden from RectObject:
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( qreal new_var );
	
	/// overridden from RectObject
	virtual DrawObject* createDrawObject();
	
protected:
	/// this member fixes up the physical model based on new width or height
	void adjustParameters(void);

	static const qreal theSlabThickness = 0.04; // m

	bool isRight;
	friend class DrawRamp;
};


class LeftRamp : public RightRamp
{
public:
	/**
	 * Empty Constructor
	 */
	LeftRamp ( ) : RightRamp ()
	{ isRight=false; adjustParameters(); }

	/**
	 * Empty Destructor
	 */
	virtual ~LeftRamp ( ) {};

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("LeftRamp");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return QObject::tr("This is a ramp. The right is higher than the left, so things slide to the left.");
	}

};

#endif // RAMP_H
