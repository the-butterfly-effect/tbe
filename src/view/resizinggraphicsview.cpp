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

#include "tbe_global.h"
#include "resizinggraphicsview.h"
#include "DrawObject.h"
#include "UndoDeleteCommand.h"
#include "ImageStore.h"


// local, always existing variable.
// It is kept current every time by the constructor and/or resize
// through the updatePixelsPerUnit() member
static float thePixelsPerSceneUnitHorizontal;

ResizingGraphicsView::ResizingGraphicsView (QWidget* aParent)
	: QGraphicsView(aParent), isDoNotResize(false)
{
	setAlignment(Qt::AlignCenter);

	setInteractive(true);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::ContainsItemShape);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QSizePolicy vpol;
	vpol.setHorizontalPolicy(QSizePolicy::Expanding);
	vpol.setVerticalPolicy(QSizePolicy::Expanding);
	setSizePolicy(vpol);
	updatePixelsPerUnit();
}

float ResizingGraphicsView::getPixelsPerSceneUnitHorizontal(void)
{
	return thePixelsPerSceneUnitHorizontal;
}

void ResizingGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	// if we're running out of the widget to the right, let's start the Drag&Drop
	QPoint myPos = event->pos();

	// limit our efforts: for real objects, while we're actually dragging
	// OK, here's a dirty trick.
	// If we get here, we hopefully know that myItem is a DrawObject
	// if it is not, dynamic_cast will return NULL.
	DrawObject* myDOPtr = dynamic_cast<DrawObject*>(itemAt(myPos));
	if (myDOPtr!=NULL && event->buttons()==Qt::LeftButton)
	{
		BaseObject* myBOPtr = myDOPtr->getBaseObjectPtr();

		// calculate wether we are close to the right border...
		//
		// I need to map the width of the object to the width of the view.
		// Unfortunately, that can only be done for QPoints: a detour :-(
		QPointF myObjWidth = mapFromScene(myBOPtr->getTheWidth()/2.0, 0.0);
		if (myPos.x() + myObjWidth.x() > width()-5 && myBOPtr->isMovable())
		{
			UndoDeleteCommand* myCommandPtr =
					new UndoDeleteCommand(myBOPtr);

			// create MIME data
			QByteArray itemData;
			QDataStream dataStream(&itemData, QIODevice::WriteOnly);
			dataStream << "TBE-lala"; //item->getID();
			QMimeData* myMimeDataPtr = new QMimeData;
			myMimeDataPtr->setData(TBItem::DrawWorldMimeType, itemData);

			// add an icon to the QDrag
			QPixmap myPixmap = ImageStore::getPNGPixmap("ToRight")->scaledToWidth(32);
			QDrag *drag = new QDrag(this);
			drag->setMimeData(myMimeDataPtr);
			drag->setHotSpot(QPoint(myPixmap.width()/2, myPixmap.height()/2));
			drag->setPixmap(myPixmap);

			if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
			{
				myCommandPtr->push();
				return;
			}

			delete myCommandPtr;
			myCommandPtr = NULL;
		}
	}

	// no, we still have an object to move around
	QGraphicsView::mouseMoveEvent(event);
}

void ResizingGraphicsView::on_timerTick(void)
{
	if (scene())
		QGraphicsView::fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
	updatePixelsPerUnit();
}

void ResizingGraphicsView::updatePixelsPerUnit()
{
	QPolygon myPoly = mapFromScene(QRectF(0,0,1,0));
	thePixelsPerSceneUnitHorizontal = myPoly.boundingRect().width();
	DEBUG5("ResizingGraphicsView::updatePixelsPerUnit() - now is %f\n", thePixelsPerSceneUnitHorizontal);

	if (theTBECaching==true)
	{
		// also redo all caching for our objects
		DrawWorld* myScene = dynamic_cast<DrawWorld*>(scene());
		if (myScene != NULL)
			myScene->invalidateCaching();
	}
}
