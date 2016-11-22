/* The Butterfly Effect
 * This file copyright (C) 2011,2016 Klaas van Gend
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

#include "SetPhoneQUndoCommand.h"
#include "ViewItem.h"
#include "UndoSingleton.h"
#include "RequestDialog.h"
#include "TriggerExplosion.h"

#include <QQmlProperty>
#include <QQuickItem>

SetPhoneQUndoCommand::SetPhoneQUndoCommand(ViewItem* anViewItemPtr,
                                       QQuickItem* aHandlePtr,
                                       QUndoCommand *parent)
    : AbstractQUndoCommand(anViewItemPtr, aHandlePtr, QObject::tr("Set phone number"), parent)
{
    theDBPtr = dynamic_cast<DetonatorBox*>(theAOPtr.get());
    assert (nullptr != theDBPtr);
    oldPhoneNumber = theDBPtr->getCurrentPhoneNumber();
    QStringList myList = theDBPtr->getAllPhoneNumbers();

    // TODO: actually create the ChoosePhoneDialog.
    // The interface was coded in RequestDialog, but this class cannot obtain a pointer :-(
    // I need to figure this out - e.g. GameFlow has access...
}

void SetPhoneQUndoCommand::commit()
{
    AbstractQUndoCommand::commit();
}

bool SetPhoneQUndoCommand::isChanged()
{
    return oldPhoneNumber != newPhoneNumber;
}

void SetPhoneQUndoCommand::redo()
{
    assert(nullptr != theDBPtr);
    theDBPtr->setPhoneNumber(newPhoneNumber);
}

void SetPhoneQUndoCommand::slot_updateVars(qreal /*anXM*/, qreal /*aYM*/, qreal /*aRotDegrees*/, qreal /*aWidthM*/, qreal /*aHeightM*/)
{
    // nothing to do here...
}

void SetPhoneQUndoCommand::undo()
{
    assert(nullptr != theDBPtr);
    theDBPtr->setPhoneNumber(oldPhoneNumber);
}
