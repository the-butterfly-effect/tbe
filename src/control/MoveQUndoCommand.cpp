/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2016 Klaas van Gend
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

#include "MoveQUndoCommand.h"
#include "ViewItem.h"
#include "UndoSingleton.h"

#include <QQmlProperty>
#include <QQuickItem>

MoveQUndoCommand::MoveQUndoCommand(ViewItem* anViewItemPtr,
                                       QQuickItem* aHandlePtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, aHandlePtr,
                           QObject::tr("Move %1"), parent)
{
}

void MoveQUndoCommand::commit()
{
    if (isBackInToolbox())
        undo();
    else
        AbstractQUndoCommand::commit();
}

bool MoveQUndoCommand::isChanged()
{
    if (areQRealsTheSame(theNewPos.x,theOrigPos.x)
            && areQRealsTheSame(theNewPos.y, theOrigPos.y)) {
        return false;
    }
    return true;
}

void MoveQUndoCommand::slot_updateVars(qreal anXM, qreal aYM, qreal /*aRotDegrees*/, qreal /*aWidthM*/, qreal /*aHeightM*/)
{
    theNewPos = Position(anXM, aYM, theOrigPos.angle);
    redo();
    if (!isColliding())
        theLastKnownGood = theNewPos;
}
