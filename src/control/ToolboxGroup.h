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

#ifndef TOOLBOXGROUP_H
#define TOOLBOXGROUP_H

#include "AbstractObject.h"
#include "LocalString.h"
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
class ViewToolboxGroup;
class ToolboxListWidgetItem;

/// This class holds a series of objects for the Toolbox
class ToolboxGroup
{
public:
    explicit ToolboxGroup(const LocalString& aGroupName);
    ~ToolboxGroup();

    void addObject(AbstractObjectPtr anObjectPtr);

    int count(void)
    { return theObjectsList.count(); }

    /// @returns a pointer to the first object in this group
    ///          without removing it .
    /// (compare to getObject(), that returns+removes the LAST one)
    AbstractObjectPtr first() const
    { return theObjectsList.first(); }

    /// @returns a pointer to one of the objects that was in the group
    ///          the object is removed from the group.
    /// (compare to first(), which just returns a pointer to the FIRST one)
    AbstractObjectPtr getObject(void);

    void setItemPtr(ToolboxListWidgetItem* aWidgetItemPtr);

    LocalString theGroupName;
    QString     theInternalName;

private:
    typedef QList<AbstractObjectPtr> ObjectsList;
    ObjectsList theObjectsList;
    ToolboxListWidgetItem* theWidgetItemPtr;
};

#endif // TOOLBOXGROUP_H
