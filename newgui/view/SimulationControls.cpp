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

#include "tbe_global.h"
#include "SimulationControls.h"
#include "ui_SimulationControls.h"
#include "ImageCache.h"

#include <cstdio>

void SimState::onEntry ( QEvent * event )
{
	DEBUG4("SimulationControls-SimState %s onEntry!\n", ASCII(theName));
	QState::onEntry(event);
}


SimulationControls::SimulationControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationControls)
{
    ui->setupUi(this);

    QPixmap myPixmap;
    ImageCache::getPixmap("StatusPlay", QSize(64,64), &myPixmap);
    ui->statusLabel->setPixmap(myPixmap);

    thePlayIcon  = ImageCache::getQIcon("ActionMenuPlay", QSize(32,32));
    thePauseIcon = ImageCache::getQIcon("ActionMenuPause", QSize(32,32));
    theStopIcon  = ImageCache::getQIcon("ActionMenuStop", QSize(32,32));
    theResetIcon = ImageCache::getQIcon("ActionUndo", QSize(32,32));
    theFwdIcon   = ImageCache::getQIcon("ActionFastForward", QSize(32,32));

    ImageCache::getPixmap("StatusFail",  QSize(64,64), &theFailStatusPixmap);
    ImageCache::getPixmap("StatusFault", QSize(64,64), &theFaultStatusPixmap);
    ImageCache::getPixmap("StatusFF",    QSize(64,64), &theFFStatusPixmap);
    ImageCache::getPixmap("StatusPlay",  QSize(64,64), &thePlayStatusPixmap);
    ImageCache::getPixmap("StatusPause", QSize(64,64), &thePauseStatusPixmap);
    ImageCache::getPixmap("StatusStop",  QSize(64,64), &theStopStatusPixmap);
}


SimulationControls::~SimulationControls()
{
    delete ui;
}


void SimulationControls::hideYourself()
{
	emit hide();
	theTopAction->setEnabled(false);
	theBotAction->setEnabled(false);
}


void SimulationControls::hookSignalsUp(ViewWorld* aViewWorld)
{
    DEBUG1ENTRY;

    emit internalReset();

    // hook up states to signals for ViewWorld/World
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
}


void SimulationControls::parentResize(const QSize& aSize)
{
    // TODO/FIXME: magic numbers here
    // I bet these have to be different for Windows and MacOSX :-(
    move(aSize.width()-size().width()-2,-8);
}

void SimulationControls::setup(QMenu* aMenuPtr)
{
	//	QState* myInvalidState  = new SimState(&theSimStateMachine, "Invalid");
	theFailedState  = new SimState(&theSimStateMachine, "Failed");
	theForwardState = new SimState(&theSimStateMachine, "Forward");
	thePausedState  = new SimState(&theSimStateMachine, "Paused");
	theRunningState = new SimState(&theSimStateMachine, "Running");
	theStoppedState = new SimState(&theSimStateMachine, "Stopped");
	theSimStateMachine.setInitialState(theStoppedState);

	// add actions and their quick keys
	theTopAction = new QAction(thePlayIcon, tr("&Play"), NULL);
	//: translators: space is for start/pause the sim - no need to translate
	theTopAction->setShortcut(QKeySequence(tr("Space")));
	theBotAction = new QAction(theFwdIcon, tr("&Fast Forward"), NULL);
	//: translators: 'f' is for fast forward - make sure it fits your "&Fast Forward"
	QKeySequence myFwdKey(tr("f"));
	//: translators: 'r' is for reset - make sure it fits your "&Reset"
	QKeySequence myResetKey(tr("r"));

	aMenuPtr->addAction(theTopAction);
	aMenuPtr->addAction(theBotAction);

	ui->topButton->setDefaultAction(theTopAction);
	ui->bottomButton->setDefaultAction(theBotAction);

	QLabel* myLabelPtr = ui->statusLabel;

	// add transitions here
	theStoppedState->addTransition(theTopAction, SIGNAL(triggered()), theRunningState);
	theStoppedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
	theRunningState->addTransition(theTopAction, SIGNAL(triggered()), thePausedState);
	theRunningState->addTransition(theBotAction, SIGNAL(triggered()), theForwardState);
	theRunningState->addTransition(this, SIGNAL(internalFailed()), theFailedState);
	theRunningState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
	thePausedState ->addTransition(theTopAction, SIGNAL(triggered()), theRunningState);
	thePausedState ->addTransition(theBotAction, SIGNAL(triggered()), theStoppedState);
	thePausedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
	theFailedState ->addTransition(theBotAction, SIGNAL(triggered()), theStoppedState);
	theFailedState->addTransition(this, SIGNAL(internalReset()), theStoppedState);
	theForwardState->addTransition(theTopAction, SIGNAL(triggered()), thePausedState);
	theForwardState->addTransition(theBotAction, SIGNAL(triggered()), theRunningState);
	theForwardState->addTransition(this, SIGNAL(internalFailed()), theFailedState);
	theForwardState->addTransition(this, SIGNAL(internalReset()), theStoppedState);

	// set the start conditions for the icons for each state
	// upon entering stopped state, Top = Play/enabled, Bottom = FF/disable
	theStoppedState->assignProperty(theTopAction, "icon",     thePlayIcon);
	theStoppedState->assignProperty(theTopAction, "enabled",  true);
	theStoppedState->assignProperty(theBotAction, "icon",     theFwdIcon);
	theStoppedState->assignProperty(theBotAction, "enabled",  false);
	theStoppedState->assignProperty(theBotAction, "shortcut", myFwdKey);
	theStoppedState->assignProperty(myLabelPtr,   "pixmap",   theStopStatusPixmap);
	// upon entering running state, Top = Pause/enabled, Bottom = FF/enabled
	theRunningState->assignProperty(theTopAction, "icon",     thePauseIcon);
	theRunningState->assignProperty(theTopAction, "enabled",  true);
	theRunningState->assignProperty(theBotAction, "icon",     theFwdIcon);
	theRunningState->assignProperty(theBotAction, "shortcut", myFwdKey);
	theRunningState->assignProperty(theBotAction, "enabled",  true);
	theRunningState->assignProperty(myLabelPtr,   "pixmap",  thePlayStatusPixmap);
	// upon entering paused  state, Top = Play/enabled, Bottom = Reset/enabled
	thePausedState ->assignProperty(theTopAction, "icon",     thePlayIcon);
	thePausedState ->assignProperty(theTopAction, "enabled",  true);
	thePausedState ->assignProperty(theBotAction, "icon",     theResetIcon);
	thePausedState ->assignProperty(theBotAction, "shortcut", myResetKey);
	thePausedState ->assignProperty(theBotAction, "enabled",  true);
	thePausedState ->assignProperty(myLabelPtr,   "pixmap",   thePauseStatusPixmap);
	// upon entering forward state, Top = Pause/enabled, Bottom = Play/enabled
	theForwardState->assignProperty(theTopAction, "icon",     thePauseIcon);
	theForwardState->assignProperty(theTopAction, "enabled",  true);
	theForwardState->assignProperty(theBotAction, "icon",     thePlayIcon);
	theForwardState->assignProperty(theBotAction, "shortcut", myFwdKey);
	theForwardState->assignProperty(theBotAction, "enabled",  true);
	theForwardState->assignProperty(myLabelPtr,   "pixmap",   theFFStatusPixmap);
	// upon entering failed  state, Top = Play/disabled, Bottom = Reset/enabled
	theFailedState ->assignProperty(theTopAction, "icon",     thePlayIcon);
	theFailedState ->assignProperty(theTopAction, "enabled",  false);
	theFailedState ->assignProperty(theBotAction, "icon",     theResetIcon);
	theFailedState ->assignProperty(theBotAction, "shortcut", myResetKey);
	theFailedState ->assignProperty(theBotAction, "enabled",  true);
	theFailedState ->assignProperty(myLabelPtr,   "pixmap",   theFailStatusPixmap);

	emit theSimStateMachine.start();
}

void SimulationControls::showYourself()
{
	emit show();
	theTopAction->setEnabled(true);
	theBotAction->setEnabled(true);
}
