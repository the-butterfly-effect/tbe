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
      theTIGPtr(nullptr)
{
}

void InsertMoveQUndoCommand::commit()
{
    if (isBackInToolbox())
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
    //AbstractQUndoCommand::redo();
}

void InsertMoveQUndoCommand::slot_updateVars(qreal anXM, qreal aYM, qreal /*aRotDegrees*/, qreal /*aWidthM*/, qreal /*aHeightM*/)
{
    theNewPos = Position(anXM, aYM, theOrigPos.angle);
//    redo();
}

void InsertMoveQUndoCommand::undo()
{
    // Note how we don't call AbstractQUndoCommand::undo()!!!
    // (as there's nothing to undo - we just removed it all)
}

