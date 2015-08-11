/* The Butterfly Effect
 * This file copyright (C) 2011,2014 Klaas van Gend
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
    explicit InsertUndoCommand(ViewObject* anViewObjectPtr, QString anActionString = QObject::tr("Insert"));

    /// This static member creates an InsertUndoCommand from a Toolboxgroup pointer
    /// and handles everything - including the commit().
    /// @param anToolboxGroupPtr    pointer to the toolboxgroup to take the object from
    /// @returns true if successful (which is always)
    static bool createInsertUndoCommand(ToolboxGroup* anToolboxGroupPtr);

    /// This static member creates an InsertUndoCommand from an AbstractObjectPtr
    /// and handles everything - including the commit().
    /// @param anToolboxGroup       pointer to the toolboxgroup to take the object from
    /// @returns true if successful (which is always)
    static bool createInsertUndoCommand(AbstractObjectPtr anAOPtr);

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

protected:
    /// Pointer to the toolboxgroup to take the object from (redo) / push back (undo)
    /// @note: is NULL in the case of a insert from menu in level creator
    ToolboxGroup* theTBGPtr;

    /// TODO: document
    static ViewObject *createVOfromAO(AbstractObjectPtr anAOPtr);

    /// TODO: document
    static InsertUndoCommand* createInsertUndoCommandIntern(
            AbstractObjectPtr anAOPtr);

};

#endif // INSERTUNDOCOMMAND_H
