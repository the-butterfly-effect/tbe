/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef VIEWDETONATORBOX_H
#define VIEWDETONATORBOX_H

#include "ViewObject.h"

// forward declarations
class AbstractObject;

/** class ViewDetonatorBox
  *
  * This class draws the DetonatorBox class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class ViewDetonatorBox : public ViewObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/// Image Constructor
	ViewDetonatorBox (AbstractObject* aAbstractObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewDetonatorBox ( );

	/// this member starts the display of the actual ChoosePhoneNumber dialog
	void displayChoosePhoneNumber(void);

protected:
	/// overridden to ALWAYS allow highlighting
	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	/// overridden to ALWAYS allow mouse button presses
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event);

private:
	// prevent copy constructor / assignment operator
	ViewDetonatorBox(const ViewDetonatorBox&);
	const ViewDetonatorBox& operator= (const ViewDetonatorBox&);
};

#endif // VIEWDETONATORBOX_H
