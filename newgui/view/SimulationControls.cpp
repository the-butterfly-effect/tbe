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

#include <QMenuBar>
#include <QPropertyAnimation>
#include <QSignalTransition>

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


void SimulationControls::parentResize(const QSize& aSize)
{
    // TODO/FIXME: magic numbers here
    // I bet these have to be different for Windows and MacOSX :-(
    move(aSize.width()-size().width()-2,-8);
}

void SimulationControls::setup(QMenu* aMenuPtr)
{
	//	QState* myInvalidState  = new SimState(&theSimStateMachine, "Invalid");
	QState* myFailedState  = new SimState(&theSimStateMachine, "Failed");
	QState* myForwardState = new SimState(&theSimStateMachine, "Forward");
	QState* myPausedState  = new SimState(&theSimStateMachine, "Paused");
	QState* myRunningState = new SimState(&theSimStateMachine, "Running");
	QState* myStoppedState = new SimState(&theSimStateMachine, "Stopped");
	theSimStateMachine.setInitialState(myStoppedState);

	// add actions
	QAction* myTopAction = new QAction(thePlayIcon, tr("&Play"), NULL);
	QAction* myBotAction = new QAction(theFwdIcon, tr("&Fast Forward"), NULL);

	aMenuPtr->addAction(myTopAction);
	aMenuPtr->addAction(myBotAction);

	ui->topButton->setDefaultAction(myTopAction);
	ui->bottomButton->setDefaultAction(myBotAction);

	QLabel* myLabelPtr = ui->statusLabel;

	// add transitions here
	myStoppedState->addTransition(myTopAction, SIGNAL(triggered()), myRunningState);
	myRunningState->addTransition(myTopAction, SIGNAL(triggered()), myPausedState);
	myRunningState->addTransition(myBotAction, SIGNAL(triggered()), myForwardState);
	myRunningState->addTransition(this, SIGNAL(failed()), myFailedState);
	myPausedState ->addTransition(myTopAction, SIGNAL(triggered()), myRunningState);
	myPausedState ->addTransition(myBotAction, SIGNAL(triggered()), myStoppedState);
	myFailedState ->addTransition(myBotAction, SIGNAL(triggered()), myStoppedState);
	myForwardState->addTransition(myTopAction, SIGNAL(triggered()), myPausedState);
	myForwardState->addTransition(myBotAction, SIGNAL(triggered()), myRunningState);
	myForwardState->addTransition(this, SIGNAL(failed()), myFailedState);

	// set the start conditions for the icons for each state
	// upon entering stopped state, Top = Play/enabled, Bottom = FF/disable
	myStoppedState->assignProperty(myTopAction, "icon",    thePlayIcon);
	myStoppedState->assignProperty(myTopAction, "enabled", true);
	myStoppedState->assignProperty(myBotAction, "icon",    theFwdIcon);
	myStoppedState->assignProperty(myBotAction, "enabled", false);
	myStoppedState->assignProperty(myLabelPtr,   "pixmap",  theStopStatusPixmap);
	// upon entering running state, Top = Pause/enabled, Bottom = FF/enabled
	myRunningState->assignProperty(myTopAction, "icon",    thePauseIcon);
	myRunningState->assignProperty(myTopAction, "enabled", true);
	myRunningState->assignProperty(myBotAction, "icon",    theFwdIcon);
	myRunningState->assignProperty(myBotAction, "enabled", true);
	myRunningState->assignProperty(myLabelPtr,  "pixmap",  thePlayStatusPixmap);
	// upon entering paused  state, Top = Play/enabled, Bottom = Reset/enabled
	myPausedState ->assignProperty(myTopAction, "icon",    thePlayIcon);
	myPausedState ->assignProperty(myTopAction, "enabled", true);
	myPausedState ->assignProperty(myBotAction, "icon",    theResetIcon);
	myPausedState ->assignProperty(myBotAction, "enabled", true);
	myPausedState ->assignProperty(myLabelPtr,  "pixmap",  thePauseStatusPixmap);
	// upon entering forward state, Top = Pause/enabled, Bottom = Play/enabled
	myForwardState->assignProperty(myTopAction, "icon",    thePauseIcon);
	myForwardState->assignProperty(myTopAction, "enabled", true);
	myForwardState->assignProperty(myBotAction, "icon",    thePlayIcon);
	myForwardState->assignProperty(myBotAction, "enabled", true);
	myForwardState->assignProperty(myLabelPtr,  "pixmap",  theFFStatusPixmap);
	// upon entering failed  state, Top = Play/disabled, Bottom = Reset/enabled
	myFailedState ->assignProperty(myTopAction, "icon",    thePlayIcon);
	myFailedState ->assignProperty(myTopAction, "enabled", false);
	myFailedState ->assignProperty(myBotAction, "icon",    theResetIcon);
	myFailedState ->assignProperty(myBotAction, "enabled", true);
	myFailedState ->assignProperty(myLabelPtr,  "pixmap",  theFailStatusPixmap);

	// TODO: hook up states to signals for ViewWorld/World

	emit theSimStateMachine.start();
}
