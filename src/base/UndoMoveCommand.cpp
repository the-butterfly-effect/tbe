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

#include "UndoMoveCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"

// TODO: FIXME: this include should not be necessary!!!
#include "ode/ode.h"


// Constructors/Destructors
//  

UndoMoveCommand::UndoMoveCommand (
		DrawObject* aDrawObjectPtr, 
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			theDrawObjectPtr(aDrawObjectPtr)
{
	assert(aDrawObjectPtr);
	assert(aBaseObjectPtr);
	theOldPosition = theBaseObjectPtr->getTheCenter();
	
	// TODO: FIXME: ugly - direct model access outside the model class
	theNewPosition.x = dGeomGetPosition(theBaseObjectPtr->getTheGeomID())[0];
	theNewPosition.y = dGeomGetPosition(theBaseObjectPtr->getTheGeomID())[1];
	// TODO: FIXME: rotation not implemented yet
	theNewPosition.angle = 0;

	setText("Move " + theBaseObjectPtr->getName());
}

UndoMoveCommand::~UndoMoveCommand ( ) 
{
	DEBUG5("~UndoMoveCommand() for %p - %s\n", this, text().toAscii().constData());
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
	theBaseObjectPtr->setTheCenter(theNewPosition);
	theBaseObjectPtr->reset();
	theDrawObjectPtr->advance(0);
}

void UndoMoveCommand::undo ()
{
	theBaseObjectPtr->setTheCenter(theOldPosition);
	theBaseObjectPtr->reset();
	theDrawObjectPtr->advance(0);
}
