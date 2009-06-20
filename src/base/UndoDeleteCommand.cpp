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

#include "UndoDeleteCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"
#include <QGraphicsScene>
#include "World.h"

// Constructors/Destructors
//  

UndoDeleteCommand::UndoDeleteCommand (
		DrawObject* aDrawObjectPtr, 
		BaseObject* aBaseObjectPtr)
		  : QUndoCommand(), 
			theBaseObjectPtr(aBaseObjectPtr), 
			theDrawObjectPtr(aDrawObjectPtr)
{
	assert(aDrawObjectPtr);
	assert(aBaseObjectPtr);


	setText("Delete " + theBaseObjectPtr->getName());
}

UndoDeleteCommand::~UndoDeleteCommand ( )
{
	DEBUG5("~UndoDeleteCommand() for %p - %s\n", this, text().toAscii().constData());
	// upon deletion of this object, we finally can delete the objects it holds hostage, too
	delete theBaseObjectPtr;
	theBaseObjectPtr = NULL;
	// deleting theBaseObjectPtr should also delete the DrawObject :-)
	theDrawObjectPtr = NULL;
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  
void UndoDeleteCommand::redo ()
{
	// remove the DrawObject from the scene, but keep the pointer
	theDrawObjectPtr->scene()->removeItem(theDrawObjectPtr);

	// remove the BaseObject from the scene
	assert(theBaseObjectPtr->theWorldPtr->removeObject(theBaseObjectPtr));

	// TODO: and also remove the GeomID/BodyID from the WorldID

	// TODO: a delete also implies (if not in Level editor mode) that
	// the object is available in the tool box again
}

void UndoDeleteCommand::undo ()
{
	theBaseObjectPtr->theWorldPtr->addObject(theBaseObjectPtr);

	// TODO: FIXME: An undo of the delete kind also must imply a "reset"
}
