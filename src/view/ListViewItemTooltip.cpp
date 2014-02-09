/* The Butterfly Effect
 * This file copyright (C) 2011,2014 Klaas van Gend
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
#include "InsertUndoCommand.h"
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
    assert(aTBGPtr);
    assert(aParent);

    // set the description and help
    ui->labelName->setText(theTBGPtr->theGroupName.result());
    ui->labelCount->setText(tr("%1x").arg(theTBGPtr->count()));
    AbstractObject* myAOPtr = theTBGPtr->first();
    ui->labelDescription->setText(myAOPtr->getToolTip());

    // set the image
    ViewObject* myVOPtr = myAOPtr->createViewObject();
    QPixmap myPixmap = myVOPtr->pixmap();

    // scale the image, map dimensions from scene to view
    // first: get pixels of image per scene (in "qt meters", i.e. model meters*THESCALE)
    qreal myPixelsPerSceneQtMeter = myPixmap.width() / (myAOPtr->getTheWidth()*THESCALE);
    // secondly, get pixels in viewport per scene meter
    qreal mySceneQtMeterPerViewPortPixel = aParent->transform().m11();

    qreal myBitmapConvertedWidth  = myPixmap.width() / myPixelsPerSceneQtMeter * mySceneQtMeterPerViewPortPixel;
    qreal myBitmapConvertedHeight = myPixmap.height() / myPixelsPerSceneQtMeter * mySceneQtMeterPerViewPortPixel;
    QSize myConvertedSize(myBitmapConvertedWidth, myBitmapConvertedHeight);
    ui->buttonObjectImage->setMinimumSize(myConvertedSize);
    ui->buttonObjectImage->setMaximumSize(myConvertedSize);
    ui->buttonObjectImage->setIcon(myPixmap.scaled(myConvertedSize, Qt::IgnoreAspectRatio));
    ui->buttonObjectImage->setIconSize(myConvertedSize);

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



void ListViewItemTooltip::on_buttonObjectImage_clicked()
{
    // TODO: make the newly inserted object appear underneath where one
    // clicked the button...
    InsertUndoCommand::createInsertUndoCommand(theTBGPtr);
    emit disappearAnimated();
}
