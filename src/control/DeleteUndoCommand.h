/* The Butterfly Effect
 * This file copyright (C) 2012 Klaas van Gend
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

#ifndef DELETEUNDOCOMMAND_H
#define DELETEUNDOCOMMAND_H

#include "InsertUndoCommand.h"
#include "Position.h"
#include "ToolboxGroup.h"
#include "ViewObject.h"

/// Implementation of AbstractUndoCommand to handle removal of objects.
/// Removed Objects will be returned to the corresponding ToolBoxGroup.
///
/// Because of the symmetry, derives from InsertUndoCommand
class DeleteUndoCommand : public InsertUndoCommand
{
public:
    explicit DeleteUndoCommand(ViewObject* anViewObjectPtr);

    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    virtual void redo();

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    virtual void undo();
};

#endif // DELETEUNDOCOMMAND_H
