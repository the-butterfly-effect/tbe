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

#include "AbstractQUndoCommand.h"
#include "Position.h"
#include "UndoSingleton.h"
#include "ViewItem.h"

AbstractQUndoCommand::AbstractQUndoCommand(ViewItem* anViewItemPtr,
                                           QQuickItem* aHandlePtr,
                                           const QString &anUndoText,
                                           QUndoCommand *parent)
    : QUndoCommand(parent),
      theHandlePtr(aHandlePtr),
      theAOPtr(anViewItemPtr->theAOPtr),
      isObjectColliding(false)
{
    // This is the undo action, anUndoText is e.g. “Move %s” and
    // anAbstractObjectPtr->getName() is e.g. “Birch Bar”
    setText(anUndoText.arg(theAOPtr->getName()));
    theOrigPos    = theNewPos    = theAOPtr->getOrigCenter();
    theOrigWidth  = theNewWidth  = theAOPtr->getTheWidth();
    theOrigHeight = theNewHeight = theAOPtr->getTheHeight();
    theVIConnection = connect(anViewItemPtr, SIGNAL(signalUpdateVars(qreal,qreal,qreal,qreal,qreal)),
                              this, SLOT(slot_updateVars(qreal,qreal,qreal,qreal,qreal)));
}

AbstractQUndoCommand::~AbstractQUndoCommand()
{
    DEBUG1ENTRY;
    disconnect(theVIConnection);
    //TODO:    UndoSingleton::notifyGone(this);
}

bool AbstractQUndoCommand::checkForCollisions()
{
    bool hasCollision = false;

    // check for collision with walls of scene
    AABB myAABB(theNewPos, theNewWidth, theNewHeight);
    if (myAABB.minX < 0 || myAABB.minY < 0)
        hasCollision = true;

    // TODO: check for collision with other objects
    ;

    // process results and notify everyone
    if (hasCollision != isObjectColliding)
    {
        isObjectColliding = hasCollision;
        printf("isColling is now %d\n", isObjectColliding);
        emit isCollidingChanged();
    }
    return isObjectColliding;
}


void AbstractQUndoCommand::commit(void)
{
    DEBUG1ENTRY;
    disconnect(theVIConnection);
    UndoSingleton::push(this);
}

ViewItem *AbstractQUndoCommand::getVIPtr()
{
    return theAOPtr->theViewItemPtr;
}

void AbstractQUndoCommand::redo(void)
{
    DEBUG5("AbstractQUndoCommand::redo for '%s'", ASCII(text()));
    updateAO(theNewPos);
    updateAO(theNewWidth, theNewHeight);
    checkForCollisions();
    updateVI();
}

void AbstractQUndoCommand::undo(void)
{
    DEBUG5("AbstractQUndoCommand::undo for '%s'", ASCII(text()));
    updateAO(theOrigPos);
    updateAO(theOrigWidth, theOrigHeight);
    updateVI();
}

void AbstractQUndoCommand::updateAO(const Position &aPosition)
{
    theAOPtr->setOrigCenter(aPosition);
}

void AbstractQUndoCommand::updateAO(qreal aWidth, qreal aHeight)
{
    theAOPtr->setTheWidth(aWidth);
    theAOPtr->setTheHeight(aHeight);
}

void AbstractQUndoCommand::updateVI()
{
    if (nullptr != theAOPtr->theViewItemPtr)
	    theAOPtr->theViewItemPtr->adjustObjectDrawingFromAO();
}
