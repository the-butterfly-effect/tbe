/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#include "AbstractObject.h"
#include "GameResources.h"
#include "InsertUndoCommand.h"
#include "Position.h"
#include "ViewToolboxGroup.h"
#include "ViewObject.h"

#include <QtGui/QBrush>
#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QGraphicsSceneMouseEvent>

ViewToolboxGroup::ViewToolboxGroup(ToolboxGroup *aTBGPtr, QGraphicsItem *parent) :
	QGraphicsRectItem(parent), theTBGPtr(aTBGPtr)
{
	AbstractObject* myAOPtr = theTBGPtr->first();
	int myObjW  = THESCALE*myAOPtr->getTheWidth();
	theEmpty.setPlainText(QObject::tr("(empty)"));
	theName.setPlainText(theTBGPtr->theGroupName.result() + " ");
	if (theEmpty.boundingRect().width() > myObjW)
		myObjW = theEmpty.boundingRect().width();
	if (theName.boundingRect().width() > myObjW)
		myObjW = theName.boundingRect().width();

	theBigWidth = (static_cast<int>(myObjW) & ~31) +16+32;
	int myObjH  = THESCALE*myAOPtr->getTheHeight();
	theBigHeight = (static_cast<int>(myObjH) & ~31) +16+32;

	setBrush(QBrush(Qt::gray));
	setRect(0,0, theBigWidth-5, theBigHeight-5);
	ViewObject* myVOPtr = myAOPtr->createViewObject();
	thePixmapPtr = new QGraphicsPixmapItem(myVOPtr->pixmap());
	thePixmapPtr->setTransform(myVOPtr->transform());
	myAOPtr->deleteViewObject();
	thePixmapPtr->moveBy((theBigWidth-THESCALE*myAOPtr->getTheWidth())/2,
						 (theBigHeight-myObjH)/2);
	thePixmapPtr->setParentItem(this);

	// theCount.setText is in updateCount()
	theCount.setParentItem(this);
	theCount.setZValue(5);
	// theEmpty.setText is above
	theEmpty.setParentItem(this);
	theEmpty.setZValue(5);
	theEmpty.moveBy((theBigWidth-theEmpty.boundingRect().width())/2.0,
				   (theBigHeight-theEmpty.boundingRect().height())/2.0);
	// theName.setText is above
	theName.setParentItem(this);
	theName.setZValue(5);
	theName.moveBy(theBigWidth-theName.boundingRect().width(),
				   theBigHeight-theName.boundingRect().height());

	updateCount();
	setAcceptsHoverEvents(true);
}


ViewToolboxGroup::~ViewToolboxGroup()
{
	// nothing to do?
}


void ViewToolboxGroup::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
    // this looks great, but unfortunately it also affects all children
    QGraphicsEffect* myEffect = new QGraphicsColorizeEffect();
    setGraphicsEffect(myEffect);
}


void ViewToolboxGroup::hoverLeaveEvent ( QGraphicsSceneHoverEvent* )
{
    setGraphicsEffect(NULL);
}

void ViewToolboxGroup::mousePressEvent ( QGraphicsSceneMouseEvent* event)
{
    if (theTBGPtr->count() > 0)
    {
        InsertUndoCommand::createInsertUndoCommand(theTBGPtr);
        updateCount();
    }
    event->accept();
    emit hideMe();
}

void ViewToolboxGroup::updateCount(void)
{
	theCount.setHtml(QString("<font size=\"+1\"><b>%1x</b></font>")
					 .arg(theTBGPtr->count()));
	if (theTBGPtr->count() > 0)
	{
		thePixmapPtr->setVisible(true);
		theEmpty.setVisible(false);
	}
	else
	{
		thePixmapPtr->setVisible(false);
		theEmpty.setVisible(true);
	}
}
