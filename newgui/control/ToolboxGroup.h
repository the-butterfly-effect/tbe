/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef TOOLBOXGROUP_H
#define TOOLBOXGROUP_H

class AbstractObject;
#include "LocalString.h"
#include <QList>
#include <QString>

/// This class holds a series of objects for the Toolbox
class ToolboxGroup
{
public:
    explicit ToolboxGroup(const LocalString& aGroupName);
    ~ToolboxGroup();

    void addObject(AbstractObject* anObjectPtr);

    int count(void)
    { return theObjectsList.count(); }

    /// @returns a pointer to the first object in this group
    ///          without removing it .
    /// (compare to getObject(), that returns+removes the LAST one)
    AbstractObject* first() const
    { return theObjectsList.first(); }

    /// @returns a pointer to one of the objects that was in the group
    ///          the object is removed from the group.
    /// (compare to first(), which just returns a pointer to the FIRST one)
    AbstractObject* getObject(void);

//    void returnObject(AbstractObject*);

    LocalString theGroupName;


private:
    typedef QList<AbstractObject*> ObjectsList;
    ObjectsList theObjectsList;

};

#endif // TOOLBOXGROUP_H
