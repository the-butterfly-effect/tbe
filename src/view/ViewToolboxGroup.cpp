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
#include "Popup.h"
#include "Position.h"
#include "ViewToolboxGroup.h"
#include "ViewObject.h"
#include "ViewWorld.h"

#include <QtGui/QBrush>
#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QGraphicsSceneMouseEvent>

ViewToolboxGroup::ViewToolboxGroup(ToolboxGroup *aTBGPtr, QGraphicsItem *parent) :
	QGraphicsRectItem(parent), theTBGPtr(aTBGPtr)
{
	theCount.setParentItem(this);
	theEmpty.setParentItem(this);
	theName.setParentItem(this);

	int myObjW;
	int myObjH;
	if (theTBGPtr->count() > 0)
	{
		AbstractObject* myAOPtr = theTBGPtr->first();
		myObjW  = THESCALE*myAOPtr->getTheWidth();
		myObjH  = THESCALE*myAOPtr->getTheHeight();

		ViewObject* myVOPtr = myAOPtr->createViewObject();
		thePixmapPtr = new QGraphicsPixmapItem(myVOPtr->pixmap());
		thePixmapPtr->setTransform(myVOPtr->transform());
		thePixmapPtr->setParentItem(this);

		setToolTip(myAOPtr->getToolTip());
	}
	else
	{
		theEmpty.setPlainText(QObject::tr("(empty)"));
		myObjW = theEmpty.boundingRect().width();
		myObjH  = theEmpty.boundingRect().height();
		theEmpty.setZValue(5);
	}

	theName.setPlainText(theTBGPtr->theGroupName.result() + " ");
	if (theName.boundingRect().width() > myObjW)
		myObjW = theName.boundingRect().width();

	theBigWidth = (static_cast<int>(myObjW) & ~31) +16+32;
	theBigHeight = (static_cast<int>(myObjH) & ~31) +16+32;

	setBrush(QBrush(Qt::gray));
	setRect(0,0, theBigWidth-5, theBigHeight-5);

	if (theTBGPtr->count() > 0)
	{
		thePixmapPtr->setPos((theBigWidth-myObjW)/2,
							 (theBigHeight-myObjH)/2);
	}
	else
	{
		theEmpty.setPos((theBigWidth-theEmpty.boundingRect().width())/2.0,
					   (theBigHeight-theEmpty.boundingRect().height())/2.0);
	}

	theName.setZValue(5);
	theName.setPos(theBigWidth-theName.boundingRect().width(),
				   theBigHeight-theName.boundingRect().height());


	theCount.setHtml(QString("<font size=\"+1\"><b>%1x</b></font>")
					 .arg(theTBGPtr->count()));
	theCount.setZValue(5);

	setAcceptsHoverEvents(true);
}


ViewToolboxGroup::~ViewToolboxGroup()
{
	// nothing to do?
}


void ViewToolboxGroup::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
    QGraphicsEffect* myEffect = new QGraphicsColorizeEffect();
    setGraphicsEffect(myEffect);
}


void ViewToolboxGroup::hoverLeaveEvent ( QGraphicsSceneHoverEvent* )
{
    setGraphicsEffect(NULL);
}


void ViewToolboxGroup::mousePressEvent ( QGraphicsSceneMouseEvent* event)
{
    if (ViewWorld::getIsSimRunning()==true)
    {
        Popup::Info(tr("The simulation is not in rest, you cannot insert new things. Please reset the sim first!"));
        event->ignore();
        return;
    }

    if (theTBGPtr->count() > 0)
    {
        InsertUndoCommand::createInsertUndoCommand(theTBGPtr);
    }
    event->accept();
    emit hideMe();
}
