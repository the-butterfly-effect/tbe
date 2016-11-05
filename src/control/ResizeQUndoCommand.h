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

#ifndef RESIZEQUNDOCOMMAND_H
#define RESIZEQUNDOCOMMAND_H

#include "AbstractQUndoCommand.h"
#include "Position.h"

/// Implementation of AbstractQUndoCommand to handle resizing
/// (either horizontal or vertical) of an QQuickItem by the user.
class ResizeQUndoCommand : public AbstractQUndoCommand
{
    Q_OBJECT

public:
    explicit ResizeQUndoCommand(ViewItem* anViewItemPtr,
                                QQuickItem* aHandlePtr,
                                QUndoCommand *parent = 0);

    void commit() override;

    /// Obtains all the latest changes and checks if actually changed.
    /// @returns true if this undo/redo changes one or more properties.
    bool isChanged() override;

    void redo() override;
    void undo() override;

private:
    void slot_updateVars(qreal anXM, qreal aYM, qreal aRotDegrees, qreal aWidthM, qreal aHeightM) override;

private:
    Position theNewPos;
    qreal theNewWidthM;
    qreal theNewHeightM;
};

#endif // RESIZEQUNDOCOMMAND_H
