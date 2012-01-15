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

	/// elaborate constructor, only one to use
	explicit
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

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if the property mass was set
	virtual b2BodyType getObjectType(void) const;

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return theToolTipString; }

	/// parse all properties
	/// partially overridden from BaseObject
	virtual void  parseProperties(void);

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

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

	/// overridden so we can use our DrawPolyObject
	virtual DrawObject* createDrawObject();

protected:
	/// TODO/FIXME: see same notes in RectObject...
	virtual void setFriction(b2FixtureDef* aFixtureDef);

	/** Parse the property POLYGONS_STRING into
	 *  shapes.
	 *  note: does not clear the shapelist!
	 */
	virtual void fillShapeList(void);

private:
	PolyObject(const PolyObject& aRef);
	PolyObject& operator = (const PolyObject& aRef);
	
protected:
	QString theNameString;
	QString theToolTipString;

	/// AABB calculates the Axis-Aligned Bounding Box
	/// which we can use for scaling
	class AABB {
	public:
		// default constructor, just to make sure we can detect uninit usage
		AABB(void);

		/// constructor
		/// @param myPolygons string with the polygons definiton
		AABB(QString& myPolygons);

		/// @returns the width of the unscaled object
		float getOrigWidth();
		/// @returns the height the unscaled object
		float getOrigHeight();
		/// @returns true if the AABB are initialised
		bool isInitialised();

	private:
		float theOrigWidth;
		float theOrigHeight;

		static const float UNDEFINED = 99999.0f;
	};

	AABB theAABB;
};

#endif // POLYOBJECT_H
