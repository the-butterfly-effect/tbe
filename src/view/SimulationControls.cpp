/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2013 Klaas van Gend
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
#include "SimulationControls.h"
#include "ui_SimulationControls.h"
#include "ImageCache.h"

#include <cstdio>

void SimState::onEntry ( QEvent * event )
{
    DEBUG4("SimulationControls-SimState %s onEntry!\n", ASCII(theName));
    QState::onEntry(event);
    isActive = true;
}

void SimState::onExit ( QEvent * event )
{
    QState::onExit(event);
    isActive = false;
}


SimulationControls::SimulationControls(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::SimulationControls)
{
    ui->setupUi(this);

//  QPixmap myPixmap;
//	ImageCache::getPixmap("SimulationControlsLabelFrame", size(), &myPixmap);
//	ui->statusFrame->(myPixmap);

    const QSize myIconSize(36,36);
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
    ui->statusLabel->setPixmap(theStoppedStatusPixmap);
}


SimulationControls::~SimulationControls()
{
    delete ui;
}


void SimulationControls::hideYourself()
{
    // todo: remove entirely
    emit hide();
}


void SimulationControls::hookSignalsUp(ViewWorld* aViewWorld)
{
    DEBUG1ENTRY;

    emit internalReset();

    // hook up states to signals for ViewWorld/World
    // note: no need to hook up theProblemState
    connect(theFailedState, SIGNAL(entered()), aViewWorld,
            SLOT(slot_signalPause()));
    connect(theForwardState, SIGNAL(entered()), aViewWorld,
            SLOT(slot_signalFF()));
    connect(thePausedState, SIGNAL(entered()), aViewWorld,
            SLOT(slot_signalPause()));
    connect(theRunningState, SIGNAL(entered()), aViewWorld,
            SLOT(slot_signalPlay()));
    connect(theStoppedState, SIGNAL(entered()), aViewWorld,
            SLOT(slot_signalReset()));

    connect(this, SIGNAL(go_quadspeed()), aViewWorld,
            SLOT(slot_signal4F()));
}


void SimulationControls::parentResize(const QSize& aSize)
{
    // TODO/FIXME: magic numbers here
    // I bet these have to be different for Windows and MacOSX :-(
	move(aSize.width()-size().width(),0);
}

void SimulationControls::setup(QMenu* aMenuPtr)
{
    theFailedState  = new SimState(&theSimStateMachine, "Failed");
    theForwardState = new SimState(&theSimStateMachine, "Forward");
    theHiddenState  = new SimState(&theSimStateMachine, "Hidden");
    thePausedState  = new SimState(&theSimStateMachine, "Paused");
    theProblemState = new SimState(&theSimStateMachine, "Problem");
    theRunningState = new SimState(&theSimStateMachine, "Running");
    theStoppedState = new SimState(&theSimStateMachine, "Stopped");
    theSimStateMachine.setInitialState(theStoppedState);

    // add actions and their quick keys
    theForwardAction = new QAction(theForwardIcon, tr("&Forward"), NULL);
    //: translators: 'f' is for (fast) forward
    theForwardAction->setShortcut(QKeySequence(tr("f")));

    // add actions and their quick keys
    thePauseAction = new QAction(thePauseIcon, tr("P&ause"), NULL);

    // add actions and their quick keys
    thePlayAction = new QAction(thePlayIcon, tr("&Play"), NULL);

    // add actions and their quick keys
    theResetAction = new QAction(theResetIcon, tr("&Reset"), NULL);
    //: translators: 'r' is for reset
    theResetAction->setShortcut(QKeySequence(tr("r")));

    QKeySequence mySpaceKey(tr("Space"));
    QKeySequence myEmptyKey(tr(""));

    //: translators: really-fast-forward is only available as a key shortcut
    //: it should be shift-<normal fast-forward>...
    QKeySequence my4FwdKey(tr("Shift+f"));
    the4FAction = new QAction(NULL);
    the4FAction->setShortcut(my4FwdKey);
    //Qt::ApplicationShortcu
    this->addAction(the4FAction);
    connect(the4FAction, SIGNAL(triggered()), this, SLOT(slot_4SpeedForward()));

    aMenuPtr->addAction(thePauseAction);
    aMenuPtr->addAction(thePlayAction);
    aMenuPtr->addAction(theResetAction);
    aMenuPtr->addAction(theForwardAction);

    ui->buttonForward->setDefaultAction(theForwardAction);
    ui->buttonPause->setDefaultAction(thePauseAction);
    ui->buttonPlay->setDefaultAction(thePlayAction);
    ui->buttonReset->setDefaultAction(theResetAction);

	QLabel* myLabelPtr = ui->statusLabel;

    // add transitions here
    theStoppedState->addTransition(thePlayAction, SIGNAL(triggered()), theRunningState);
    theStoppedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
    theStoppedState->addTransition(this, SIGNAL(internalCrossPresent()), theProblemState);
    theStoppedState->addTransition(this, SIGNAL(hide()), theHiddenState);

    theRunningState->addTransition(thePauseAction, SIGNAL(triggered()), thePausedState);
    theRunningState->addTransition(theForwardAction, SIGNAL(triggered()), theForwardState);
    theRunningState->addTransition(theResetAction, SIGNAL(triggered()), theStoppedState);
    theRunningState->addTransition(this, SIGNAL(internalFailed()), theFailedState);
    theRunningState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
    theRunningState->addTransition(this, SIGNAL(hide()), theHiddenState);

    theProblemState->addTransition(this, SIGNAL(internalCrossGone()), theStoppedState);
    theProblemState->addTransition(this, SIGNAL(hide()), theHiddenState);
    theProblemState->addTransition(this, SIGNAL(internalReset()), theStoppedState);

    thePausedState ->addTransition(thePlayAction, SIGNAL(triggered()), theRunningState);
    thePausedState ->addTransition(theResetAction, SIGNAL(triggered()), theStoppedState);
    thePausedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
    thePausedState->addTransition(this, SIGNAL(hide()), theHiddenState);

    theHiddenState ->addTransition(this, SIGNAL(show()), theStoppedState);
    theHiddenState->addTransition(this, SIGNAL(internalReset()), theStoppedState);

    theFailedState ->addTransition(theResetAction, SIGNAL(triggered()), theStoppedState);
    theFailedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
    theFailedState->addTransition(this, SIGNAL(hide()), theHiddenState);

    theForwardState->addTransition(thePauseAction, SIGNAL(triggered()), thePausedState);
    theForwardState->addTransition(thePlayAction, SIGNAL(triggered()), theRunningState);
    theForwardState->addTransition(this, SIGNAL(internalFailed()), theFailedState);
    theForwardState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
    theForwardState->addTransition(this, SIGNAL(hide()), theHiddenState);


    // set the start conditions for the icons for each state
    theStoppedState->assignProperty(theForwardAction,"enabled", false);
    theStoppedState->assignProperty(thePauseAction,  "enabled", false);
    theStoppedState->assignProperty(thePlayAction,   "enabled", true);
    theStoppedState->assignProperty(theResetAction,  "enabled", false);
    theStoppedState->assignProperty(myLabelPtr,      "pixmap",  theStoppedStatusPixmap);
    theStoppedState->assignProperty(thePlayAction,   "shortcut", mySpaceKey);
    theStoppedState->assignProperty(thePauseAction,  "shortcut", myEmptyKey);
    // entering running state
    theRunningState->assignProperty(theForwardAction,"enabled", true);
    theRunningState->assignProperty(thePauseAction,  "enabled", true);
    theRunningState->assignProperty(thePlayAction,   "enabled", false);
    theRunningState->assignProperty(theResetAction,  "enabled", true);
    theRunningState->assignProperty(myLabelPtr,      "pixmap",  theRunningStatusPixmap);
    theRunningState->assignProperty(thePlayAction,   "shortcut", myEmptyKey);
    theRunningState->assignProperty(thePauseAction,  "shortcut", mySpaceKey);
    // entering problem state
    theProblemState->assignProperty(theForwardAction,"enabled", false);
    theProblemState->assignProperty(thePauseAction,  "enabled", false);
    theProblemState->assignProperty(thePlayAction,   "enabled", false);
    theProblemState->assignProperty(theResetAction,  "enabled", false);
    theProblemState->assignProperty(myLabelPtr,      "pixmap",  theProblemStatusPixmap);
    theProblemState->assignProperty(thePlayAction,   "shortcut", myEmptyKey);
    theProblemState->assignProperty(thePauseAction,  "shortcut", myEmptyKey);
    // entering paused state
    thePausedState->assignProperty(theForwardAction,"enabled", false);
    thePausedState->assignProperty(thePauseAction,  "enabled", false);
    thePausedState->assignProperty(thePlayAction,   "enabled", true);
    thePausedState->assignProperty(theResetAction,  "enabled", true);
    thePausedState->assignProperty(myLabelPtr,      "pixmap",  thePausedStatusPixmap);
    thePausedState->assignProperty(thePlayAction,   "shortcut", mySpaceKey);
    thePausedState->assignProperty(thePauseAction,  "shortcut", myEmptyKey);
    // entering hiden state
    // (none of the UI thingies matter)
    // entering forward state
    theForwardState->assignProperty(theForwardAction,"enabled", false);
    theForwardState->assignProperty(thePauseAction,  "enabled", true);
    theForwardState->assignProperty(thePlayAction,   "enabled", true);
    theForwardState->assignProperty(theResetAction,  "enabled", false);
    theForwardState->assignProperty(myLabelPtr,      "pixmap",  theForwardStatusPixmap);
    theForwardState->assignProperty(thePlayAction,   "shortcut", myEmptyKey);
    theForwardState->assignProperty(thePauseAction,  "shortcut", mySpaceKey);
    // entering failed state
    theFailedState->assignProperty(theForwardAction,"enabled", false);
    theFailedState->assignProperty(thePauseAction,  "enabled", false);
    theFailedState->assignProperty(thePlayAction,   "enabled", false);
    theFailedState->assignProperty(theResetAction,  "enabled", true);
    theFailedState->assignProperty(myLabelPtr,      "pixmap",  theFailedStatusPixmap);
    theFailedState->assignProperty(thePlayAction,   "shortcut", myEmptyKey);
    theFailedState->assignProperty(thePauseAction,  "shortcut", myEmptyKey);

    emit theSimStateMachine.start();
}

void SimulationControls::showYourself()
{
    emit show();
}


void SimulationControls::slotNumberOfCrossesChanged(int aNewNumber)
{
    if (aNewNumber==0)
        emit internalCrossGone();
    else
        emit internalCrossPresent();
}

void SimulationControls::slot_4SpeedForward(void)
{
    // if in play mode, fake going to fast forward
    // but secretly emit a second signal to go a lot faster
    if (theRunningState->isActive==true)
    {
        emit theForwardAction->trigger();
        emit go_quadspeed();
    }
}
