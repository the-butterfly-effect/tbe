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

#ifndef AbstractQUndoCommand_H
#define AbstractQUndoCommand_H

#include "AbstractObjectPtr.h"
#include "Position.h"
#include <QUndoCommand>

// forward declarations
class QQuickItem;
class ViewItem;

/** this abstract class is the godfather of all undo classes
  * (ChoosePhone/Delete/EditProperty/Insert/Move/Resize/Rotate)
  */
class AbstractQUndoCommand : public QUndoCommand
{
public:
    /// Unlike QUndoCommand, we always require arguments
    /// to the constructor.
    AbstractQUndoCommand(AbstractObjectPtr anAbstractObjectPtr,
                        const QString &anUndoName,
                        QUndoCommand *parent = 0);

    virtual ~AbstractQUndoCommand();

    /// Call this member to cement this action into the undo stack
    /// and redo the last action to make sure the object is correct.
    /// @note Overridden in DummyUndoCommand.
    virtual void commit(void);

    /// @returns true if the ViewObject is in collision with anything else
    bool isObjectColliding(void);

    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    void redo() override;

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    void undo() override;

protected:
    // All positions and sizes are in meters!
    Position theOrigPos;
    Position theNewPos;
    qreal theOrigWidth;
    qreal theNewWidth;
    qreal theOrigHeight;
    qreal theNewHeight;
};

#endif // AbstractQUndoCommand_H
