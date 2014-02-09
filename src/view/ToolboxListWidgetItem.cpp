/* The Butterfly Effect
 * This file copyright (C) 2014 Klaas van Gend
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
#include "ListViewItemTooltip.h"
#include "ToolboxListWidgetItem.h"
#include "ViewObject.h"

#include <cstdio>

ToolboxListWidgetItem::ToolboxListWidgetItem(
        ResizingGraphicsView *aRSGVPtr,
        ToolboxGroup* aTBGPtr,
        QListWidget *parent) :
    QObject(parent),
    QListWidgetItem(parent, QListWidgetItem::UserType),
    theTBGPtr(aTBGPtr),
    theRSGVPtr(aRSGVPtr)
{
    AbstractObject* myAOPtr = theTBGPtr->first();
    ViewObject* myVOPtr = myAOPtr->createViewObject();

    setIcon(myVOPtr->pixmap());
    setText(theTBGPtr->theGroupName.result());
    setTextAlignment(Qt::AlignHCenter);
    setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(parent, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(slotSelected(QListWidgetItem*)));
}


void ToolboxListWidgetItem::slotSelected(QListWidgetItem*)
{
    // when we're called, we already know it's about us :-)
    printf("void MainWindow::slotToolboxItemselected(%p)\n", this);
    ListViewItemTooltip* myNewTooltip =
            new ListViewItemTooltip(theTBGPtr, theRSGVPtr);
    emit myNewTooltip->appearAnimated();
}
