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

#include <QGraphicsSceneMouseEvent>

#include "MoveUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "AbstractObject.h"


MoveUndoCommand::MoveUndoCommand(
        ViewObjectPtr anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("Move"), nullptr)
{
    DEBUG3ENTRY;
    setDecoratorImage("ProxyMove");
}


void MoveUndoCommand::basicMoveEvent(const QPointF& aPos)
{
    DEBUG4ENTRY;
    theNewPos = theOrigPos.toVector() + Vector(aPos-theButtonDownPosition);
    theNewPos.angle = theOrigPos.angle;
    theViewObjPtr->setNewGeometry(theNewPos);
    if (true==setDecoratorStateMouseMove())
    {
        if (theLastKnownGood.isValid())
        {
            // if we get here, we can leave the object object at its location
            // and only move the decorator around
            theViewObjPtr->setNewGeometry(theLastKnownGood);
            setDecoratorPosition(theNewPos.toVector());
        }
    }
    else
    {
        theLastKnownGood = theNewPos;
        resetDecoratorPosition();
    }
}


void MoveUndoCommand::basicPressEvent(const QPointF& aPos)
{
    DEBUG4ENTRY;
    theButtonDownPosition = aPos;
    // Replace theOrigPos (as set by the constructor) to use the butondown position.
    theOrigPos = Position(aPos, theOrigPos.angle);
    // Essential is *not* to initialize theLastKnowngood here - so it remains invalid.
}


bool MoveUndoCommand::basicReleaseEvent(void)
{
    DEBUG4ENTRY;
    if (theLastKnownGood!=theNewPos)
    {
        if (theLastKnownGood.isValid())
            theNewPos = theLastKnownGood;
    }
    return AbstractUndoCommand::mouseReleaseEvent(nullptr);
}


bool MoveUndoCommand::mouseMoveEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    QPointF myMousePos = anEventPtr->scenePos();
    basicMoveEvent(myMousePos);
    return true;
}

bool MoveUndoCommand::mousePressEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    basicPressEvent(anEventPtr->scenePos());
    return false;
}


bool MoveUndoCommand::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    return basicReleaseEvent();
}
