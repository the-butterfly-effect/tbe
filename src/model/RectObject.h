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

#ifndef RECTOBJECT_H
#define RECTOBJECT_H

#include "BaseObject.h"

/**
  * class RectObject
  * 
  * This class defines a rectangular object. It can have any size - it
  * works around the limitation of Box2D that the width/height should not be more than 10 or less than 1/10th.
  * It can have a mass, but it doesn't need to: If it does not have a mass, it won't move in the simulation.
  * Many things will use this class as a base - it should replace Floor and Wall for example.
  *
  *
  * Parameters that can be set through Properties:
  *   * ObjectName  - name under which the object can appear in the toolbox
  *   * Bounciness  - number between 0 and 2.0 - note that 1.0 already is very bouncy!
  *   * Resizable   - "none", "hori", "verti" or "both"
  *   * Mass        - in kg
  *   * ImageName   - png or svg file, do not specify the extension
  *   * Description - the "tooltip" - description shown when hovering over the object
  */

class RectObject : public BaseObject
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	RectObject ( );

	/**
	 * Empty Destructor
	 */
	virtual ~RectObject ( );

	// Public attribute accessor methods
	//  

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return theNameString;
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		return theToolTipString;
	}

	/// set property aKey to aValue
	/// overridden from BaseObject
	virtual void  setProperty(const QString& aKey, const QString& aValue);

	/// returns true if the object can be rotated by the user
	/// FIXME/TODO: MUST BE FLEXIBLE
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	/// FIXME/TODO: MUST BE FLEXIBLE
	virtual SizeDirections isResizable ( ) const
	{	return TOTALRESIZE;	}


	/**
	 * Set the value of theWidth
	 * @param new_var the new value of theWidth
	 */
	virtual void setTheWidth ( qreal new_var );

	/**
	 * Set the value of theHeight
	 * @param new_var the new value of theHeight
	 */
	virtual void setTheHeight ( qreal new_var );
	
	virtual DrawObject* createDrawObject();
	
protected:
	/// this member fixes up the physical model based on new width or height
	void adjustParameters(void);


	QString theNameString;
	QString theToolTipString;
};

#endif // RECTOBJECT_H
