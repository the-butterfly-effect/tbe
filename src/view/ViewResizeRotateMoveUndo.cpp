/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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

#include "ViewWorldItem.h"
#include "ViewResizeRotateMoveUndo.h"
#include "ViewItem.h"
#include "UndoSingleton.h"
#include "AbstractQUndoCommand.h"

ViewResizeRotateMoveUndo::ViewResizeRotateMoveUndo(QQuickItem *parent)
    : QQuickItem(parent),
      theUndoPtr(nullptr)
{
    assert(nullptr == parent);
    emit isCollidingChanged();
}

ViewResizeRotateMoveUndo::~ViewResizeRotateMoveUndo()
{
    // check if we're being deleted because the parent ends...
    if (nullptr==ViewWorldItem::me())
        return;
    // if not, we're probably up for an undo commit...
    if (theUndoPtr) {
        DEBUG3("ViewResizeRotateMoveUndo::~ViewResizeRotateMoveUndo(), but still unfinished undo present.");
        DEBUG3("  this is common: it likely didn't get completed yet.")
        DEBUG3("  theUndoPtr: %p, name: '%s'.", theUndoPtr, ASCII(theUndoPtr->text()));
        commitChanges();
    }
}

QQuickItem *ViewResizeRotateMoveUndo::activeHandle()
{
    if (nullptr == theUndoPtr)
        return nullptr;
    return theUndoPtr->theHandlePtr;
}

void ViewResizeRotateMoveUndo::addUndo(QObject* anUndoToUse)
{
    DEBUG1ENTRY;

    // do we need to take care of the previous undo first?
    assert(nullptr==theUndoPtr);
    theUndoPtr = qobject_cast<AbstractQUndoCommand*>(anUndoToUse);
    hookup();
}

void ViewResizeRotateMoveUndo::commitChanges()
{
    DEBUG1ENTRY
    disconnect(theUndoPtr, SIGNAL(isCollidingChanged()), this, SIGNAL(isCollidingChanged()));
    if (theUndoPtr->isChanged())
        theUndoPtr->commit();
    else
        delete theUndoPtr;
    theUndoPtr = nullptr;
}


void ViewResizeRotateMoveUndo::hookup()
{
    connect(theUndoPtr, SIGNAL(isCollidingChanged()), this, SIGNAL(isCollidingChanged()));
    connect(theUndoPtr, SIGNAL(isBackInToolboxChanged()), this, SIGNAL(isBackInToolboxChanged()));
    emit theActiveHandleChanged();
    emit isCollidingChanged();
    emit isBackInToolboxChanged();
}


bool ViewResizeRotateMoveUndo::isBackInToolbox()
{
    if (theUndoPtr)
        return theUndoPtr->isBackInToolbox();
    return false;
}

bool ViewResizeRotateMoveUndo::isColliding()
{
    if (theUndoPtr)
        return theUndoPtr->isColliding();
    return true;
}

void ViewResizeRotateMoveUndo::startNewUndo(const QString& aType,
                                            QQuickItem *aHandlePtr)
{
    DEBUG1ENTRY;

    // do we need to take care of the previous undo first?
    if (theUndoPtr)
    {
        commitChanges();
        theUndoPtr = nullptr;
    }
    assert(nullptr != aHandlePtr);
    theUndoPtr = UndoSingleton::createQUndoCommand(theDecoratedPtr, aHandlePtr, aType);
    hookup();
}

qreal ViewResizeRotateMoveUndo::vector2AngleDegrees(qreal dx, qreal dy)
{
    Vector myVector(dx, dy);
    return myVector.toAngle()* 180/PI;
}
