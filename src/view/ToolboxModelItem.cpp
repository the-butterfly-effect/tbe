/* The Butterfly Effect
 * This file copyright (C) 2016,2017 Klaas van Gend
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

#include "InsertMoveQUndoCommand.h"
#include "ToolboxGroup.h"
#include "ToolboxModelItem.h"
#include "UndoSingleton.h"
#include "ViewItem.h"

#include <QQuickItem>

ToolboxModelItem::ToolboxModelItem(QObject *parent) : QObject(parent)
{
assert(false);
}

ToolboxModelItem::ToolboxModelItem(const QString &aName, int aCount, qreal aWidth,
                                   qreal aHeight, const QString& anIconName,
                                   const QString &aTooltip, ToolboxGroup *aTBGPtr)
    : QObject(nullptr), theName(aName), theCount (aCount), theWidth(aWidth),
      theHeight(aHeight), theIconName(anIconName), theTooltipText(aTooltip), theTBGPtr(aTBGPtr)
{
}

QObject *ToolboxModelItem::createUndo(QQuickItem *aHandlePtr, qreal anXinM, qreal aYinM)
{
    // At this point, we know what object to insert as we will have an AO
    // and a position on the screen to push it to.
    AbstractObjectPtr myAOPtr = getAOfromToolbox();
    // TODO: figure out rotation
    myAOPtr->setOrigCenter(Position(anXinM+myAOPtr->getTheWidth()/2.,
                                    aYinM+myAOPtr->getTheHeight()/2., 0.));
    ViewItem* myVIPtr = myAOPtr->createViewItem();

    AbstractQUndoCommand* myQUndoPtr = UndoSingleton::createQUndoCommand(myVIPtr, aHandlePtr, "ToolboxInsert");
    InsertMoveQUndoCommand* myIMQUCPtr = dynamic_cast<InsertMoveQUndoCommand*>(myQUndoPtr);
    assert (nullptr != myIMQUCPtr);
//    myIMQUCPtr->setToolboxItemGroupPtr(this);
    return myIMQUCPtr;
}

AbstractObjectPtr ToolboxModelItem::getAOfromToolbox()
{
    setCount(theTBGPtr->count()-1);
    return theTBGPtr->popObject();
}

void ToolboxModelItem::returnAO2Toolbox(AbstractObjectPtr anAOPtr)
{
    theTBGPtr->addObject(anAOPtr);
    setCount(theTBGPtr->count());
}

void ToolboxModelItem::setCount(int aNewCount)
{
    theCount = aNewCount;
    emit countChanged();
}
