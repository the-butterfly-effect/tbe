/* The Butterfly Effect
 * This file copyright (C) 2012 Klaas van Gend
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

#include "AbstractObject.h"
#include "DeleteUndoCommand.h"
#include "Level.h"
#include <QGraphicsScene>
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "World.h"


DeleteUndoCommand::DeleteUndoCommand(
        ViewObject* anViewObjectPtr)
    : InsertUndoCommand(anViewObjectPtr, QObject::tr("Remove"))
{
    DEBUG3ENTRY;
    // The list of toolboxes is kept in Level as a QMap.
    // so we need to ask Level...
    theTBGPtr = Level::findToolBoxGroup(theViewObjPtr->getAbstractObjectPtr());
    assert(theTBGPtr!=NULL);
    commit();
}


void DeleteUndoCommand::redo(void)
{
    // opposite of Insert :-)
    InsertUndoCommand::undo();
}


void DeleteUndoCommand::undo(void)
{
    // opposite of Insert :-)
    InsertUndoCommand::redo();
}
