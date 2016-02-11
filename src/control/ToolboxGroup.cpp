/* The Butterfly Effect
 * This file copyright (C) 2011,2014 Klaas van Gend
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

#include "AbstractObject.h"
#include "ToolboxGroup.h"
#include "ToolboxListWidgetItem.h"

ToolboxGroup::ToolboxGroup(const QString &aGroupName)
    : theGroupName(aGroupName),
      theWidgetItemPtr(nullptr)
{
    printf("ToolboxGroup for '%s'\n", ASCII(aGroupName));
    // nothing to do
}


ToolboxGroup::~ToolboxGroup()
{
    // as theObjectsList contains smart pointers,
    // no need to do explicit destruction anymore
}




void ToolboxGroup::addObject(AbstractObjectPtr anObjectPtr)
{
    Q_ASSERT(anObjectPtr!=nullptr);
    theObjectsList.push_back(anObjectPtr);
    theInternalName = anObjectPtr->getInternalName();

    if (theWidgetItemPtr)
        emit theWidgetItemPtr->slotUpdateCount();
}


AbstractObjectPtr ToolboxGroup::popObject(void)
{
    Q_ASSERT(theObjectsList.count() > 0);
    AbstractObjectPtr myAOPtr = theObjectsList.takeLast();
    if (theWidgetItemPtr)
        emit theWidgetItemPtr->slotUpdateCount();
    return myAOPtr;
}


void ToolboxGroup::setItemPtr(ToolboxListWidgetItem* aWidgetItemPtr)
{
    theWidgetItemPtr = aWidgetItemPtr;
}
