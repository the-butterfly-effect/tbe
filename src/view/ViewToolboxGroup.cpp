/* The Butterfly Effect
 * This file copyright (C) 2011,2013 Klaas van Gend
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
#include "GameResources.h"
#include "InsertUndoCommand.h"
#include "Popup.h"
#include "Position.h"
#include "ViewToolboxGroup.h"
#include "ViewWorld.h"


ViewToolboxGroup::ViewToolboxGroup(ToolboxGroup* aTBGPtr, GameResources* aGRPtr, QWidget* aParentPtr)
    : QPushButton(aParentPtr),
      theTBGPtr(aTBGPtr), theIcon(aTBGPtr, aGRPtr, this)
{
    Q_ASSERT(aTBGPtr!=NULL);


    updateCount();
    connect (this, SIGNAL(clicked()), this, SLOT(onClicked()));
}


void ViewToolboxGroup::onClicked ( void )
{
    if (ViewWorld::getIsSimRunning()==true)
    {
        Popup::Info(tr("The simulation is not in rest, you cannot insert new things. Please reset the sim first!"));
        return;
    }

    if (theTBGPtr->count() > 0)
    {
        InsertUndoCommand::createInsertUndoCommand(theTBGPtr);
    }
    emit hideMe();
}


void ViewToolboxGroup::updateCount(void)
{
    if (theTBGPtr->count() > 0)
    {
        theIcon.updateCount();
        setIcon(QPixmap::grabWidget(&theIcon));
        setIconSize(theIcon.getSize());
        setText(tr("%1x %2")
                .arg(theTBGPtr->count())
                .arg(theTBGPtr->theGroupName.result()));
    }
    else
    {
        setIcon(QIcon());
        setText(tr("(empty)"));
        setIconSize(QSize(40,20));
    }
}

