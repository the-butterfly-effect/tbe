/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

#include "InsertMoveQUndoCommand.h"
#include "ToolboxModelItem.h"
#include "ViewItem.h"
#include "UndoSingleton.h"
#include "World.h"

#include <QQmlProperty>
#include <QQuickItem>

InsertMoveQUndoCommand::InsertMoveQUndoCommand(ViewItem* anViewItemPtr,
                                       QQuickItem* aHandlePtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, aHandlePtr,
                           QObject::tr("Insert %1"), parent),
      theTMIPtr(nullptr)
{
}

InsertMoveQUndoCommand::~InsertMoveQUndoCommand()
{
    DEBUG1ENTRY;
}

void InsertMoveQUndoCommand::commit()
{
    if (isBackInToolbox() || isColliding())
        undo();
    else
        AbstractQUndoCommand::commit();
}

bool InsertMoveQUndoCommand::isChanged()
{
    return true;
}

void InsertMoveQUndoCommand::redo()
{
    // REDO:
    // 1) (if VI gone): take AO out of the TIG and create a new VI
    // 2) (if VI exists): do little updating only
    assert(nullptr != theTMIPtr);
    if (nullptr == theAOPtr)
    {
        theAOPtr = theTMIPtr->getAOfromToolbox();
        updateAO(theNewPos);
        theAOPtr->createViewItem();
    }
    World::getWorldPtr()->addObject(theAOPtr);
    AbstractQUndoCommand::redo();
}

void InsertMoveQUndoCommand::setToolboxModelItemPtr(ToolboxModelItem *aPtr)
{
    assert (nullptr == theTMIPtr);
    theTMIPtr = aPtr;
    assert (nullptr != theTMIPtr);
}

void InsertMoveQUndoCommand::slot_updateVars(qreal anXM, qreal aYM, qreal /*aRotDegrees*/, qreal /*aWidthM*/, qreal /*aHeightM*/)
{
    theNewPos = Position(anXM + theAOPtr->getTheWidth()/2, aYM - theAOPtr->getTheHeight()/2, theOrigPos.angle);
    AbstractQUndoCommand::redo();
    getVIPtr()->setOpacity(isColliding() ? 0.3 : 1.0);
}

void InsertMoveQUndoCommand::undo()
{
    if (nullptr == theAOPtr)
            return;
    // UNDO: Take AO from world, return AO to the ToolboxItemGroup, delete VI.
    assert(nullptr != theTMIPtr);
    theAOPtr->deleteViewItem();
    assert(nullptr==getVIPtr());
    // remove from the world and viewworld
    World::getWorldPtr()->removeObject(theAOPtr);
    theTMIPtr->returnAO2Toolbox(theAOPtr);
    theAOPtr = nullptr;

    // Note how we don't call AbstractQUndoCommand::undo()!!!
    // (as there's nothing to undo - we just removed it all)
}

