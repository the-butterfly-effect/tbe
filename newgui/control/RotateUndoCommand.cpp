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

#include "RotateUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "AbstractObject.h"
#include <QGraphicsSceneMouseEvent>

RotateUndoCommand::RotateUndoCommand(
        ViewObject* anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("Rotate"), NULL)
{
    qDebug() << Q_FUNC_INFO;
    setupProxyImage("../images/ProxyRotate.svg");
    theNewPos = theOrigPos;
}


bool RotateUndoCommand::mouseMoveEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qreal myOrigAngle = theOrigPos.angle;

    Vector myNewVector = Position(anEventPtr->scenePos())-theOrigPos;

    theNewPos.angle = theButtonDownVectorAngle-myNewVector.toAngle()
                      + myOrigAngle;

    theViewObjPtr->setRotation(theNewPos.angleInDegrees());

    return true;
}

bool RotateUndoCommand::mousePressEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qDebug() << Q_FUNC_INFO;
    theButtonDownPosition = anEventPtr->scenePos();
    theButtonDownVectorAngle = Vector(theButtonDownPosition - theOrigPos).toAngle();
    return true;
}

bool RotateUndoCommand::mouseReleaseEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qDebug() << Q_FUNC_INFO;
    // first, make sure the on-screen position is up-to-date
    mousePressEvent(anEventPtr);

    // now, it's time to finalize everything
    // and push the Undo on the stack
    deleteProxyImage();
    UndoSingleton::push(this);

    // we've completely handled the event, we're done
    return true;
}
