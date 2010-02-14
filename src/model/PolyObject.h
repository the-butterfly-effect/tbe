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

#ifndef POLYOBJECT_H
#define POLYOBJECT_H

#include "BaseObject.h"

/// forward declarations
struct b2PolygonDef;

/**
  * class PolyObject
  *
  * This class defines an object consisting of one or more convex polygons.
  * It can be resized, as long as all the polygons reach a minimum area.
  * It can have a mass, but it doesn't need to: If it does not have a mass, it won't move in the simulation.
  * Many things will use this class as a base.
  *
  * Parameters that can be set through Properties are defined in Property.h
  */

class PolyObject : public BaseObject
{
public:

	// Constructors/Destructors
	//

	/**
	 * Empty Constructor
	 */
	PolyObject ( );

	/// elaborate constructor, only used by the ConcreteRectObject thingies
	PolyObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& anImageName,
				const QString& anOutline,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness);

	/// Destructor
	virtual ~PolyObject ( );

	// Public attribute accessor methods
	//

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return theNameString;	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return theToolTipString; }

	/// parse all properties
	/// partially overridden from BaseObject
	virtual void  parseProperties(void);

	/// returns true if the object can be rotated by the user
	/// FIXME/TODO: MUST BE FLEXIBLE
	virtual bool isRotatable ( ) const
	{	return rotatableInfo;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}


protected:
	void setFriction(b2PolygonDef* aBoxDef);

protected:
	QString theNameString;
	QString theToolTipString;

	bool rotatableInfo;
};

#endif // POLYOBJECT_H
