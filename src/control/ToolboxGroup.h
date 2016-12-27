/* The Butterfly Effect
 * This file copyright (C) 2011,2014,2016 Klaas van Gend
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
#include <QList>
#include <QString>
#include <QVariant>

/// This class holds a series of objects for the Toolbox
class ToolboxGroup
{
public:
    explicit ToolboxGroup(const QString &aGroupName);
    ~ToolboxGroup();

    void addObject(AbstractObjectPtr anObjectPtr);

    int count(void)
    {
        return theObjectsList.count();
    }

    /// @returns a pointer to the last object in this group
    ///          without removing it .
    /// (compare to popObject(), that REMOVES the last one)
    AbstractObjectPtr last() const
    {
        return theObjectsList.last();
    }

    /// Removes the last object from the group.
    /// @note: you already used last() to retrieve its pointer
    AbstractObjectPtr popObject(void);

    QString     theGroupName;    // translatable
    QString     theInternalName; // not translatable

private:
    typedef QList<AbstractObjectPtr> ObjectsList;
    ObjectsList theObjectsList;

    friend class ToolboxItemGroup;
};

#endif // TOOLBOXGROUP_H
