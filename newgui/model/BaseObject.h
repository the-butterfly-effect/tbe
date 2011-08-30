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

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QtGlobal>
#include "Position.h"


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
  * class BaseObject
  *
  * abstract base class for everything in the simulation model
  */

class BaseObject
{
public:
    BaseObject();


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

	/// Get the value of theHeight
	/// @return the value of theHeight
	qreal getTheHeight ( ) const
	{ return theHeight; }

	/// Get the value of theWidth
	/// @return the value of theWidth
	qreal getTheWidth ( ) const
	{ return theWidth; }

	/// Set the value of theCenter - this is the "original" center,
	/// i.e. where the object will return to after a "reset".
	/// @param new_var the new value of theCenter
	virtual void setOrigCenter ( const Position& aNewPos )
	{ theCenter = aNewPos; }

	/// Set the value of theHeight
	/// @param new_var the new value of theHeight
	virtual void setTheHeight ( qreal new_var )
		{ if (new_var>0.01) theHeight = new_var;	}

	/// Set the value of theWidth
	/// @param new_var the new value of theWidth
	virtual void setTheWidth ( qreal new_var )
		{ if (new_var>0.01) theWidth = new_var; }




private:
	// Private attributes of the Object
	//

	/// the position of the center of the object when not in simulation.
	Position theCenter;

	/// static width of the object (i.e. at rest before simulation starts)
	qreal theWidth;

	/// static height of object (i.e. at rest before simulation starts)
	qreal theHeight;

	/** indicates how object should bounce - two objects of 0.0 will stick
	*  together, whereas two objects of 1.0 will bounce without loosing
	*  energy. Bounciness over 1.0 will resulting in ever larger bounces
	*  (not really realistic, right?)
	*/
	qreal theBounciness;

};

#endif // BASEOBJECT_H
