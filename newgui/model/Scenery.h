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

#ifndef SCENERY_H
#define SCENERY_H

#include "RectObject.h"


/** The Scenery class is just foreground/background graphics
 *  Never moveable
 *
 *  Scenerys do not have a body or shape in simulation
 */
class Scenery : public RectObject
{
public:
	Scenery();

	virtual ~Scenery();

	/// Make sure there is no physics body by overriding
	/// the creational member of AbstractObject.
	virtual void createPhysicsObject(const Position&)
	{ };

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Scenery");
	}

	/// Scenery has no mass. But no b2Body will be created either :-)
	virtual b2BodyType getObjectType(void) const
	{	return b2_staticBody; }

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		// no tooltip, no i18n
		return "";
	}

	/// returns true if the object can be rotated by the user
	/// Scenerys cannot be adjusted by the player
	virtual bool isRotatable ( ) const
	{	return false; }

	/// returns whether the object can be resized by the user
	/// Scenerys cannot be adjusted by the player
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/** Creates the ViewObject, finds associated images,
	  * sets ZValue and returns a pointer to it.
	  * Overridden to set a default depth of 0.1 so it moves behind everything.
	  * @param   aDefaultDepth, ZValue depth in view if not set as property,
	  *          the higher the value the more likely it is drawn on top
	  * @returns pointer to ViewObject
	  * @note: a ZValue set in a property always overrides aDefaultDepth
	  */
	virtual ViewObject* createViewObject(float aDefaultDepth = 0.1)
	{ return RectObject::createViewObject(aDefaultDepth); }
};

#endif // Scenery_H
