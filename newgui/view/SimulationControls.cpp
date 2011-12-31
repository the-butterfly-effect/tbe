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

#include <QtCore/QPropertyAnimation>
#include <QtCore/QSignalTransition>

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


void SimulationControls::hookSignalsUp(ViewWorld* aViewWorld)
{
	DEBUG1ENTRY;
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
	QAction* myTopAction = new QAction(thePlayIcon, tr("&Play"), NULL);
	//: translators: space is for start/pause the sim - no need to translate
	myTopAction->setShortcut(QKeySequence(tr("Space")));
	QAction* myBotAction = new QAction(theFwdIcon, tr("&Fast Forward"), NULL);
	//: translators: 'f' is for fast forward - make sure it fits your "&Fast Forward"
	QKeySequence myFwdKey(tr("f"));
	//: translators: 'r' is for reset - make sure it fits your "&Reset"
	QKeySequence myResetKey(tr("r"));

	aMenuPtr->addAction(myTopAction);
	aMenuPtr->addAction(myBotAction);

	ui->topButton->setDefaultAction(myTopAction);
	ui->bottomButton->setDefaultAction(myBotAction);

	QLabel* myLabelPtr = ui->statusLabel;

	// add transitions here
	theStoppedState->addTransition(myTopAction, SIGNAL(triggered()), theRunningState);
	theRunningState->addTransition(myTopAction, SIGNAL(triggered()), thePausedState);
	theRunningState->addTransition(myBotAction, SIGNAL(triggered()), theForwardState);
	theRunningState->addTransition(this, SIGNAL(failed()), theFailedState);
	thePausedState ->addTransition(myTopAction, SIGNAL(triggered()), theRunningState);
	thePausedState ->addTransition(myBotAction, SIGNAL(triggered()), theStoppedState);
	theFailedState ->addTransition(myBotAction, SIGNAL(triggered()), theStoppedState);
	theForwardState->addTransition(myTopAction, SIGNAL(triggered()), thePausedState);
	theForwardState->addTransition(myBotAction, SIGNAL(triggered()), theRunningState);
	theForwardState->addTransition(this, SIGNAL(failed()), theFailedState);

	// set the start conditions for the icons for each state
	// upon entering stopped state, Top = Play/enabled, Bottom = FF/disable
	theStoppedState->assignProperty(myTopAction, "icon",    thePlayIcon);
	theStoppedState->assignProperty(myTopAction, "enabled", true);
	theStoppedState->assignProperty(myBotAction, "icon",    theFwdIcon);
	theStoppedState->assignProperty(myBotAction, "enabled", false);
	theStoppedState->assignProperty(myBotAction, "shortcut", myFwdKey);
	theStoppedState->assignProperty(myLabelPtr,   "pixmap",  theStopStatusPixmap);
	// upon entering running state, Top = Pause/enabled, Bottom = FF/enabled
	theRunningState->assignProperty(myTopAction, "icon",    thePauseIcon);
	theRunningState->assignProperty(myTopAction, "enabled", true);
	theRunningState->assignProperty(myBotAction, "icon",    theFwdIcon);
	theRunningState->assignProperty(myBotAction, "shortcut", myFwdKey);
	theRunningState->assignProperty(myBotAction, "enabled", true);
	theRunningState->assignProperty(myLabelPtr,  "pixmap",  thePlayStatusPixmap);
	// upon entering paused  state, Top = Play/enabled, Bottom = Reset/enabled
	thePausedState ->assignProperty(myTopAction, "icon",    thePlayIcon);
	thePausedState ->assignProperty(myTopAction, "enabled", true);
	thePausedState ->assignProperty(myBotAction, "icon",    theResetIcon);
	thePausedState ->assignProperty(myBotAction, "shortcut", myResetKey);
	thePausedState ->assignProperty(myBotAction, "enabled", true);
	thePausedState ->assignProperty(myLabelPtr,  "pixmap",  thePauseStatusPixmap);
	// upon entering forward state, Top = Pause/enabled, Bottom = Play/enabled
	theForwardState->assignProperty(myTopAction, "icon",    thePauseIcon);
	theForwardState->assignProperty(myTopAction, "enabled", true);
	theForwardState->assignProperty(myBotAction, "icon",    thePlayIcon);
	theForwardState->assignProperty(myBotAction, "shortcut", myFwdKey);
	theForwardState->assignProperty(myBotAction, "enabled", true);
	theForwardState->assignProperty(myLabelPtr,  "pixmap",  theFFStatusPixmap);
	// upon entering failed  state, Top = Play/disabled, Bottom = Reset/enabled
	theFailedState ->assignProperty(myTopAction, "icon",    thePlayIcon);
	theFailedState ->assignProperty(myTopAction, "enabled", false);
	theFailedState ->assignProperty(myBotAction, "icon",    theResetIcon);
	theFailedState ->assignProperty(myBotAction, "shortcut", myResetKey);
	theFailedState ->assignProperty(myBotAction, "enabled", true);
	theFailedState ->assignProperty(myLabelPtr,  "pixmap",  theFailStatusPixmap);

	emit theSimStateMachine.start();
}
