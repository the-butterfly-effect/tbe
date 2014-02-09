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
