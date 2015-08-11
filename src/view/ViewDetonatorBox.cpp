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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObject.h"
#include "ChoosePhoneNumber.h"
#include "ImageCache.h"
#include "TriggerExplosion.h"
#include "ViewDetonatorBox.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//

ViewDetonatorBox::ViewDetonatorBox (AbstractObjectPtr aAbstractObjectPtr, const QString& anImageName)
	: ViewObject(aAbstractObjectPtr, anImageName)
{
	// Everything is done in the ViewObject constructor...
	DEBUG5ENTRY;
}

ViewDetonatorBox::~ViewDetonatorBox ( )
{
}

//
// Methods
//


// Accessor methods
//


// Other methods
//

void ViewDetonatorBox::displayChoosePhoneNumber(void)
{
	// display a combobox with those numbers
    DetonatorBox* myDetBoxPtr = dynamic_cast<DetonatorBox*>(theAbstractObjectPtr);
	assert (myDetBoxPtr != NULL);
	ChoosePhoneNumber* myDialogPtr = new ChoosePhoneNumber(myDetBoxPtr, ResizingGraphicsView::me());
	myDialogPtr->appearAnimated();

	// The dialog will set the phone numbers itself, no need for us to worry
	// it will also clean up after itself, I hope :-)
}


void ViewDetonatorBox::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
	realHoverEnterEvent();
}


void ViewDetonatorBox::mousePressEvent ( QGraphicsSceneMouseEvent* anEvent )
{
	theClickedScenePos = anEvent->scenePos();
	QTimer::singleShot(thePieMenuDelay, this, SLOT(realMousePressEvent()));
}
