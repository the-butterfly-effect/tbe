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
#include "UndoSingleton.h"
#include "ViewItem.h"

#include <chrono>

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
    theOrigPos    = theAOPtr->getOrigCenter();
    theOrigWidth  = theAOPtr->getTheWidth();
    theOrigHeight = theAOPtr->getTheHeight();

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
    bool newCollision = false;

    // TODO: temporary code!!!
    static std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> delta = now - start;
    newCollision = static_cast<long>(delta.count())%2 ? 1 : 0;

    if (newCollision != isObjectColliding)
    {
        printf("isCollidingChanged!\n");
        isObjectColliding = newCollision;
        emit isCollidingChanged();
    }
    return newCollision;
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
    DEBUG3("AbstractQUndoCommand::redo for '%s'", ASCII(text()));
    // nothing to do here :-)
}

void AbstractQUndoCommand::undo(void)
{
    DEBUG3("AbstractQUndoCommand::undo for '%s'", ASCII(text()));
    // nothing to do here :-)
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
