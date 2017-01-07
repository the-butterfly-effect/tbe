/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2016,2017 Klaas van Gend
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

#ifndef INSERTInsertMoveQUndoCommand_H
#define INSERTInsertMoveQUndoCommand_H

#include "AbstractQUndoCommand.h"
#include "Position.h"

class ToolboxModelItem;

/// Implementation of AbstractQUndoCommand to handle insertion of an object
/// by the user.
/// Note that the user is immediately moving the object around, that's why
/// this class is called InsertMoveQUndoCommand ;-)
class InsertMoveQUndoCommand : public AbstractQUndoCommand
{
    Q_OBJECT

public:
    explicit InsertMoveQUndoCommand(ViewItem* anViewItemPtr,
                                QQuickItem* aHandlePtr,
                                QUndoCommand *parent = 0);

    void commit() override;

    bool isChanged() override;

    void redo() override;

    void undo() override;

private:
    void slot_updateVars(qreal anXM, qreal aYM, qreal aRotDegrees, qreal aWidthM, qreal aHeightM) override;

private:
    /// Upon undo, this is the ToolboxItemGroup to return the object to.
    ToolboxModelItem* theTIGPtr;
};

#endif // INSERTInsertMoveQUndoCommand_H
