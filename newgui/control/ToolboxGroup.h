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

    // TODO: for later
//    AbstractObject* getObject();
//    void returnObject(AbstractObject*);

    // TODO: for later
//    ViewToolGroup* createViewToolGroup(void);

private:
    typedef QList<AbstractObject*> ObjectsList;
    ObjectsList theObjectsList;

    LocalString theGroupName;
};

#endif // TOOLBOXGROUP_H
