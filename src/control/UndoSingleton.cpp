/* The Butterfly Effect
 * This file copyright (C) 2011,2016 Klaas van Gend
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
//#include "EditPropertyUndoCommand.h"

#include "DeleteQUndoCommand.h"
#include "InsertMoveQUndoCommand.h"
#include "MoveQUndoCommand.h"
#include "ResizeQUndoCommand.h"
#include "ReturnToToolboxQUndoCommand.h"
#include "RotateQUndoCommand.h"

static UndoSingleton *theUndoSingletonPtr = nullptr;


UndoSingleton::UndoSingleton(void)
{
    // nothing to do
}


void UndoSingleton::clear()
{
    me()->theUndoStack.clear();
}


QAction *UndoSingleton::createRedoAction (QObject *parent, const QString &prefix)
{
    return me()->theUndoStack.createRedoAction(parent, prefix);
}


QAction *UndoSingleton::createUndoAction (QObject *parent, const QString &prefix)
{
    return me()->theUndoStack.createUndoAction(parent, prefix);
}

AbstractQUndoCommand *UndoSingleton::createQUndoCommand(ViewItem *aViewItemPtr, QQuickItem *aHandlePtr, const QString &anUndoType)
{
    DEBUG1("UndoSingleton::createQUndoCommand(%p,%p, %s)", aViewItemPtr, aHandlePtr, ASCII(anUndoType));
    // TODO: factory for all types (duh)
    if (anUndoType == "HandleDelete")
        return new DeleteQUndoCommand(aViewItemPtr);
    if (anUndoType == "HandleResize")
        return new ResizeQUndoCommand(aViewItemPtr, aHandlePtr);
    if (anUndoType == "HandleRotate")
        return new RotateQUndoCommand(aViewItemPtr, aHandlePtr);
    if (anUndoType == "ReturnToToolbox")
        return new ReturnToToolboxQUndoCommand(aViewItemPtr);
    if (anUndoType == "Move")
        return new MoveQUndoCommand(aViewItemPtr, aHandlePtr);
    if (anUndoType == "ToolboxInsert")
        return new InsertMoveQUndoCommand(aViewItemPtr, aHandlePtr);
    return nullptr;
}


bool UndoSingleton::isClean()
{
    DEBUG3ENTRY;
    return me()->theUndoStack.isClean();
}


UndoSingleton *UndoSingleton::me(void)
{
    if (theUndoSingletonPtr == nullptr)
        theUndoSingletonPtr = new UndoSingleton();
    return theUndoSingletonPtr;
}


void UndoSingleton::push(QUndoCommand *anUndoPtr)
{
    DEBUG3("UndoSingleton::push for '%s'", ASCII(anUndoPtr->text()));
    me()->theUndoStack.push(anUndoPtr);
}


void UndoSingleton::setClean()
{
    DEBUG3ENTRY;
    me()->theUndoStack.setClean();
}
