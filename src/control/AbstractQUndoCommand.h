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
class AbstractQUndoCommand : public QObject, public QUndoCommand
{
    Q_OBJECT

public:
    /// Unlike QUndoCommand, we always require arguments to the constructor.
    /// @param anViewItemPtr   Pointer to the ViewItem. Is only used once,
    ///                        afterwards we'll always retrieve the ptr through
    ///                        the AbstractObject.
    AbstractQUndoCommand(ViewItem* anViewItemPtr,
                         QQuickItem* aHandlePtr,
                         const QString &anUndoText,
                        QUndoCommand *parent = 0);

    virtual ~AbstractQUndoCommand();

    Q_PROPERTY(bool isColliding READ isColliding NOTIFY isCollidingChanged)

    /// Check for collisions and emit signal isCollidingChanged() if necessary.
    /// @returns true if the ViewObject is in collision with anything else.
    virtual bool checkForCollisions();

    /// Call this member to cement this action into the undo stack
    /// and redo the last action to make sure the object is correct.
    virtual void commit();

    /// Obtains all the latest changes and checks if actually changed.
    /// @returns true if this undo/redo changes one or more properties.
    virtual bool isChanged() = 0;

    /// @returns true if the ViewObject is in collision with anything else.
    virtual bool isColliding()
    {   return isObjectColliding; }


    /// Called by the Undo stack after the action of this
    /// class instance (Move/Rotate/Insert/Delete/Resize)
    /// is firstly done OR when it needs to be redone.
    void redo() override;

    /// Called by the Undo stack to undo the action specified in
    /// this command.
    void undo() override;

private slots:
    /// pure virtual slot
    virtual void slot_updateVars(qreal anXM, qreal aYM, qreal aRotDegrees, qreal aWidthM, qreal aHeightM) = 0;

signals:
     void isCollidingChanged();

protected:
    // Is friends to be able to access theHandlePtr.
    friend class ViewResizeRotateMoveUndo;
    /// Pointer to QML Handle that the user is currently moving around.
    QQuickItem* theHandlePtr;

    AbstractObjectPtr theAOPtr;

    // All positions and sizes are in meters!
    Position theOrigPos;
    qreal theOrigWidth;
    qreal theOrigHeight;

    bool isObjectColliding;

    /// Contains the connection to the ViewItem.
    QMetaObject::Connection theVIConnection;

    void updateAO(const Position& aPosition);
    void updateAO(qreal aWidth, qreal aHeight);
    ViewItem* getVIPtr();
    void updateVI();
};

#endif // AbstractQUndoCommand_H
