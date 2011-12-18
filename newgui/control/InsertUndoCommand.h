/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef INSERTUNDOCOMMAND_H
#define INSERTUNDOCOMMAND_H

#include "AbstractUndoCommand.h"
#include "Position.h"
#include "ToolboxGroup.h"
#include "ViewObject.h"

/// implementation of AbstractUndoCommand to handle
/// insertion of objects
class InsertUndoCommand : public AbstractUndoCommand
{
public:
    explicit InsertUndoCommand(ViewObject* anViewObjectPtr);

    /// This static member creates an InsertUndoCommand from an AbstractObjectPtr
    /// and handles everything - including the commit().
    /// @param anToolboxGroup       pointer to the toolboxgroup to take the object from
    /// @returns true if successful (which is always)
    static bool createInsertUndoCommand(ToolboxGroup* anToolboxGroup);


    /// mandatory, but we don't care - not used
    virtual bool mouseMoveEvent   (QGraphicsSceneMouseEvent*) { return false; }
    /// mandatory, but we don't care - not used
    virtual bool mousePressEvent  (QGraphicsSceneMouseEvent*) { return false; }


    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    virtual void redo();

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    virtual void undo();

private:
    /// Pointer to the toolboxgroup to take the object from (redo) / push back (undo)
    ToolboxGroup* theTBGPtr;

    /// TODO: document
    static ViewObject *createAOandVO(ToolboxGroup *anToolboxGroupPtr);
};

#endif // MOVEUNDOCOMMAND_H
