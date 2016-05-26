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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObject.h"
#include "TriggerExplosion.h"   // for the DetonatorBox* type
#include "ImageCache.h"
#include "InsertUndoCommand.h"
#include "ListViewItemTooltip.h"
#include "Position.h"
#include "resizinggraphicsview.h"
#include "Translator.h"
#include "ViewObject.h"
#include "ui_ListViewItemTooltip.h"

ListViewItemTooltip::ListViewItemTooltip(ToolboxGroup *aTBGPtr,
                                         ResizingGraphicsView *aParent) :
    AnimatedDialog(aParent, AnimatedDialog::TOOLTIP),
    ui(new Ui::ListViewItemTooltip),
    theTBGPtr(aTBGPtr), thRSGVPtr(aParent)
{
    ui->setupUi(this);
    Q_ASSERT(aTBGPtr);
    Q_ASSERT(aParent);

    // set the description and help
    ui->labelName->setText(TheGetText(theTBGPtr->theGroupName));
    ui->labelCount->setText(tr("%1x").arg(theTBGPtr->count()));
    AbstractObjectPtr myAOPtr = theTBGPtr->last();
    ui->labelDescription->setText(myAOPtr->getToolTip());

    // set the image
    ViewObjectPtr myVOPtr = myAOPtr->createViewObject();

    // scale & rotate the image, map dimensions from scene to view
    qreal myBitmapConvertedWidth  = myAOPtr->getTheWidth() * THESCALE * aParent->transform().m11();
    qreal myBitmapConvertedHeight = myAOPtr->getTheHeight() * THESCALE * aParent->transform().m22();
    QSize myPixmapRegularSize(myBitmapConvertedWidth, myBitmapConvertedHeight);
    QPixmap myPixmap = myVOPtr->pixmap().scaled(myPixmapRegularSize, Qt::IgnoreAspectRatio);
    QTransform myTransform;
    myTransform.rotateRadians(-myAOPtr->getTempCenter().angle);
    QPixmap myFinalPixmap = myPixmap.transformed(myTransform);

    ui->buttonObjectImage->setMinimumSize(myFinalPixmap.size());
    ui->buttonObjectImage->setMaximumSize(myFinalPixmap.size());
    ui->buttonObjectImage->setIcon(myFinalPixmap);
    ui->buttonObjectImage->setIconSize(myFinalPixmap.size());

    AbstractObject::SizeDirections isResizable = myAOPtr->isResizable();
    if ((isResizable & AbstractObject::HORIZONTALRESIZE) &&
            (isResizable & AbstractObject::VERTICALRESIZE))
        addActionIcon("ActionResize", tr("You can resize the object in all directions."));
    else {
        if (isResizable & AbstractObject::HORIZONTALRESIZE)
            addActionIcon("ActionResizeHori", tr("You can resize the object horizontally."));
        if (isResizable & AbstractObject::VERTICALRESIZE)
            addActionIcon("ActionResizeVerti", tr("You can resize the object vertically."));
    }
    if (myAOPtr->isRotatable())
        addActionIcon("ActionRotate", tr("You can rotate the object."));
    if (dynamic_cast<DetonatorBox *>(myAOPtr.get()) != nullptr)
        addActionIcon("ActionSetNumber", tr("You can set the phone number."));
}


ListViewItemTooltip::~ListViewItemTooltip()
{

}


void ListViewItemTooltip::addActionIcon(const QString &anIconName, const QString &aToolTip)
{
    QLabel *myLabelPtr = new QLabel();
    QPixmap myPixmapPtr;
    ImageCache::getPixmap(anIconName, QSize(24, 24), &myPixmapPtr);
    myLabelPtr->setPixmap(myPixmapPtr);
    myLabelPtr->setStyleSheet("border-image: url(:/transparant.png);");
    myLabelPtr->setToolTip(aToolTip);
    ui->optionsLayout->addWidget(myLabelPtr);
}


void ListViewItemTooltip::adjustVPos(int aVPos)
{
    // First make sure that the dialog is on screen and properly sized.
    show();

    // Make it appear at the right height (next to the object that is clicked),
    // but don't go out of the screen (and give it a small margin)!

    if (aVPos + height() < thRSGVPtr->height())
        theYCoord = aVPos;
    else
        theYCoord = thRSGVPtr->height() - height() - 2;
}


void ListViewItemTooltip::on_buttonRemove_clicked()
{
    setDeleteOnDisappear(true);
    disappearAnimated();
}



void ListViewItemTooltip::on_buttonObjectImage_clicked()
{
    AbstractObjectPtr myAOPtr = theTBGPtr->last();
    // TODO: FIXME: MAGIC NUMBER BELOW - GUARANTEED TO BREAK ON HIGH-RES SCREENS
    Position mySpot(thRSGVPtr->mapToScene(x(), y() + 50), myAOPtr->getTempCenter().angle );
    InsertUndoCommand::createInsertUndoCommand(theTBGPtr, mySpot);
    emit disappearAnimated();
}
