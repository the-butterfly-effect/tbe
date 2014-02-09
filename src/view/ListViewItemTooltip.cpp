#include "ListViewItemTooltip.h"
#include "ImageCache.h"
#include "MainWindow.h"
#include "ListViewItemTooltip.h"
#include "ui_ListViewItemTooltip.h"

ListViewItemTooltip::ListViewItemTooltip(ResizingGraphicsView* aParent) :
    AnimatedDialog(aParent, AnimatedDialog::TOOLTIP),
    ui(new Ui::ListViewItemTooltip)
{
    ui->setupUi(this);

    // set the image, description and help

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
