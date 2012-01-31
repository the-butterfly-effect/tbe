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

#ifndef DRAWDETONATORBOX_H
#define DRAWDETONATORBOX_H

#include "DrawObject.h"

// forward declarations
class BaseObject;


/** class DrawDetonatorBox
  *
  * This class draws the DetonatorBox class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class DrawDetonatorBox : public QObject, public DrawObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/// Image Constructor
	DrawDetonatorBox (BaseObject* aBaseObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawDetonatorBox ( );

	/// this member starts the display of the actual ChoosePhoneNumber dialog
	void displayChoosePhoneNumber(void);

	/** overridden from QGraphicsItem
	 * Upon a single click event, let's choose the phone number
	 * (just move if in level creator - there's an icon for choosephonenumber)
	 */
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* anEvent);

	/// overriden from DrawObject to allow extra graphics on hovering
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

protected:
	/// overridden to allow highlighting
	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	/// overridden to allow highlighting
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

private:
	virtual void initAttributes ( ) ;

	bool isHovering;

	// prevent copy constructor / assignment operator
	DrawDetonatorBox(const DrawDetonatorBox&);
	const DrawDetonatorBox& operator= (const DrawDetonatorBox&);

};

#endif // DRAWDETONATORBOX_H
