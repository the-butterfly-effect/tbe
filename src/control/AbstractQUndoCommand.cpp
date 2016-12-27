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
#include "ViewWorldItem.h"
#include "World.h"

AbstractQUndoCommand::AbstractQUndoCommand(ViewItem* anViewItemPtr,
                                           QQuickItem* aHandlePtr,
                                           const QString &anUndoText,
                                           QUndoCommand *parent)
    : QUndoCommand(parent),
      theHandlePtr(aHandlePtr),
      theAOPtr(anViewItemPtr->theAOPtr),
      isObjectColliding(false),
      isInToolbox(false)
{
    // This is the undo action, anUndoText is e.g. “Move %1” and
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
}

bool AbstractQUndoCommand::checkForCollisions()
{
    bool hasCollision = false;
    bool isOverToolbox = false;

    // check for collision with walls of scene
    AABB myAABB(theNewPos, theNewWidth, theNewHeight);
    if (myAABB.minX < 0 || myAABB.minY < 0)
        hasCollision = true;
    const World* myWPtr = ViewWorldItem::me()->getWorldPtr();
    if (myAABB.maxY > myWPtr->getTheWorldHeight())
        hasCollision = true;
    if (myAABB.maxX > myWPtr->getTheWorldWidth()) {
        isOverToolbox = true;
        hasCollision = true;
    }

    // TODO: check for collision with other objects
    ;

    // process results and notify everyone
    if (hasCollision != isObjectColliding) {
        isObjectColliding = hasCollision;
        emit isCollidingChanged();
    }
    if (isOverToolbox != isInToolbox) {
        isInToolbox = isOverToolbox;
        emit isBackInToolboxChanged();
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
    if (nullptr == theAOPtr)
        return;
    DEBUG3("AbstractQUndoCommand::undo for '%s'", ASCII(text()));
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
