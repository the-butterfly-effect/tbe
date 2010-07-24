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

#include "UndoInsertCommand.h"
#include "UndoMoveCommand.h"

#include "BaseObject.h"
#include "DrawObject.h"
#include "World.h"
#include "toolbox.h"

#include <QGraphicsSceneMouseEvent>

// Constructors/Destructors
//  

UndoInsertCommand::UndoInsertCommand (
		BaseObject* aBaseObjectPtr)
		  : UndoDeleteCommand(aBaseObjectPtr)
{
	setText(QObject::tr("Insert %1").arg(theBaseObjectPtr->getName()));
	DEBUG1("UndoInsertCommand()/this=%p - %s\n", this, ASCII(text()));
}

UndoInsertCommand::~UndoInsertCommand ( )
{
	DEBUG1("~UndoInsertCommand()/this=%p - %s\n", this, ASCII(text()));
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

void UndoInsertCommand::cancel(void)
{
	theBaseObjectPtr->deregister();
	delete theBaseObjectPtr;
	theBaseObjectPtr=NULL;
}

bool UndoInsertCommand::checkForValidPositionOrRevert(void)
{
	bool isGood = true;
	UndoMoveCommand* myUMCPtr = theBaseObjectPtr->theDrawObjectPtr->theUndoMovePtr;
	if (myUMCPtr->revertIfNeeded()==true)
	{
		if (myUMCPtr->isChanged()==false)
		{
			// we're in trouble: the object never was valid - we need to cancel D&D
			isGood = false;
		}
		// we're in good shape: even though the last position was illegal,
		// we did have a good position in the history
	}
	delete myUMCPtr;
	theBaseObjectPtr->theDrawObjectPtr->theUndoMovePtr = NULL;
	return isGood;
}

void UndoInsertCommand::redo ()
{
	DEBUG3("UndoInsertCommand::redo() START\n");

	if (getCurrentToolboxPtr())
		getCurrentToolboxPtr()->modifyCountOfBaseObject(theBaseObjectPtr,-1);
	theBaseObjectPtr->reregister();
	isInUse=false;
	DEBUG3("UndoInsertCommand::redo() END\n");
}

void UndoInsertCommand::setNewPosition(const Position& aNewPos)
{
	theBaseObjectPtr->theDrawObjectPtr->mouseMoveEvent(QPointF(aNewPos.x, aNewPos.y));
}

void UndoInsertCommand::undo ()
{
	DEBUG3("UndoInsertCommand::undo() START\n");
	bool isDone = theBaseObjectPtr->deregister();
	assert (isDone);
	UNUSED_VAR(isDone);
	if (getCurrentToolboxPtr())
		getCurrentToolboxPtr()->modifyCountOfBaseObject(theBaseObjectPtr,+1);
	isInUse=true;
	DEBUG3("UndoInsertCommand::undo() END\n");
}
