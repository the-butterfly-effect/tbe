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

#include "DrawRamp.h"
#include "Ramp.h"


DrawRamp::DrawRamp (BaseObject* aBaseObjectPtr,
						const QString& anImageName,
						DrawObject::ImageType anImageType)
	: DrawObject(aBaseObjectPtr, anImageName, anImageType)
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawRamp\n");
}

DrawRamp::~DrawRamp ( )
{
	// nothing to be done here, either...
}


QPainterPath DrawRamp::shape() const
{
	QPainterPath myPath;

	QRectF myRect = boundingRect();
	RightRamp* myPtr = reinterpret_cast<RightRamp*>(theBaseObjectPtr);

	if (myPtr->isRight)
	{
		myPath.moveTo(myRect.topLeft());
		myPath.lineTo(myRect.bottomRight());
		myPath.lineTo(myRect.bottomLeft());
		myPath.closeSubpath();
	}
	else
	{
		myPath.moveTo(myRect.topRight());
		myPath.lineTo(myRect.bottomLeft());
		myPath.lineTo(myRect.bottomRight());
		myPath.closeSubpath();
	}

	return myPath;
}
