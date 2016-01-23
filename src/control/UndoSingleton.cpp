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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "UndoSingleton.h"
#include "RotateUndoCommand.h"
#include "ResizeUndoCommand.h"
#include "MoveUndoCommand.h"
#include "InsertUndoCommand.h"
#include "DeleteUndoCommand.h"
#include "ChoosePhoneUndoCommand.h"

static UndoSingleton* theUndoSingletonPtr = nullptr;
static AbstractUndoCommand* theCurrentlyActiveUndoCommand = nullptr;


UndoSingleton::UndoSingleton(void)
{
	// nothing to do
}


void UndoSingleton::clear()
{
    me()->theUndoStack.clear();
}


QAction* UndoSingleton::createRedoAction (QObject* parent,const QString& prefix)
{
	return me()->theUndoStack.createRedoAction(parent, prefix);
}


QAction* UndoSingleton::createUndoAction (QObject* parent,const QString& prefix)
{
	return me()->theUndoStack.createUndoAction(parent, prefix);
}


AbstractUndoCommand*
UndoSingleton::createUndoCommand(ViewObjectPtr anObject,
								 ActionIcon::ActionType anUndoType)
{
	DEBUG3("UndoSingleton::createUndoCommand() for '%d'", anUndoType);

	AbstractUndoCommand* myNewCommand = nullptr;
	// return immediately for UndoActions that do not need further
	// user interactions (i.e. those derived from Dummy).
	switch(anUndoType)
	{
	case ActionIcon::ACTION_INSERT:
		return new InsertUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_MOVE:
		myNewCommand = new MoveUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_ROTATE:
		myNewCommand =  new RotateUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_RESIZE:
		myNewCommand =  new ResizeUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_SETPHONE:
		return new ChoosePhoneUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_DELETE:
		return new DeleteUndoCommand(anObject);
		break;
	case ActionIcon::ACTION_EDITSPECIAL:
		// TODO/FIXME
		break;
	}
	if (theCurrentlyActiveUndoCommand!=nullptr)
		delete theCurrentlyActiveUndoCommand;
	theCurrentlyActiveUndoCommand = myNewCommand;
	return myNewCommand;
}


bool UndoSingleton::isClean()
{
    DEBUG3ENTRY;
    return me()->theUndoStack.isClean();
}


UndoSingleton* UndoSingleton::me(void)
{
	if (theUndoSingletonPtr==nullptr)
		theUndoSingletonPtr = new UndoSingleton();
	return theUndoSingletonPtr;
}


void UndoSingleton::notifyGone(AbstractUndoCommand* anAUCPtr)
{
	if (anAUCPtr == theCurrentlyActiveUndoCommand)
		theCurrentlyActiveUndoCommand = nullptr;
}


void UndoSingleton::push(AbstractUndoCommand* anAUCPtr)
{
	DEBUG3("UndoSingleton::push for '%s'", ASCII(anAUCPtr->text()));
	me()->theUndoStack.push(anAUCPtr);
	notifyGone(anAUCPtr);
}


void UndoSingleton::setClean()
{
    DEBUG3ENTRY;
    me()->theUndoStack.setClean();
}
