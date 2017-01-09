/* The Butterfly Effect
 * This file copyright (C) 2017 Klaas van Gend
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

#include "InsertMoveProxy.h"
#include "InsertMoveQUndoCommand.h"
#include "ToolboxModelItem.h"
#include "UndoSingleton.h"
#include "ViewItem.h"
#include "ViewWorldItem.h"

InsertMoveProxy::InsertMoveProxy(QQuickItem *parent)
    : QQuickItem(parent),
      theIMQCPtr(nullptr)
{
    // We don't need to create the InsertMoveQUndoCommand, it will be passed to us.
}

InsertMoveProxy::~InsertMoveProxy()
{
    DEBUG1ENTRY;
    // Check if we're being deleted because the parent ends...
    if (nullptr==ViewWorldItem::me())
        return;
    // If not, we're probably up for a commit (or not).
    if (theIMQCPtr) {
        if (theIMQCPtr->isBackInToolbox()) {
            printf("is back in toolbox\n");
            theIMQCPtr->undo();
            delete theIMQCPtr;
        } else {
            printf("outside toolbox, time to commit\n");
            theIMQCPtr->commit();
        }
        theIMQCPtr = nullptr;
    }
    DEBUG1EXIT;
}

void InsertMoveProxy::createUndo(QObject* aTMIPtr,
                              QQuickItem* aHandlePtr, qreal anXinM, qreal aYinM)
{
    DEBUG1ENTRY;
    assert (nullptr == theIMQCPtr);
    auto myTMIPtr = qobject_cast<ToolboxModelItem*>(aTMIPtr);
    assert (nullptr != myTMIPtr);
    theIMQCPtr = myTMIPtr->createUndo(aHandlePtr, anXinM, aYinM);
    assert (nullptr != theIMQCPtr);
}


void InsertMoveProxy::updateVars(qreal anXM, qreal aYM)
{
    theIMQCPtr->slot_updateVars(anXM, aYM);
}

