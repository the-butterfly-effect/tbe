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

#include "DrawObject.h"
#include "BaseObject.h"

DrawObject::DrawObject (BaseObject* aBaseObjectPtr)
	: theBaseObjectPtr(aBaseObjectPtr)
{
	DEBUG6("StubDrawObject::DrawObject(%p)\n", aBaseObjectPtr);
	if (theBaseObjectPtr!=NULL)
		initAttributes();
}

DrawObject::DrawObject (BaseObject* aBaseObjectPtr,
						const QString& anImageName,
						UNUSED_ARG DrawObject::ImageType anImageType)
	: theBaseObjectPtr(aBaseObjectPtr)
{
	DEBUG6("StubDrawObject::DrawObject(%p,%s)\n", aBaseObjectPtr, ASCII(anImageName));
	initAttributes();
}


DrawObject::~DrawObject ( )
{
}


QRectF DrawObject::boundingRect() const
{
	qreal myWidth = theBaseObjectPtr->getTheWidth();
	qreal myHeight= theBaseObjectPtr->getTheHeight();
	qreal adjust = 0.03;

	return QRectF(-myWidth/2-adjust, -myHeight/2-adjust, myWidth+2*adjust, myHeight+2*adjust);
}


void DrawObject::initAttributes ( )
{
}


void DrawObject::paint(QPainter*, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void DrawObject::setupCache(void)
{
}
