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

#include "ToolboxGroup.h"
#include "ToolboxModelItem.h"
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

QObject *ToolboxModelItem::getToolboxModelItemPtr()
{
    return this;
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
