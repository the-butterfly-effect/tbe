/* The Butterfly Effect
 * This file copyright (C) 2011, 2012 Klaas van Gend
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

#ifndef ABSTRACTUNDOCOMMAND_H
#define ABSTRACTUNDOCOMMAND_H

#include "Position.h"
#include <QUndoCommand>

// forward declarations
class ViewObject;
#include "ViewObjectActionDectorator.h"
class QGraphicsSceneMouseEvent;

/** this abstract class is the godfather of all undo classes
  * (Delete/Insert/Move/Resize/Rotate)
  */
class AbstractUndoCommand : public QUndoCommand
{
public:
    /// Unlike QUndoCommand, we always require arguments
    /// to the constructor.
    AbstractUndoCommand(ViewObjectPtr anViewObjectPtr,
                        const QString& anUndoName,
                        QUndoCommand *parent = 0);

    virtual ~AbstractUndoCommand();

    /// remove the pointer from AbstractUndoCommand to this class
    void clearDecoratorPointerToMe();

    /// Call this member to cement this action into the undo stack
    /// and redo the last action to make sure the object is correct.
    /// @note Overridden in DummyUndoCommand.
    virtual void commit(void);

    /// @returns true if the ViewObject is in collision with anything else
    bool isViewObjectColliding(void);

    /// TODO/FIXME: Implement
    /// @returns true if event is handled already.
    virtual bool mouseMoveEvent   (QGraphicsSceneMouseEvent* anEventPtr) = 0;
    virtual bool mousePressEvent  (QGraphicsSceneMouseEvent* anEventPtr) = 0;
    virtual bool mouseReleaseEvent(QGraphicsSceneMouseEvent* anEventPtr);

    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    void redo() override;

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    void undo() override;

protected:
    /// This member sets the Proxy image in the ViewObjectActionDecorator.
    /// @param anImageName  theImage to use as Proxy (no path, no extension)
    void setDecoratorImage(const QString& anImageName);

    /// Reset the DecoratorPosition to align with the ViewObject.
    void resetDecoratorPosition();

    /// Put the Decorator on a certain position, unrelated to the ViewObject.
    /// This isused by the Move Undo.
    void setDecoratorPosition(Vector aPosition);

    /// Set the ViewObjectActionDecorator to either proxy+cross or just proxy,
    /// depending on whether the ViewObject is colliding or not.
    /// @returns true if colliding, false if not
    bool setDecoratorStateMouseMove(void);

    /// Set the ViewObjectActionDecorator to either cross or just nothing,
    /// depending on whether the ViewObject is colliding or not.
    /// @returns true if colliding, false if not
    bool setDecoratorStateUndoRedo(void);

protected:
    /// pointer to the view object
    ViewObjectPtr theViewObjPtr;

    QPointF theButtonDownPosition;
    Position theOrigPos;
    Position theNewPos;

    qreal theOrigWidth;
    qreal theNewWidth;
    qreal theOrigHeight;
    qreal theNewHeight;

    /// Set to true in constructor of AbstractUndoCommand.
    /// Set to false in constructor of DummyUndoCommand.
    /// Used in destructor of AbstractUndoCommand.
    bool  handleDecoratorOnDestructionBool;

private:
    QString theProxyImageName;

private:
    /// keep private & unimplemented: no default constructor
    AbstractUndoCommand();
    /// keep private & unimplemented: no copy constructor
    AbstractUndoCommand(AbstractUndoCommand&);
    /// keep private & unimplemented: no assignment operator
    AbstractUndoCommand& operator=(AbstractUndoCommand&);
};

#endif // ABSTRACTUNDOCOMMAND_H
