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

#include "ResizeQUndoCommand.h"
#include "ViewItem.h"
#include "UndoSingleton.h"

#include <QQmlProperty>
#include <QQuickItem>

ResizeQUndoCommand::ResizeQUndoCommand(ViewItem* anViewItemPtr,
                                       QQuickItem* aHandlePtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, aHandlePtr,
                           QObject::tr("Resize %1"), parent)
{
    theVIConnection = connect(anViewItemPtr, SIGNAL(signalUpdateVars(qreal,qreal,qreal,qreal,qreal)),
                              this, SLOT(slot_updateVars(qreal,qreal,qreal,qreal,qreal)));

}

void ResizeQUndoCommand::commit()
{
    disconnect(theVIConnection);
    AbstractQUndoCommand::commit();
}

bool ResizeQUndoCommand::isChanged()
{
    if (areQRealsTheSame(theOrigWidth,theNewWidthM)
            && areQRealsTheSame(theOrigHeight,theNewHeightM)) {
        return false;
    }
    return true;
}

void ResizeQUndoCommand::redo()
{
    updateAO(theNewPos);
    updateAO(theNewWidthM, theNewHeightM);
    updateVI();
    AbstractQUndoCommand::redo();
}

void ResizeQUndoCommand::slot_updateVars(qreal anXM, qreal aYM, qreal /*aRotDegrees*/, qreal aWidthM, qreal aHeightM)
{
    theNewPos = Position(anXM, aYM, theOrigPos.angle);
    theNewWidthM = aWidthM;
    theNewHeightM = aHeightM;
    redo();
}

void ResizeQUndoCommand::undo()
{
    updateAO(theOrigPos);
    updateAO(theOrigWidth, theOrigHeight);
    updateVI();
    AbstractQUndoCommand::undo();
}

