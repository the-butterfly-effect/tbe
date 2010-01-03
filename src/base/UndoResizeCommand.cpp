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

#include "UndoResizeCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"

#include <cmath>

// Constructors/Destructors
//  

UndoResizeCommand::UndoResizeCommand (
		DrawObject* aDrawObjectPtr, 
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			theDrawObjectPtr(aDrawObjectPtr)
{
	assert(aDrawObjectPtr);
	assert(aBaseObjectPtr);
	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theLastGoodCenter = theOldCenter;
	theNewCenter = theBaseObjectPtr->getTempCenter();
	theOldSize = QPointF(theBaseObjectPtr->getTheWidth(),theBaseObjectPtr->getTheHeight());
	theLastGoodSize = theOldSize;
	setText("Resize " + theBaseObjectPtr->getName());
}

UndoResizeCommand::~UndoResizeCommand ( )
{
	DEBUG5("~UndoResizeCommand() for %p - %s\n", this, ASCII(text()));
	// there's nothing to do here...
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

bool UndoResizeCommand::isResized(void)
{
	if (theNewSize.isNull())
		return false;

	QPointF theDelta = theOldSize-theNewSize;
	if (fabs(theDelta.x()) < Position::minimalMove && fabs(theDelta.y()) < Position::minimalMove)
		return false;
	return true;
}


void UndoResizeCommand::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.x());
	theBaseObjectPtr->setTheHeight(theNewSize.y());

	theBaseObjectPtr->setTempCenter(theNewCenter);
	theBaseObjectPtr->setOrigCenter(theNewCenter);

	theBaseObjectPtr->reset();
	theDrawObjectPtr->applyPosition();
}


void UndoResizeCommand::revertToLastGood(void)
{
	theNewSize = theLastGoodSize;
	theNewCenter = theLastGoodCenter;
	theDrawObjectPtr->removeCollisionCross();
	redo();
}


void UndoResizeCommand::setDelta(qreal anAnchorPos, QPointF aDelta)
{
	aDelta.setY(-aDelta.y());

	// check if new size is possible
	QPointF theTempSize = theOldSize + anAnchorPos*aDelta;
	if (theTempSize.x() < 0.1 ||
		theTempSize.y() < 0.1)
		return;

	theNewSize = theTempSize;

	theNewCenter = theOldCenter + 0.5*aDelta;
	redo();

	// collision detection - only do this *after* the redraw :-)
	isColliding = theDrawObjectPtr->checkForCollision();
	if (isColliding == false)
	{
		theLastGoodSize = theNewSize;
		theLastGoodCenter = theNewCenter;
	}

}


void UndoResizeCommand::undo ()
{
	theBaseObjectPtr->setTheWidth(theOldSize.x());
	theBaseObjectPtr->setTheHeight(theOldSize.y());

	theBaseObjectPtr->setTempCenter(theOldCenter);
	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->reset();
	theDrawObjectPtr->focusRemove();
	theDrawObjectPtr->applyPosition();
}
