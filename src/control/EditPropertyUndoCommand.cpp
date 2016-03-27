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

#include "AbstractObject.h"
#include "AbstractObjectPtr.h"
#include "EditPropertyUndoCommand.h"
#include "Property.h"
#include "ViewObject.h"

EditPropertyUndoCommand::EditPropertyUndoCommand(
        ViewObjectPtr anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("EditProperty"))
{
	DEBUG3ENTRY;
}


EditPropertyUndoCommand::~EditPropertyUndoCommand()
{
    DEBUG1ENTRY;
}


void EditPropertyUndoCommand::changedID(const QString& anOldID, const QString& aNewID)
{
    DEBUG4ENTRY;
    theOldID = anOldID;
    theNewID = aNewID;
}


void EditPropertyUndoCommand::changedProperty(const QString& aKey, const QString& anOldValue, const QString& aNewValue)
{
    DEBUG4ENTRY;
    theKey = aKey;
    theOldValue = anOldValue;
    theNewValue = aNewValue;
}


void EditPropertyUndoCommand::redo(void)
{
    DEBUG4("EditPropertyUndoCommand::redo for '%s'", ASCII(text()));
    AbstractObjectPtr myAOPtr = theViewObjPtr->getAbstractObjectPtr();
    if (!theKey.isNull())
        myAOPtr->theProps.setProperty(theKey, theNewValue);
    else
        myAOPtr->theID = theNewID;

    // our parent also has to do some things...
    AbstractUndoCommand::redo();
}


void EditPropertyUndoCommand::undo(void)
{
    DEBUG4("EditPropertyUndoCommand::undo for '%s'", ASCII(text()));
    AbstractObjectPtr myAOPtr = theViewObjPtr->getAbstractObjectPtr();
    if (!theKey.isNull())
        myAOPtr->theProps.setProperty(theKey, theOldValue);
    else
        myAOPtr->theID = theOldID;

    AbstractUndoCommand::undo();
}
