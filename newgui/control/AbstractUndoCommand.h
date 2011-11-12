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

#ifndef ABSTRACTUNDOCOMMAND_H
#define ABSTRACTUNDOCOMMAND_H

#include "Position.h"
#include <QUndoCommand>

// forward declarations
class ViewObject;
class ViewObjectActionDecorator;
class QGraphicsSceneMouseEvent;

/** this abstract class is the godfather of all undo classes
  * (Delete/Insert/Move/Resize/Rotate)
  */
class AbstractUndoCommand : public QUndoCommand
{
public:
    /// Unlike QUndoCommand, we always require arguments
    /// to the constructor.
    AbstractUndoCommand(ViewObject* anViewObjectPtr,
                        const QString& anUndoName,
                        QUndoCommand *parent = 0);

    /// call this member to cement this action into the undo stack
    /// and redo the last action to make sure the object is correct
    void commit();

    /// TODO/FIXME: Implement
    /// @returns true if event is handled already.
    virtual bool mouseMoveEvent   (QGraphicsSceneMouseEvent* anEventPtr) = 0;
    virtual bool mousePressEvent  (QGraphicsSceneMouseEvent* anEventPtr) = 0;
    virtual bool mouseReleaseEvent(QGraphicsSceneMouseEvent* anEventPtr) = 0;

    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    virtual void redo();

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    virtual void undo();

protected:
    /// pointer to the view object
    ViewObject* theViewObjPtr;

    /// pointer to ObjectActionDecorator for this action on theVOPtr;
    ViewObjectActionDecorator* theVOADPtr;

    /// TODO: explain
    void setupProxyImage(const QString& anImageName);

protected:
    Position theButtonDownPosition;
    Position theOrigPos;
    Position theNewPos;

private:
    /// keep private & unimplemented: no default constructor
    AbstractUndoCommand();
    /// keep private & unimplemented: no copy constructor
    AbstractUndoCommand(AbstractUndoCommand&);
    /// keep private & unimplemented: no assignment operator
    AbstractUndoCommand& operator=(AbstractUndoCommand&);
};

#endif // ABSTRACTUNDOCOMMAND_H
