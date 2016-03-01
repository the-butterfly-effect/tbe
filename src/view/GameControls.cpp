/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2013,2016 Klaas van Gend
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

#include "tbe_global.h"
#include "GameControls.h"
#include "ui_GameControls.h"
#include "ImageCache.h"
#include "MainWindow.h"
#include "resizinggraphicsview.h"

#include <cstdio>

GameControls::GameControls(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::GameControls)
{
    ui->setupUi(this);

    const QSize myIconSize(16,16);
    theForwardIcon= ImageCache::getQIcon("ActionFastForward", myIconSize);
    thePauseIcon  = ImageCache::getQIcon("ActionMenuPause", myIconSize);
    thePlayIcon   = ImageCache::getQIcon("ActionMenuPlay", myIconSize);
    theResetIcon  = ImageCache::getQIcon("ActionUndo", myIconSize);

	ImageCache::getPixmap("StatusFail",   QSize(64,64), &theFailedStatusPixmap);
    ImageCache::getPixmap("StatusFF",     QSize(64,64), &theForwardStatusPixmap);
    ImageCache::getPixmap("StatusPlay",   QSize(64,64), &theRunningStatusPixmap);
    ImageCache::getPixmap("StatusPause",  QSize(64,64), &thePausedStatusPixmap);
    ImageCache::getPixmap("StatusProblem",QSize(64,64), &theProblemStatusPixmap);
    ImageCache::getPixmap("StatusStop",   QSize(64,64), &theStoppedStatusPixmap);
    ImageCache::getPixmap("Status4F",     QSize(64,64), &the4FStatusPixmap);
    ui->statusLabel->setPixmap(theStoppedStatusPixmap);
}


GameControls::~GameControls()
{
    delete ui;
}


void GameControls::parentResize(const QSize& aSize)
{
	move(aSize.width()-size().width(),0);
}

void GameControls::setup(QMenu* aMenuPtr)
{
    // add actions and their quick keys
    theForwardAction = new QAction(theForwardIcon, tr("&Forward"), nullptr);
    //: translators: 'f' is for (fast) forward
    theForwardAction->setShortcut(QKeySequence(tr("f")));

    // add actions and their quick keys
    thePauseAction = new QAction(thePauseIcon, tr("P&ause"), nullptr);

    // add actions and their quick keys
    thePlayAction = new QAction(thePlayIcon, tr("&Play"), nullptr);

    // add actions and their quick keys
    theResetAction = new QAction(theResetIcon, tr("&Reset"), nullptr);
    //: translators: 'r' is for reset
    theResetAction->setShortcut(QKeySequence(tr("r")));

    QKeySequence mySpaceKey(tr("Space"));
    QKeySequence myEmptyKey(tr(""));

    //: translators: really-fast-forward is only available as a key shortcut
    //: it should be shift-<normal fast-forward>...
    QKeySequence myRealFastKey(tr("Shift+f"));
    theRealFastAction = new QAction(nullptr);
    theRealFastAction->setShortcut(myRealFastKey);
    this->addAction(theRealFastAction);

    //: translators: slow is only available as a key shortcut
    //: it should be shift-S...
    QKeySequence mySlowKey(tr("Shift+s"));
    theSlowAction = new QAction(nullptr);
    theSlowAction->setShortcut(mySlowKey);
    this->addAction(theSlowAction);


    aMenuPtr->addAction(thePauseAction);
    aMenuPtr->addAction(thePlayAction);
    aMenuPtr->addAction(theResetAction);
    aMenuPtr->addAction(theForwardAction);

	ui->buttonForward->setDefaultAction(theForwardAction);
    ui->buttonPause->setDefaultAction(thePauseAction);
    ui->buttonPlay->setDefaultAction(thePlayAction);
    ui->buttonReset->setDefaultAction(theResetAction);

	QLabel* myLabelPtr = ui->statusLabel;
}
