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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef RESIZEUNDOCOMMAND_H
#define RESIZEUNDOCOMMAND_H

#include "AbstractObject.h"
#include "AbstractUndoCommand.h"
#include "Position.h"

/// implementation of AbstractUndoCommand to handle
/// resize (horizontal and vertical) of objects by the user
class ResizeUndoCommand : public AbstractUndoCommand
{
public:
    explicit ResizeUndoCommand(ViewObjectPtr anViewObjectPtr);

    virtual bool mouseMoveEvent   (QGraphicsSceneMouseEvent* anEventPtr);
    virtual bool mousePressEvent  (QGraphicsSceneMouseEvent* anEventPtr);

    /// Calculate the local position from a scene position
    /// (but remain in scene units)
    /// @param aScenePos position in scene coords
    /// @returns position in local coords
    ///          (using theOrigPos from the parent class)
    QPointF toLocalPos(const QPointF& aScenePos);

private:
    AbstractObject::SizeDirections theResizingOptions;

    enum Axis
    {
        NONE,
        WIDTHLEFT=1,
        WIDTHRIGHT=2,
        HEIGHTBOTTOM=4,
        HEIGHTTOP=8
    };
    // TODO/FIXME: make this one use enum Axis
    int theAxis;
    qreal theButtonDownLength;

};

#endif // RESIZEUNDOCOMMAND_H
