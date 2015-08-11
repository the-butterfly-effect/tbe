/* The Butterfly Effect 
 * This file copyright (C) 2009-2010  Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "UndoDeleteCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"
#include "World.h"
#include "toolbox.h"
#include "Anchors.h"

static ToolBox* theCurrentToolBoxPtr = NULL;

// Constructors/Destructors
//  

UndoDeleteCommand::UndoDeleteCommand (
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			isInUse(false)
{
	assert(aBaseObjectPtr);
	setText(QObject::tr("Delete %1").arg(theBaseObjectPtr->getName()));
}

UndoDeleteCommand::~UndoDeleteCommand ( )
{
	DEBUG5("~UndoDeleteCommand()/this=%p - %s\n", this, ASCII(text()));
	if (isInUse)
	{
		// upon deletion of this object, we finally can delete the objects it holds hostage, too
		delete theBaseObjectPtr;
		theBaseObjectPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

ToolBox* UndoDeleteCommand::getCurrentToolboxPtr(void)
{
	return theCurrentToolBoxPtr;
}


// Other methods
//  

void UndoDeleteCommand::push()
{
	theBaseObjectPtr->theDrawObjectPtr->getUndoStackPtr()->push(this);
}

void UndoDeleteCommand::redo ()
{
	DEBUG3("UndoDeleteCommand::redo() START\n");
	bool isDone = theBaseObjectPtr->deregister();
	Anchors::clearEditObjectDialogPtr();
	assert (isDone);
	UNUSED_VAR(isDone);
	if (theCurrentToolBoxPtr)
		theCurrentToolBoxPtr->modifyCountOfBaseObject(theBaseObjectPtr,+1);
	isInUse=true;
	DEBUG3("UndoDeleteCommand::redo() END\n");
}


void UndoDeleteCommand::setToolBoxPtr(ToolBox* aPtr)
{
	theCurrentToolBoxPtr = aPtr;
}


void UndoDeleteCommand::undo ()
{
	DEBUG3("UndoDeleteCommand::undo() START\n");
	if (theCurrentToolBoxPtr)
		theCurrentToolBoxPtr->modifyCountOfBaseObject(theBaseObjectPtr,-1);
	theBaseObjectPtr->reregister();
	isInUse=false;
	DEBUG3("UndoDeleteCommand::undo() END\n");
}
