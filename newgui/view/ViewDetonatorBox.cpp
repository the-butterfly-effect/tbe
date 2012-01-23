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

#include "ViewDetonatorBox.h"
#include "AbstractObject.h"
#include "ChoosePhoneNumber.h"
#include "TriggerExplosion.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//

ViewDetonatorBox::ViewDetonatorBox (AbstractObject* aAbstractObjectPtr, const QString& anImageName)
	: ViewObject(aAbstractObjectPtr, anImageName)
{
	// everything is done in the ViewObject constructor
	DEBUG5("ViewDetonatorBox\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);
	setCacheMode(QGraphicsItem::NoCache);
	setAcceptHoverEvents(true);
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
	QGraphicsView* myViewPtr = scene()->views()[0];
	DetonatorBox* myDetBoxPtr = dynamic_cast<DetonatorBox*>(theAbstractObjectPtr);
	assert (myDetBoxPtr != NULL);
	ChoosePhoneNumber* myDialogPtr = new ChoosePhoneNumber(myDetBoxPtr, myViewPtr);
	myDialogPtr->setAutoFillBackground(true);
	QSize myDialogSize = myDialogPtr->size();
	QSize myViewSize = myViewPtr->size();
	Position myC = theAbstractObjectPtr->getOrigCenter();
	QPoint myPosInView = myViewPtr->mapFromScene(myC.x,-myC.y);
	if (myPosInView.x()+myDialogSize.width() > myViewSize.width())
		myPosInView.setX(myViewSize.width() - 1.2*myDialogSize.width());
	if (myPosInView.y()+myDialogSize.height() > myViewSize.height())
		myPosInView.setY(myViewSize.height() - 1.2*myDialogSize.height());
	myDialogPtr->move(myPosInView);
	myDialogPtr->show();

	// The dialog will set the phone numbers itself, no need for us to worry
	// it will also clean up after itself, I hope :-)
}

//void ViewDetonatorBox::hoverEnterEvent ( QGraphicsSceneHoverEvent* anEvent)
//{
//	ViewObject::hoverEnterEvent(anEvent);
//}

//void ViewDetonatorBox::hoverLeaveEvent ( QGraphicsSceneHoverEvent* anEvent)
//{
//	ViewObject::hoverLeaveEvent(anEvent);
//}



void ViewDetonatorBox::initAttributes ( )
{

}

void ViewDetonatorBox::mousePressEvent(QGraphicsSceneMouseEvent* anEvent)
{
	// depending on whether we're level editor or not, move or display
	if (theIsLevelEditor)
		ViewObject::mousePressEvent(anEvent);
	else
	{
		displayChoosePhoneNumber();
		anEvent->accept();
	}
}

//void ViewDetonatorBox::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *myStyle, QWidget *myWPtr)
//{
//	ViewObject::paint(myPainter, myStyle, myWPtr);

//	if (isHovering)
//	{
//		qreal myWidth = theAbstractObjectPtr->getTheWidth()*theScale;
//		qreal myHeight= theAbstractObjectPtr->getTheHeight()*theScale;
//		QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

//		QPen   myPen(Qt::NoPen);
//		myPainter->setPen(myPen);
//		QColor myColor(255,255,255,155);
//		myPainter->setBrush(myColor);

//		myPainter->drawRect(myRect);
//	}
//}
