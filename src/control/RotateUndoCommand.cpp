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

#include "RotateUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "AbstractObject.h"
#include <QGraphicsSceneMouseEvent>

RotateUndoCommand::RotateUndoCommand(
        ViewObjectPtr anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("Rotate"), nullptr),
      theButtonDownVectorAngle(999.f)
{
    DEBUG3ENTRY;
    setDecoratorImage("ProxyRotate");
    theNewPos = theOrigPos;
}


bool RotateUndoCommand::editAngleMove(qreal aCurrentAngle)
{
    theNewPos.angle = aCurrentAngle;
    theViewObjPtr->setNewGeometry(theNewPos);
    setDecoratorStateMouseMove();
    return true;
}

bool RotateUndoCommand::editAngleDone(qreal aFinalAngle)
{
    theNewPos.angle = aFinalAngle;
    return AbstractUndoCommand::mouseReleaseEvent(nullptr);
}

bool RotateUndoCommand::mouseMoveEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qreal myOrigAngle = theOrigPos.angle;

    Vector myNewVector = Position(anEventPtr->scenePos())-theOrigPos;

    theNewPos.angle = -theButtonDownVectorAngle+myNewVector.toAngle()
                      + myOrigAngle;

    theViewObjPtr->setNewGeometry(theNewPos);
    setDecoratorStateMouseMove();
    return true;
}

bool RotateUndoCommand::mousePressEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    DEBUG3ENTRY;
    theButtonDownPosition = anEventPtr->scenePos();
    theButtonDownVectorAngle = Vector(theButtonDownPosition - theOrigPos.toQPointF()).toAngle();
    return true;
}

