/* The Butterfly Effect 
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#include "UndoMoveCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"

#include <cassert>

// Constructors/Destructors
//  

UndoMoveCommand::UndoMoveCommand (
		BaseObject* aBaseObjectPtr,
		const Vector& aHotSpot)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr),
			theHotSpot(aHotSpot)
{
	assert(aBaseObjectPtr);

	setText(QObject::tr("Move %1").arg(theBaseObjectPtr->getName()));
	DEBUG5("UndoMoveCommand() for %p - %s\n", this, ASCII(text()));

	theOldPosition = theBaseObjectPtr->getOrigCenter();
	theLastGoodPosition = theOldPosition;
}

UndoMoveCommand::~UndoMoveCommand ( ) 
{
	DEBUG5("~UndoMoveCommand() for %p - %s\n", this, ASCII(text()));
	// there's nothing to do here...
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  
void UndoMoveCommand::redo ()
{
	theBaseObjectPtr->setOrigCenter(theNewPosition);
	theBaseObjectPtr->reset();
	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);
	theBaseObjectPtr->theDrawObjectPtr->advance(0);
}

bool UndoMoveCommand::revertIfNeeded(void)
{
	DrawObject* theDOPtr = theBaseObjectPtr->theDrawObjectPtr;

	// are we currently in a collision?
	// in that case, go back to last known good
	if (theDOPtr->checkForCollision())
	{
		DEBUG4("Reverting to last known non-colliding position\n");
		revertToLastGood();
		theDOPtr->update(theDOPtr->boundingRect());
		theDOPtr->removeCollisionCross();
		return true;
	}
	return false;
}

void UndoMoveCommand::undo ()
{
	theBaseObjectPtr->setOrigCenter(theOldPosition);
	theBaseObjectPtr->reset();
	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);
	theBaseObjectPtr->theDrawObjectPtr->focusRemove();
	theBaseObjectPtr->theDrawObjectPtr->advance(0);
}
