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

#ifndef VIEWCIRCLEOBJECT_H
#define VIEWCIRCLEOBJECT_H

#include "ViewObject.h"
#include "CircleObjects.h"

// forward declarations
//class AbstractObject;


/** class ViewCircleObject
  *
  * This class only overrides one concept of the regular
  * DrawObject: it sets a round shape, which in turn reduces
  * the bounding box (for collission detection inside QT) from
  * a box to a circle.
  */

class ViewCircleObject : public QObject, public ViewObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/**
	 * Image Constructor
	 */
	ViewCircleObject (AbstractObject* aAbstractObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewCircleObject ( );

	/// overriden from DrawObject
	virtual QPainterPath shape() const;
};

#endif // ViewCircleObject_H
