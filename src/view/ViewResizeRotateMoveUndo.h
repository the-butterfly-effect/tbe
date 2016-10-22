/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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

#ifndef VIEWRESIZEROTATEMOVEUNDO_H
#define VIEWRESIZEROTATEMOVEUNDO_H

#include "ViewItem.h"

class AbstractQUndoCommand;
#include <QQuickItem>

/// An instance of this class gets instantiated every time a ResizeRotateMoveDecorator
/// item gets created. It will be destroyed upon destruction as well.
/// The VRRMU has a QUndoCommand member, that it will populate when instructed.
/// Upon destruction or whenever told, it will push the QUndoCommand.
class ViewResizeRotateMoveUndo : public QQuickItem
{
    Q_OBJECT
public:

    Q_PROPERTY(ViewItem* theDecorated MEMBER theDecoratedPtr NOTIFY theDecoratedChanged)
    Q_PROPERTY(QQuickItem* theActiveHandle READ activeHandle NOTIFY theActiveHandleChanged)


    Q_INVOKABLE qreal vector2AngleDegrees(qreal dx, qreal dy);

    /// Calling this member will start a new undo process and commit the old undo
    /// - if there is one. Technically, this is a factory method.
    Q_INVOKABLE void startNewUndo(const QString& aType, QQuickItem* aHandlePtr);

public:
    explicit ViewResizeRotateMoveUndo(QQuickItem *parent = nullptr);
    virtual ~ViewResizeRotateMoveUndo();

    QQuickItem* activeHandle();

signals:
    void theDecoratedChanged();
    void theActiveHandleChanged();

private:
    /// Take the dimensions/position/angle from the object and pass to our undo.
    /// Then make it commit() itself to the stack (or not, if nothing changed?).
    void commitChanges();

private:
    ViewItem* theDecoratedPtr;

    AbstractQUndoCommand* theUndoPtr;
};

#endif // VIEWRESIZEROTATEMOVEUNDO_H
