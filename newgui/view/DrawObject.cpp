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
#include "ImageStore.h"

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
	// (note: ImageRenderStore does already handle SVG if needed, but it guesses for bitmap sizes)
	QPixmap* myPixmapPtr = ImageRendererStore::getPixmap(anImageName);
	Q_ASSERT(myPixmapPtr!=NULL);
	setPixmap(*myPixmapPtr);

	// FIXME/TODO: set origin, scale, rotate
}

DrawObject::~DrawObject()
{
	// nothing to do yet :-)
}


