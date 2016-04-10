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

#include <QMenu>

GameControls::GameControls(QWidget *parent) :
    QLabel(parent),
    theGameButtonGroup(this),
    ui(new Ui::GameControls)
{
    ui->setupUi(this);

    const QSize myIconSize(16,16);
    theForwardIcon= ImageCache::getQIcon("ActionFastForward", myIconSize);
    thePauseIcon  = ImageCache::getQIcon("ActionMenuPause", myIconSize);
    thePlayIcon   = ImageCache::getQIcon("ActionMenuPlay", myIconSize);
    theResetIcon  = ImageCache::getQIcon("ActionUndo", myIconSize);

    // ordering is important for the below operations!
    QPixmap myTempPixmap;
    ImageCache::getPixmap("StatusFail",   QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusFF",     QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusPlay",   QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusPause",  QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusProblem",QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("Status4F",     QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusSlow",   QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusStop",   QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ImageCache::getPixmap("StatusWon",    QSize(64,64), &myTempPixmap);
    thePixmaps.append(myTempPixmap);
    ui->statusLabel->setPixmap(thePixmaps[0]);
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
    theForwardAction->setCheckable(true);

    // add actions and their quick keys
    thePauseAction = new QAction(thePauseIcon, tr("P&ause"), nullptr);
    thePauseAction->setCheckable(true);

    // add actions and their quick keys
    thePlayAction = new QAction(thePlayIcon, tr("&Play"), nullptr);
    thePlayAction->setCheckable(true);

    // add actions and their quick keys
    theResetAction = new QAction(theResetIcon, tr("&Reset"), nullptr);
    //: translators: 'r' is for reset
    theResetAction->setShortcut(QKeySequence(tr("r")));

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

    // do not add Slow and RealFast - they're keyboard shortcuts only
    aMenuPtr->addAction(thePauseAction);
    aMenuPtr->addAction(thePlayAction);
    aMenuPtr->addAction(theResetAction);
    aMenuPtr->addAction(theForwardAction);

    // add everything to the QActionGroup to make them mutually exclusive
    theGameButtonGroup.addAction(theForwardAction);
    theGameButtonGroup.addAction(thePauseAction);
    theGameButtonGroup.addAction(thePlayAction);
    theGameButtonGroup.addAction(theRealFastAction);
    //theGameButtonGroup.addAction(theResetAction); reset is not mutually exclusive
    theGameButtonGroup.addAction(theSlowAction);
    theGameButtonGroup.setExclusive(true);

	ui->buttonForward->setDefaultAction(theForwardAction);
    ui->buttonPause->setDefaultAction(thePauseAction);
    ui->buttonPlay->setDefaultAction(thePlayAction);
    ui->buttonReset->setDefaultAction(theResetAction);

    // hook the triggered() signals to this class' signals
    connect (theForwardAction, SIGNAL(triggered()), this,  SIGNAL(signal_Forward_triggered()));
    connect (thePauseAction, SIGNAL(triggered()), this,    SIGNAL(signal_Pause_triggered()));
    connect (thePlayAction, SIGNAL(triggered()), this,     SIGNAL(signal_Play_triggered()));
    connect (theRealFastAction, SIGNAL(triggered()), this, SIGNAL(signal_RealFast_triggered()));
    connect (theResetAction, SIGNAL(triggered()), this,    SIGNAL(signal_Reset_triggered()));
    connect (theSlowAction, SIGNAL(triggered()), this,     SIGNAL(signal_Slow_triggered()));
}


void GameControls::slot_updateIcon(GameStateMachine::States aStatus)
{
    DEBUG1ENTRY;
    Q_ASSERT(aStatus >= GameStateMachine::FailedStatus);
    Q_ASSERT(aStatus <= GameStateMachine::WonStatus);
    ui->statusLabel->setPixmap(thePixmaps[aStatus]);
    QKeySequence mySpaceKey(tr("Space", "key for start/pause the simulation"));
    QKeySequence myEmptyKey("");
    switch(aStatus)
    {
    case GameStateMachine::FailedStatus:
    case GameStateMachine::ProblemStatus:
    case GameStateMachine::WonStatus:
        // disable the game controls incl. reset
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setShortcut(myEmptyKey);
        theResetAction->setEnabled(false);
        theGameButtonGroup.setEnabled(false);
        break;
    case GameStateMachine::StoppedStatus:
        // enable the game controls incl. reset
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setShortcut(mySpaceKey);
        theResetAction->setEnabled(false);
        theGameButtonGroup.setEnabled(true);
        break;
    case GameStateMachine::ForwardStatus:
    case GameStateMachine::NormalStatus:
    case GameStateMachine::RealFastStatus:
    case GameStateMachine::SlowStatus:
        // don't allow reset while running (even though the state machine can handle it)
        thePauseAction->setEnabled(true);
        thePauseAction->setShortcut(mySpaceKey);
        thePlayAction->setShortcut(myEmptyKey);
        theResetAction->setEnabled(false);
        break;
    case GameStateMachine::PausedStatus:
        // enable the reset - only makes sense in this state :-)
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setShortcut(mySpaceKey);
        theResetAction->setEnabled(true);
        break;
    }
}
