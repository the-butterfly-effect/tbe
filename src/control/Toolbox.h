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

#include <QDomElement>
#include <QMap>

class ResizingGraphicsView;
class QListWidget;
class QQuickWidget;

// This class keeps track of all ToolboxGroup, which each contain
// objects to add to levels.
// It also knows on where and how to update the UI.
class Toolbox
{
public:
    explicit Toolbox();
    ~Toolbox();

    /// Create a ToolboxGroup from a QDomNode and add it to the Toolbox.
    /// @param q The DomNode to create the group from.
    /// @note Member only called by Level.
    /// @returns an empty QString if everything went fine or an error if not.
    QString addToolboxGroup(const QDomNode& aNode);

    /// Clear the contents of the Toolbox.
    void clear();

    /// Setup everything again in the Toolbox.
    /// @note probably going away when switch to new Qml toolbox complete.
    void repopulateToolbox(ResizingGraphicsView *aGVPtr);

    /// Serialize all contents of the toolbox to aDomNodeRef
    /// in aDocumentRef.
    /// @note Member only called by Level.
    /// @param aDocumentRef
    /// @param aDomNodeRef
    void serialize(QDomDocument& aDocumentRef,
                   QDomElement& aDomNodeRef);

    void setupOld(QListWidget *aToolboxOldStylePtr);
    void setupQml(QQuickWidget *aToolboxQmlStylePtr);

    /// Finds the ToolboxGroup that the object belongs to.
    /// @note (used by DeleteUndoCommand only)
    /// @param anAOPtr the object to find a TBG for.
    static ToolboxGroup *findToolBoxGroup(AbstractObjectPtr anAOPtr);

private:
    typedef QMap<QString, ToolboxGroup *> ToolboxGroupList;
    ToolboxGroupList theToolboxList;

    QListWidget *theToolboxOldStylePtr;
    QQuickWidget *theToolboxQmlStylePtr;
};

#endif // TOOLBOX_H
