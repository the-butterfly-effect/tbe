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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "UndoDeleteCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"
#include <QGraphicsScene>
#include "World.h"
#include "toolbox.h"

static ToolBox* theCurrentToolBoxPtr = NULL;

// Constructors/Destructors
//  

UndoDeleteCommand::UndoDeleteCommand (
		DrawObject* aDrawObjectPtr, 
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			theDrawObjectPtr(aDrawObjectPtr),
			isInUse(false)
{
	assert(aDrawObjectPtr);
	assert(aBaseObjectPtr);


	setText("Delete " + theBaseObjectPtr->getName());
}

UndoDeleteCommand::~UndoDeleteCommand ( )
{
	DEBUG1("~UndoDeleteCommand() for %p - %s\n", this, ASCII(text()));
	if (isInUse)
	{
		// upon deletion of this object, we finally can delete the objects it holds hostage, too
		delete theBaseObjectPtr;
		theBaseObjectPtr = NULL;
		// deleting theBaseObjectPtr should also delete the DrawObject :-)
		theDrawObjectPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

void UndoDeleteCommand::push()
{
	theDrawObjectPtr->getUndoStackPtr()->push(this);
	isInUse=true;
}


void UndoDeleteCommand::redo ()
{
	DEBUG3("UndoDeleteCommand::redo() START\n");
	// remove the DrawObject from the scene, but keep the pointer
	assert(theBaseObjectPtr->deregister());
	if (theCurrentToolBoxPtr)
		theCurrentToolBoxPtr->announceReturnOfBaseObject(theBaseObjectPtr);
	DEBUG3("UndoDeleteCommand::redo() END\n");
}


void UndoDeleteCommand::setToolBoxPtr(ToolBox* aPtr)
{
	theCurrentToolBoxPtr = aPtr;
}


void UndoDeleteCommand::undo ()
{
	DEBUG3("UndoDeleteCommand::undo() START\n");
	theBaseObjectPtr->reregister();
	DEBUG3("UndoDeleteCommand::undo() END\n");
}
