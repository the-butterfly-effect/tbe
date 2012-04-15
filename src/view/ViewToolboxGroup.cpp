/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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
#include "GameResources.h"
#include "InsertUndoCommand.h"
#include "Popup.h"
#include "Position.h"
#include "ViewToolboxGroup.h"
#include "ViewObject.h"
#include "ViewWorld.h"


ViewToolboxGroup::ViewToolboxGroup(ToolboxGroup* aTBGPtr, GameResources* aGRPtr, QWidget* aParentPtr)
    : QPushButton(aParentPtr),
      theTBGPtr(aTBGPtr), theVBoxLayout(this)
{
    Q_ASSERT(aTBGPtr!=NULL);

    if (theTBGPtr->count() > 0)
    {
        AbstractObject* myAOPtr = theTBGPtr->first();
        ViewObject* myVOPtr = myAOPtr->createViewObject();

        // We have a few different measures:
        //   1) Planned object width in meters
        //   2) THESCALE converts meters to pixels, part 1
        //   3) Transform matrix, which scales the viewimage, i.e. meters to pixels, part 2

        theIconSize.setWidth(( myAOPtr->getTheWidth()* (float)THESCALE *
                           aGRPtr->theTransformMatrix.m11()));
        theIconSize.setHeight(( myAOPtr->getTheHeight()* (float)THESCALE *
                           aGRPtr->theTransformMatrix.m22()));

        QPixmap myPixmap = myVOPtr->pixmap();
        theIcon = myPixmap.scaled(theIconSize);
    }

    theVBoxLayout.addWidget(&theCountLabel);
    theVBoxLayout.addWidget(&theDescriptionLabel);

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


void ViewToolboxGroup::updateCount()
{
    theCountLabel.setText(QString("<b>%1x</b>").arg(theTBGPtr->count()));
    theCountLabel.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    theDescriptionLabel.setText(theTBGPtr->theGroupName.result());
    theDescriptionLabel.setAlignment(Qt::AlignBottom | Qt::AlignRight);

    QSize myIconSize = theIconSize;
    if (theTBGPtr->count() > 0)
    {
        setIcon(theIcon);
        setIconSize(theIconSize);
        setText("");
    }
    else
    {
        setIcon(QIcon());
        setText(tr("(empty)"));
        myIconSize = QSize(40,20);
    }


    int myMinWidth = max2(theCountLabel.minimumSizeHint().width() +
                          theDescriptionLabel.minimumSizeHint().width(),
                          myIconSize.width());
    int myMinHeight= theCountLabel.minimumSizeHint().height() +
                          myIconSize.height();
    theMinSize = QSize(myMinWidth, myMinHeight);
    setMinimumSize(theMinSize);
}

