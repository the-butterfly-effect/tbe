/* The Butterfly Effect
 * This file copyright (C) 2012,2016 Klaas van Gend
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

#include "DeleteQUndoCommand.h"
#include "ToolboxModelItem.h"
#include "UndoSingleton.h"
#include "ViewItem.h"
#include "World.h"

#include <QQmlProperty>
#include <QQuickItem>

DeleteQUndoCommand::DeleteQUndoCommand(ViewItem* anViewItemPtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, nullptr,
                           QObject::tr("Delete %1"), parent),
      theTMIPtr(nullptr)
{
}

void DeleteQUndoCommand::commit()
{
    AbstractQUndoCommand::commit();
}

void DeleteQUndoCommand::redo()
{
    theAOPtr->deleteViewItem();
    assert(nullptr==getVIPtr());
    // remove from the world and viewworld
    bool myResult = World::getWorldPtr()->removeObject(theAOPtr);
    Q_ASSERT(myResult == true);
    /* and remove compiler warning: */ (void)myResult;

    // let's find which ToolboxModelItem to return to...
    if (false) {
        theTMIPtr->returnAO2Toolbox(theAOPtr);
        theAOPtr = nullptr;
    }

    AbstractQUndoCommand::redo();
}

void DeleteQUndoCommand::undo()
{
    // 1) (if TIG pointer set): take AO out of the TIG and create a new VI
    // 2) (if not set): re-use the AO (we should have kept the pointer alive) and create a new VI
    if(nullptr != theTMIPtr) {
        assert(nullptr == theAOPtr);
        theAOPtr = theTMIPtr->getAOfromToolbox();
        updateAO(theOrigPos);
        theAOPtr->createViewItem();
    }
    else
        assert(nullptr != theAOPtr);

    updateAO(theOrigPos);
    updateAO(theOrigWidth, theOrigHeight);
    World::getWorldPtr()->addObject(theAOPtr);
    updateVI();
    AbstractQUndoCommand::undo();
}

