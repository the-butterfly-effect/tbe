/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#include "DrawObject.h"
#include "BaseObject.h"

#include "tbe_global.h"

DrawObject::DrawObject(BaseObject* aBaseObjectPtr) :
	QGraphicsPixmapItem(NULL), theBaseObjectPtr(aBaseObjectPtr)
{
	// nothing to do yet :-)
	Q_ASSERT(aBaseObjectPtr!=NULL);
}

DrawObject::DrawObject(BaseObject* aBaseObjectPtr, const QString& anImageName) :
	QGraphicsPixmapItem(NULL), theBaseObjectPtr(aBaseObjectPtr)
{
	DEBUG5ENTRY;
	Q_ASSERT(aBaseObjectPtr!=NULL);
	Q_ASSERT(anImageName.isEmpty() == false);

	// FIXME/TODO: Introduce multiple images handling
	// FIXME/TODO: Introduce SVG rendering - or maybe still use QGraphicsSvgItem for that?
	QPixmap* myPixmapPtr = new QPixmap(anImageName);
	Q_ASSERT(myPixmapPtr!=NULL);
	setPixmap(*myPixmapPtr);

	// set origin, scale, rotate
	QTransform theTransform;
	theTransform.scale(theBaseObjectPtr->getTheWidth()/boundingRect().width(),
					   theBaseObjectPtr->getTheHeight()/boundingRect().height());
	theTransform.translate(-boundingRect().width()/2.0,-boundingRect().height()/2.0);
	setTransform(theTransform,false);

	setPos(theBaseObjectPtr->getOrigCenter().toQPointF());

	// TODO/FIXME: make rotation work correctly
	setRotation(theBaseObjectPtr->getOrigCenter().angleInDegrees());
}

DrawObject::~DrawObject()
{
	// nothing to do yet :-)
}

void DrawObject::rotateSomeMore(void)
{
	setRotation(5);
}

