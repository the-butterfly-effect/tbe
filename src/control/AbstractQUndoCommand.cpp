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

AbstractQUndoCommand::AbstractQUndoCommand(ViewItem* anViewItemPtr,
                                           QQuickItem* aHandlePtr,
                                           const QString &anUndoText,
                                           QUndoCommand *parent)
    : QUndoCommand(parent),
      theHandlePtr(aHandlePtr),
      theAOPtr(anViewItemPtr->theAOPtr)
{
    // This is the undo action, anUndoText is e.g. “Move %s” and
    // anAbstractObjectPtr->getName() is e.g. “Birch Bar”
    setText(anUndoText.arg(theAOPtr->getName()));
    theOrigPos    = theAOPtr->getOrigCenter();
    theOrigWidth  = theAOPtr->getTheWidth();
    theOrigHeight = theAOPtr->getTheHeight();
}


AbstractQUndoCommand::~AbstractQUndoCommand()
{
    DEBUG1ENTRY;
//TODO:    UndoSingleton::notifyGone(this);
}


void AbstractQUndoCommand::commit(void)
{
    DEBUG1ENTRY;
    UndoSingleton::push(this);
}

ViewItem *AbstractQUndoCommand::getVIPtr()
{
    assert(theAOPtr->theViewItemPtr);
    return theAOPtr->theViewItemPtr;
}

bool AbstractQUndoCommand::isObjectColliding(void)
{
    // TODO/FIXME: implement this one as a property!
    return false;
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
    assert(theAOPtr->theViewItemPtr);
    theAOPtr->theViewItemPtr->adjustObjectDrawingFromAO();
}
