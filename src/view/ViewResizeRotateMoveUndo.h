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

#include "ViewResizeRotateMoveUndo.h"
#include <QQuickItem>

/// An instance of this class gets instantiated every time a ResizeRotateMoveDecorator
/// item gets created. It will be destroyed upon destruction as well.
/// The VRRMU has a QUndoCommand member, that it will populate. Upon destruction, it
/// will make the decision whether to push the QUndoCommand or just destroy it
/// when nothing really changed.
class ViewResizeRotateMoveUndo : public QQuickItem
{
    Q_OBJECT
public:

    Q_PROPERTY(QQuickItem* theDecorated MEMBER theDecoratedPtr NOTIFY theDecoratedChanged)

    Q_INVOKABLE qreal vector2AngleDegrees(qreal dx, qreal dy);

public:
    explicit ViewResizeRotateMoveUndo(QQuickItem *parent = nullptr);
    virtual ~ViewResizeRotateMoveUndo() {};

signals:
    void theDecoratedChanged();

public slots:
//    void slot_parentChanged();

private:
    //ResizeRotateMoveUndoCommand *theRRMUCPtr;
    QQuickItem* theDecoratedPtr;
};

#endif // VIEWRESIZEROTATEMOVEUNDO_H
