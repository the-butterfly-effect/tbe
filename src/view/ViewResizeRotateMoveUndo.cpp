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

#include "ViewResizeRotateMoveUndo.h"
#include "UndoSingleton.h"
#include "ViewItem.h"

ViewResizeRotateMoveUndo::ViewResizeRotateMoveUndo(QQuickItem *parent)
    : QQuickItem(parent)
//    theRRMUCPtr(nullptr)
{
    assert(nullptr == parent);
    // Once theDecorated is set, we want to know so we can create
    // our UndoCommand and have it grab the initial data.
//    connect(this, SIGNAL(theDecoratedChanged()), this, SLOT(slot_parentChanged()));
}

/*
ViewResizeRotateMoveUndo::~ViewResizeRotateMoveUndo()
{
    assert (nullptr != theRRMUCPtr);

    // *** Is changed? then push to undocommand stack
    if (theRRMUCPtr->isChanged())
        theRRMUCPtr->commit();
    else
        delete theRRMUCPtr;

}


void ViewResizeRotateMoveUndo::slot_parentChanged()
{
    // We need access to the ViewItem: the 'theVI' child of 'theDecorated'.
    ViewItem* myVIPtr = ViewItem::findVIinVO(theDecoratedPtr);
    assert (myVIPtr!=nullptr);
    assert (myVIPtr->theAOPtr != nullptr);
    theRRMUCPtr = dynamic_cast<ResizeRotateMoveUndoCommand*>(UndoSingleton::createUndoCommand(myVIPtr->theAOPtr, UndoSingleton::ACTION_RESIZEROTATE));
}
*/


qreal ViewResizeRotateMoveUndo::vector2AngleDegrees(qreal dx, qreal dy)
{
    Vector myVector(dx, dy);
    return myVector.toAngle()* 180/PI;
}
