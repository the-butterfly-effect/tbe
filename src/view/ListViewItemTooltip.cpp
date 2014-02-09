#include "AbstractObject.h"
#include "ImageCache.h"
#include "ListViewItemTooltip.h"
#include "MainWindow.h"
#include "ViewObject.h"
#include "ui_ListViewItemTooltip.h"

ListViewItemTooltip::ListViewItemTooltip(ToolboxGroup *aTBGPtr,
                                         ResizingGraphicsView* aParent) :
    AnimatedDialog(aParent, AnimatedDialog::TOOLTIP),
    ui(new Ui::ListViewItemTooltip),
    theTBGPtr(aTBGPtr)
{
    ui->setupUi(this);

    // set the image, description and help
    AbstractObject* myAOPtr = theTBGPtr->first();
    ViewObject* myVOPtr = myAOPtr->createViewObject();
    ui->labelObjectImage->setPixmap(myVOPtr->pixmap());
    ui->labelName->setText(theTBGPtr->theGroupName.result());

    ui->labelCount->setText(tr("%1x").arg(theTBGPtr->count()));
    ui->labelDescription->setText(myAOPtr->getToolTip());

    // make it appear(animated)
    theYCoord = 50;

}


ListViewItemTooltip::~ListViewItemTooltip()
{

}


void ListViewItemTooltip::on_buttonRemove_clicked()
{
    setDeleteOnDisappear(true);
    disappearAnimated();
}
