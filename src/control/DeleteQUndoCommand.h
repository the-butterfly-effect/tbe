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

#ifndef DELETEQUNDOCOMMAND_H
#define DELETEQUNDOCOMMAND_H

#include "AbstractQUndoCommand.h"

class ToolboxItemGroup;

/// Implementation of AbstractQUndoCommand to handle deletion of a QQuickItem
/// by the user.
class DeleteQUndoCommand : public AbstractQUndoCommand
{
public:
    explicit DeleteQUndoCommand(ViewItem* anViewItemPtr,
                                QUndoCommand *parent = 0);

    void commit() override;

    /// Delete is always a big change.
    bool isChanged() override
    {
        return true;
    }

    void redo() override;
    void undo() override;

private:
    /// Not needed for DeleteQUndo:
    void slot_updateVars(qreal, qreal, qreal, qreal, qreal) override
    {};

private:
    /// Upon redo, this is the ToolboxItemGroup to return the object to.
    /// (but only if the object came from a toolbox!)
    ToolboxItemGroup* theTIGPtr;
};

#endif // DELETEQUNDOCOMMAND_H
