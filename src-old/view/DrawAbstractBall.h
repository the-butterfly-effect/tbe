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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef DRAWABSTRACTBALL_H
#define DRAWABSTRACTBALL_H

#include "DrawObject.h"
#include "AbstractBall.h"

// forward declarations
class BaseObject;


/** class DrawAbstractBall
  *
  * This class only overrides one concept of the regular
  * DrawObject: it sets a round shape, which in turn reduces
  * the bounding box (for collission detection inside QT) from
  * a box to a circle.
  */

class DrawAbstractBall : public QObject, public DrawObject
{
	
	Q_OBJECT

public:
	// Constructors/Destructors
	//  

	/**
	 * Image Constructor
	 */
	DrawAbstractBall (BaseObject* aBaseObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawAbstractBall ( );

	/// overriden from DrawObject
	virtual QPainterPath shape() const;
};

#endif // DrawAbstractBall_H
