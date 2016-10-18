/* The Butterfly Effect
 * This file copyright (C) 2011,2016 Klaas van Gend
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

#include "RotateQUndoCommand.h"
#include "UndoSingleton.h"
#include "AbstractObject.h"

RotateQUndoCommand::RotateQUndoCommand(
    AbstractObjectPtr anAbstractObjectPtr)
    : AbstractQUndoCommand(anAbstractObjectPtr, QObject::tr("Rotate %1"), nullptr)
{
    DEBUG3ENTRY;
}


bool RotateQUndoCommand::editAngleMove(qreal aCurrentAngle)
{
//    theNewPos.angle = aCurrentAngle;
//    theViewObjPtr->setNewGeometry(theNewPos);
//    setDecoratorStateMouseMove();
    return true;
}

bool RotateQUndoCommand::editAngleDone(qreal aFinalAngle)
{
//    theNewPos.angle = aFinalAngle;
//    return AbstractUndoCommand::mouseReleaseEvent(nullptr);
    return true;
}
