/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

#ifndef TOOLBOX_H
#define TOOLBOX_H

class ToolboxGroup;
#include "AbstractObjectPtr.h"

//#include <QObject>
#include <QMap>

// This class keeps track of all ToolboxGroup, which each contain
// objects to add to levels.
// It also knows on where and how to update the UI.
class Toolbox
{
public:
    Toolbox();
    ~Toolbox();

    /// @note (used by DeleteUndoCommand only)
    /// Finds the ToolboxGroup that the object belongs to.
    /// @param anAOPtr the object to find a TBG for.
    static ToolboxGroup *findToolBoxGroup(AbstractObjectPtr anAOPtr);

private:
    typedef QMap<QString, ToolboxGroup *> ToolboxGroupList;
    ToolboxGroupList theToolboxList;

    friend class Level;
    friend class MainWindow;
};

#endif // TOOLBOX_H
