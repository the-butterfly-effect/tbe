#include "AbstractObject.h"
#include "ListViewItemTooltip.h"
#include "Position.h"
#include "ViewObject.h"
#include "ui_ListViewItemTooltip.h"

ListViewItemTooltip::ListViewItemTooltip(ToolboxGroup *aTBGPtr,
                                         ResizingGraphicsView* aParent) :
    AnimatedDialog(aParent, AnimatedDialog::TOOLTIP),
    ui(new Ui::ListViewItemTooltip),
    theTBGPtr(aTBGPtr)
{
    ui->setupUi(this);

    // set the description and help
    ui->labelName->setText(theTBGPtr->theGroupName.result());
    ui->labelCount->setText(tr("%1x").arg(theTBGPtr->count()));
    AbstractObject* myAOPtr = theTBGPtr->first();
    ui->labelDescription->setText(myAOPtr->getToolTip());

    // set the image
    ViewObject* myVOPtr = myAOPtr->createViewObject();
    QPixmap myPixmap = myVOPtr->pixmap();
    ui->labelObjectImage->setPixmap(myPixmap);

    // scale the image, map dimensions from scene to view
    // first: get pixels of image per scene (in "qt meters", i.e. model meters*THESCALE)
    qreal myPixelsPerSceneQtMeter = myPixmap.width() / (myAOPtr->getTheWidth()*THESCALE);
    // secondly, get pixels in viewport per scene meter
    qreal mySceneQtMeterPerViewPortPixel = aParent->transform().m11();

    qreal myBitmapConvertedWidth  = myPixmap.width() / myPixelsPerSceneQtMeter * mySceneQtMeterPerViewPortPixel;
    qreal myBitmapConvertedHeight = myPixmap.height() / myPixelsPerSceneQtMeter * mySceneQtMeterPerViewPortPixel;
    ui->labelObjectImage->setMinimumSize(myBitmapConvertedWidth, myBitmapConvertedHeight);
    ui->labelObjectImage->setMaximumSize(myBitmapConvertedWidth, myBitmapConvertedHeight);
    ui->labelObjectImage->setScaledContents(true);

    // make it appear at the right height (next to the object that is clicked)
    // TODO: hardcoded Y coordinate for now...
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
