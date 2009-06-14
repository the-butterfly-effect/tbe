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

#include "tbe_global.h"

#include "HoverPointer.h"
#include "ImageStore.h"
#include "PieMenu.h"
#include "DrawObject.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>



HoverPointer::HoverPointer(QGraphicsScene* aScene, DrawObject* aParent)
		: QGraphicsSvgItem(), theDrawObjectPtr(aParent)
{
	setSharedRenderer(ImageStore::getRenderer(PieMenu::getEditModeIconName()));

	QRectF mySquare=aScene->views()[0]->mapToScene(QRect(0,0,32,32)).boundingRect();
	printf("32 pix = %f hori / %f verti\n", mySquare.width(), mySquare.height());
	// so we now know that our image should be reduced to width&height
	scale(mySquare.width()/boundingRect().width(),
		  mySquare.height()/boundingRect().height());
	aScene->addItem(this);
	setZValue(1.0);

	theOffset=mySquare.width()/2.0;
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}


void HoverPointer::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("HoverPointer::mouseMoveEvent(%d)\n", event->type());
	theDrawObjectPtr->mouseMoveEvent(event);
	QGraphicsSvgItem::mouseMoveEvent(event);
}


void HoverPointer::setPos(const QPointF& aPos)
{
	QGraphicsSvgItem::setPos(aPos.x()-theOffset,aPos.y()-theOffset);
}

void HoverPointer::setPos(qreal x, qreal y)
{
	QGraphicsSvgItem::setPos(x-theOffset,y-theOffset);
}
