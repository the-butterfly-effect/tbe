/* The Butterfly Effect
 * This file copyright (C) 2017 Klaas van Gend
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

#ifndef INSERTMOVEPROXY_H
#define INSERTMOVEPROXY_H

#include <QQuickItem>

class InsertMoveQUndoCommand;

/// This is an RAII class: instantiated when a user drags an item from
/// toolbox to game area and destroyed whenever the drag is complete. Upon
/// destruction, it will either commit or discard its InsertMoveQUndoCommand.
class InsertMoveProxy : public QQuickItem
{
    Q_OBJECT

public:
    InsertMoveProxy(QQuickItem *parent = Q_NULLPTR);
    ~InsertMoveProxy();

signals:

public slots:

private:
    InsertMoveQUndoCommand* theIMQCPtr;
};

#endif // INSERTMOVEPROXY_H
