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

#ifndef EDITPROPERTYUNDOCOMMAND_H
#define EDITPROPERTYUNDOCOMMAND_H

#include "AbstractUndoCommand.h"

/// Implementation of EditPropertyUndoCommand, used by the
/// EditObjectDialog
class EditPropertyUndoCommand : public AbstractUndoCommand
{
public:
    explicit EditPropertyUndoCommand(ViewObjectPtr anViewObjectPtr);

    virtual ~EditPropertyUndoCommand();

    /// Mandatory, but we don't care for this one
    /// @note that mouseReleaseEvent is not overridden - it will still call commit().
    bool mouseMoveEvent   (QGraphicsSceneMouseEvent*) override { return false; }

    /// Mandatory, but we don't care for this one
    /// @note that mouseReleaseEvent is not overridden - it will still call commit().
    bool mousePressEvent  (QGraphicsSceneMouseEvent*) override { return false; };
};

#endif // EDITPROPERTYUNDOCOMMAND_H
