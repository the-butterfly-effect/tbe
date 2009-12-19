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

#ifndef DRAWRAMP_H
#define DRAWRAMP_H

#include "tbe_global.h"
#include "DrawObject.h"


/** A Ramp (both RightRamp and LeftRamp) have the problem that the
  * BoundingRect() does not match the true form of the Ramp well.
  * So this DrawRamp class only exists to fix that.
  */
class DrawRamp : public DrawObject
{
public:
	/**
	 * SVG Constructor
	 */
	DrawRamp (BaseObject* aBaseObjectPtr, const QString& anImageName, ImageType anImageType = IMAGE_ANY);

	/// standard virtual destructor
	virtual ~DrawRamp();

	/// overridden from QGraphicsItem to improve collision detection
	virtual QPainterPath shape() const;
};

#endif // DRAWRAMP_H
