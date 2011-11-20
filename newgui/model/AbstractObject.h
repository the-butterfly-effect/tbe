/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QtGlobal>
#include "Position.h"
#include "Property.h"

// ABOUT BODIES, SHAPES, JOINTS AND THE BASEOBJECT CLASS
//
//   In Box2D, we have Worlds, Bodies, Fixtures, Shapes and Joints:
//
//	 * World    - the "space" all bodies live in
//	 * Body     - the concept of any object in the world.
//                there are three types of bodies:
//                  * dynamic bodies
//					* static bodies
//                  * kinetic bodies (not used in TBE)
//				  This is described through the getObjectType() member
//                NOTE:  Box2D will automatically set mass to 1.0 if a
//                dynamic body has zero mass
//                NOTE2: only for static bodies, our code is allowed to do
//                movement by hand - otherwise movement is always handled by
//                the Box2D solver!!!
//   * Fixture  - describes how the shapes together form a body. concepts like
//                relative position, bounciness, friction and mass are
//                described here
//	 * Shape    - the shape of a body - a series of convex polygons or circles
//	 *            It is possible to use more than one shape in a body
//	 * Joint    - the link between two bodies, constraining relative movement
//
//	At instance creation, the bodydef and the shapedef will be created
//	At instance creation, also the body and shapes will be created
//
//	Upon deletion, body and shape are deleted but the defs are kept
//	Upon undelete, body and shape are re-created based on the defs
//

/**
  * class AbstractObject
  *
  * abstract base class for everything in the simulation model
  */

class AbstractObject
{
public:
	AbstractObject();

	enum SizeDirections
	{
		NORESIZING = 0,
		HORIZONTALRESIZE = 1,
		VERTICALRESIZE = 2,
		TOTALRESIZE = 3
	};

	/// (static const) minimum dimension of any (part of) object.
	static const float MINIMUM_DIMENSION;

	//
	// Getters and Setters
	// (sorted alphabetically)

    /** returns the Name of the object, for users.
     *  If a non-US locale is specified, the returned string
     *  will already be translated
     */
    virtual const QString getName ( );

	// TODO/FIXME: rename to getCenterPos()
	/// Get the Position of the object.
	/// This is the "original" center, i.e. where the object will
	/// be when not in simulation.
	/// @returns the value of theCenter
	virtual Position getOrigCenter ( ) const
		{ return theCenter; }

	/// make sure to override for objects that continously change size during
	/// the simulation
	virtual qreal getTempHeight() const
	{ return theHeight; }

	/// make sure to override for objects that continously change size
	/// during the simulation
	virtual qreal getTempWidth() const
	{ return theWidth; }

	/// Get the value of theBounciness
	/// (0.0 = stick, 1.0 = full elastic bounce)
	/// @return the value of theBounciness
	qreal getTheBounciness ( )
		{ return theBounciness; }

	/// Get the value of theHeight
	/// @return the value of theHeight
	qreal getTheHeight ( ) const
	{ return theHeight; }

	/// Get the value of theWidth
	/// @return the value of theWidth
	qreal getTheWidth ( ) const
	{ return theWidth; }

	/// returns true if the object can be moved by the user
	virtual bool isMovable ( ) const;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const // TODO/FIXME = 0;
	{	return resizableInfo;	}

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const;

	// TODO/FIXME: rename to setCenterPos()
	/// Set the value of theCenter - this is the "original" center,
	/// i.e. where the object will return to after a "reset".
	/// @param new_var the new value of theCenter
	virtual void setOrigCenter ( const Position& aNewPos )
	{ theCenter = aNewPos; }

	/// Set the value of theBounciness
	/// (0.0 = stick, 1.0 = full elastic bounce)
	/// @param new_var the new value of theBounciness
	void setTheBounciness ( qreal new_var )
	{ theBounciness = new_var; }

	/// Set the value of theHeight
	/// @param new_var the new value of theHeight
	virtual void setTheHeight ( qreal new_var )
	{ if (new_var>AbstractObject::MINIMUM_DIMENSION) theHeight = new_var;	}

	/// Set the value of theWidth
	/// @param new_var the new value of theWidth
	virtual void setTheWidth ( qreal new_var )
	{ if (new_var>AbstractObject::MINIMUM_DIMENSION) theWidth = new_var; }


	//
	// other public members
	//

	/** parse all properties
	  * NOTE: AbstractObject only understands PivotPoint and Bounciness
	  */
	virtual void  parseProperties(void);



private:
	// Private attributes of the Object
	//

	/** indicates how object should bounce - two objects of 0.0 will stick
	*  together, whereas two objects of 1.0 will bounce without loosing
	*  energy. Bounciness over 1.0 will resulting in ever larger bounces
	*  (not really realistic, right?)
	*/
	qreal theBounciness;

	/// the position of the center of the object when not in simulation.
	Position theCenter;

	/// static height of object (i.e. at rest before simulation starts)
	qreal theHeight;

	/** true if the user can move this object
	 *  Note that this has nothing to do with MovingObject or ImmovableObject
	 *  it has to do with the level design - as such, Level can modify this setting
	 */
	bool theIsMovable;

	/// the properties of the object instance
	PropertyList theProps;

	// TODO/FIXME: debugging code - should go away at some point
	friend class MainWindow;

	/// static width of the object (i.e. at rest before simulation starts)
	qreal theWidth;

	// TODO/FIXME: this one wasn't present in BaseObject
	// - but it was in RectObject...
	SizeDirections resizableInfo;
};

#endif // ABSTRACTOBJECT_H
