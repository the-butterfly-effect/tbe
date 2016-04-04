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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObject.h"
#include "ImageCache.h"
#include "ListViewItemTooltip.h"
#include "ToolboxListWidgetItem.h"
#include "Translator.h"
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
    AbstractObjectPtr myAOPtr = theTBGPtr->last();
    ViewObjectPtr myVOPtr = myAOPtr->createViewObject();

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
	ImageCache::getPixmap(myVOPtr->getBaseImageName(), myPixmapSize, &theRealPixmap);
	slotUpdateCount();
	setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    setSizeHint(QSize(90,150));

    connect(parent, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotSelected(QListWidgetItem*)));
}


void ToolboxListWidgetItem::slotSelected(QListWidgetItem* who)
{
    // I'd hoped that when we're called, we already knew it would be about us
    // Alas, it isn't so :-(
    if (who != this)
        return;

    QRect myBoundingRectInViewPort = listWidget()->visualItemRect(this);

    if (theTBGPtr->count()!=0)
    {
        ListViewItemTooltip* myNewTooltip =
            new ListViewItemTooltip(theTBGPtr, myBoundingRectInViewPort.top(), theRSGVPtr);
        emit myNewTooltip->appearAnimated();
    }
}

void ToolboxListWidgetItem::slotUpdateCount(void)
{
    if (theTBGPtr->count()==0)
    {
        QPixmap myEmptyPixmap = QPixmap(theIconSize, theIconSize);
        myEmptyPixmap.fill();
        setIcon(myEmptyPixmap);
        setText(tr("(empty)"));
    }
    else
    {
        setIcon(theRealPixmap);
        //: %1 is the number of items, %2 is the name of the item
        setText( tr("%1x %2").arg(theTBGPtr->count()).arg(TheGetText(theTBGPtr->theGroupName)));
    }
    listWidget()->update();
}
