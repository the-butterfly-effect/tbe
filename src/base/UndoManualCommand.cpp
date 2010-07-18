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

#include "UndoManualCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"
#include "Anchors.h"

#include <cassert>
#include <cmath>

// Constructors/Destructors
//  

UndoManualCommand::UndoManualCommand (
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr)
{
	assert(aBaseObjectPtr);
	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theLastGoodCenter = theOldCenter;
	theNewCenter = theBaseObjectPtr->getTempCenter();
	theOldSize = QPointF(theBaseObjectPtr->getTheWidth(),theBaseObjectPtr->getTheHeight());
	theLastGoodSize = theOldSize;
	setText(QObject::tr("Resize %1").arg(theBaseObjectPtr->getName()));
}

UndoManualCommand::~UndoManualCommand ( )
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

bool UndoManualCommand::isResized(void)
{
	if (theNewSize.isNull())
		return false;

	QPointF theDelta = theOldSize-theNewSize;
	if (fabs(theDelta.x()) < Position::minimalMove && fabs(theDelta.y()) < Position::minimalMove)
		return false;
	return true;
}

void UndoManualCommand::pushYourself(void)
{
	theBaseObjectPtr->theDrawObjectPtr->pushUndo(this);
}


void UndoManualCommand::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.x());
	theBaseObjectPtr->setTheHeight(theNewSize.y());

	theBaseObjectPtr->setTempCenter(theNewCenter);
	theBaseObjectPtr->setOrigCenter(theNewCenter);

	theBaseObjectPtr->reset();
	DrawObject* myDOPtr = theBaseObjectPtr->theDrawObjectPtr;
	myDOPtr->applyPosition();
	if (myDOPtr->theAnchorsPtr != NULL)
		myDOPtr->theAnchorsPtr->updatePosition();
	else
		myDOPtr->theAnchorsPtr = new Anchors(myDOPtr);
}


void UndoManualCommand::revertToLastGood(void)
{
	theNewSize = theLastGoodSize;
	theNewCenter = theLastGoodCenter;
	theBaseObjectPtr->theDrawObjectPtr->removeCollisionCross();
	redo();
}


void UndoManualCommand::setNewVal(Position aPos)
{
	theNewCenter = aPos;
	redo();
	// no need for collision detection - we're the manual one
}

void UndoManualCommand::setNewVal(qreal anWidth, qreal aHeight)
{
	theNewSize.setX(anWidth);
	theNewSize.setY(aHeight);

	// It is unfair: QT doesn't redraw if just the size changed
	// So we need to force a position change...
	double myRandom = 0.0001 * qrand() / (double)RAND_MAX;

	theNewCenter.x += myRandom;
	redo();
	theNewCenter.x -= myRandom;

	// no need for collision detection - we're the manual one
}


void UndoManualCommand::undo ()
{
	theBaseObjectPtr->setTheWidth(theOldSize.x());
	theBaseObjectPtr->setTheHeight(theOldSize.y());

	theBaseObjectPtr->setTempCenter(theOldCenter);
	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->reset();
	theBaseObjectPtr->theDrawObjectPtr->focusRemove();
	theBaseObjectPtr->theDrawObjectPtr->applyPosition();
}
