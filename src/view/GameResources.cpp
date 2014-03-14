/* The Butterfly Effect
 * This file copyright (C) 2011,2012  Klaas van Gend
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

#include "GameResources.h"
#include "ImageCache.h"
#include "Level.h"
#include <QAction>
#include <QMenuBar>
#include "Popup.h"
#include "tbe_global.h"
#include "ui_GameResources.h"
#include "ViewObject.h"

GameResources::GameResources(ResizingGraphicsView* aRSGVPtr) :
    AnimatedDialog(aRSGVPtr, AnimatedDialog::FROM_TOPRIGHT),
    ui(new Ui::GameResources),
    theLevelPtr(NULL),
    theParentPtr(aRSGVPtr)
{
    DEBUG1ENTRY;
    ui->setupUi(this);
    ui->theResetButton->setIcon(ImageCache::getQIcon("ActionUndo", QSize(32,32)));
    setAutoFillBackground (true);
}


GameResources::~GameResources()
{
	delete ui;
}


void GameResources::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        // TODO/FIXME: add change of Description and Title to new language...
        break;
    default:
        break;
    }
}


void GameResources::on_theOKButton_clicked()
{
    // update the toolbox viewing
 //   slot_startAppearing();
    // and go away
    emit disappearAnimated();
}


void GameResources::on_theResetButton_clicked()
{
    if (Popup::YesNoQuestion(tr("Undo all your work and go back to a clean start of this level?")))
    {
        emit disappearAnimated();
        emit signalReloadLevel();
    }
}


void GameResources::setLevelPtr(Level* aLevelPtr)
{
    DEBUG4ENTRY;
    Q_ASSERT(aLevelPtr!=NULL);
    theLevelPtr = aLevelPtr;

    ui->theLevelTitle->setText(theLevelPtr->theLevelName.result());
    //: translators: please do not try to translate the <b>%1</b> part!
    ui->theLevelAuthor->setText(tr("Level by: <b>%1</b>").arg(theLevelPtr->theLevelAuthor));
    ui->theInfoBox->setText(aLevelPtr->theLevelDescription.result());
}
