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

	// Public attribute accessor methods
	//

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return QObject::tr("Scenery");
	}

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{
		// no tooltip, no i18n
		return "";
	}

	/// returns true if the object can be moved by the user
	/// Scenerys cannot be adjusted by the player
	virtual bool isMovable ( ) const
		{ return false; }

	/// returns true if the object can be rotated by the user
	/// Scenerys cannot be adjusted by the player
	virtual bool isRotatable ( ) const
	{	return false; }

	/// returns whether the object can be resized by the user
	/// Scenerys cannot be adjusted by the player
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	virtual DrawObject* createDrawObject();

	/// this member fixes up the physical model based on new width or height
	/// overridden: no physical model
	virtual void adjustParameters(void)
	{ ; }

};

#endif // Scenery_H
