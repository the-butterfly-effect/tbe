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

ToolboxListWidgetItem::ToolboxListWidgetItem(
        ResizingGraphicsView *aRSGVPtr,
        ToolboxGroup* aTBGPtr,
        QListWidget *parent) :
    QObject(parent),
    QListWidgetItem(parent, QListWidgetItem::UserType),
    theTBGPtr(aTBGPtr),
    theRSGVPtr(aRSGVPtr)
{
    theTBGPtr->setItemPtr(this);
    AbstractObjectPtr myAOPtr = theTBGPtr->first();
    ViewObject* myVOPtr = myAOPtr->createViewObject();

    QSize myPixmapSize;
    float myObjectAspectRatio = myAOPtr->getTheWidth() / myAOPtr->getTheHeight();
    if (myObjectAspectRatio > 1.0)
    {
        myPixmapSize.setWidth(theIconSize);
        myPixmapSize.setHeight(theIconSize/myObjectAspectRatio);
    }
    else
    {
        myPixmapSize.setWidth(theIconSize*myObjectAspectRatio);
        myPixmapSize.setHeight(theIconSize);
    }
    theRealPixmap = myVOPtr->pixmap().scaled(myPixmapSize);
    slotUpdateCount();
    setText(theTBGPtr->theGroupName.result());
    setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);
    setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    setSizeHint(QSize(theIconSize+10, theRealPixmap.height()+24));

    connect(parent, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotSelected(QListWidgetItem*)));
}


void ToolboxListWidgetItem::slotSelected(QListWidgetItem* who)
{
    // I'd hoped that when we're called, we already knew it would be about us
    // Alas, it isn't so :-(
    if (who != this)
        return;

    if (theTBGPtr->count()!=0)
    {
        ListViewItemTooltip* myNewTooltip =
            new ListViewItemTooltip(theTBGPtr, theRSGVPtr);
        emit myNewTooltip->appearAnimated();
    }
}

void ToolboxListWidgetItem::slotUpdateCount(void)
{
    if (theTBGPtr->count()==0)
    {
        setIcon(QIcon());
        setText("");
    }
    else
    {
        setIcon(theRealPixmap);
        setText(theTBGPtr->theGroupName.result());
    }
}
