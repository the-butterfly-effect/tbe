/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#ifndef MOVEUNDOCOMMAND_H
#define MOVEUNDOCOMMAND_H

#include "AbstractUndoCommand.h"
#include "Position.h"

/// Implementation of AbstractUndoCommand to handle
/// movements of objects by the user.
class MoveUndoCommand : public AbstractUndoCommand
{
public:
    explicit MoveUndoCommand(ViewObjectPtr anViewObjectPtr);

    bool mouseMoveEvent   (QGraphicsSceneMouseEvent* anEventPtr) override;
    bool mousePressEvent  (QGraphicsSceneMouseEvent* anEventPtr) override;
    bool mouseReleaseEvent(QGraphicsSceneMouseEvent* anEventPtr) override;

    /// only to be called by EditObjectDialog and internals
    void basicMoveEvent   (const QPointF& aPos);
    /// only to be called by EditObjectDialog and internals
    void basicPressEvent  (const QPointF& aPos);
    /// only to be called by EditObjectDialog and internals
    bool basicReleaseEvent(void);

private:
    Position theLastKnownGood;
};

#endif // MOVEUNDOCOMMAND_H
