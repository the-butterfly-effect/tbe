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
#include "Position.h"
#include "ViewToolboxGroup.h"
#include "ViewObject.h"

#include <QBrush>

ViewToolboxGroup::ViewToolboxGroup(ToolboxGroup *aTBGPtr, QGraphicsItem *parent) :
	QGraphicsRectItem(parent), theTBGPtr(aTBGPtr)
{
	AbstractObject* myAOPtr = theTBGPtr->first();
	int theBigWidth = (static_cast<int>(THESCALE*myAOPtr->getTheWidth()) & ~31) | 27;
	int theBigHeight = (static_cast<int>(THESCALE*myAOPtr->getTheHeight()) & ~31) | 27;

//	if (theBigWidth < THESCALE*myAOPtr->getTheWidth())
		theBigWidth += 32;
//	if (theBigHeight < THESCALE*myAOPtr->getTheHeight())
		theBigHeight += 32;

	setBrush(QBrush(Qt::gray));
	setRect(0,0, theBigWidth, theBigHeight);
	ViewObject* myVOPtr = myAOPtr->createViewObject();
	myVOPtr->setParentItem(this);
	myVOPtr->moveBy(theBigWidth/2, theBigHeight/2);

	theCount.setHtml(QObject::tr("<b>%1x</b>").arg(theTBGPtr->count()));
	theCount.setParentItem(this);
	theCount.setZValue(5);
	theName.setPlainText(theTBGPtr->theGroupName.result());
	theName.setParentItem(this);
	theName.setZValue(5);
	theName.moveBy(theBigWidth-theName.boundingRect().width(),
				   theBigHeight-theName.boundingRect().height());
}
