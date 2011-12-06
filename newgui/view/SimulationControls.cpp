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
#include "Overlay.h"
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


SimulationControls::SimulationControls(QObject *parent) :
	QObject(parent)
{
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

void SimulationControls::setup(QMenu* aMenuPtr, Overlay* anOverlayPtr)
{
	//	QState* myInvalidState  = new SimState(&theSimStateMachine, "Invalid");
	QState* myFailedState  = new SimState(&theSimStateMachine, "Failed");
	QState* myForwardState = new SimState(&theSimStateMachine, "Forward");
	QState* myPausedState  = new SimState(&theSimStateMachine, "Paused");
	QState* myRunningState = new SimState(&theSimStateMachine, "Running");
	QState* myStoppedState = new SimState(&theSimStateMachine, "Stopped");
	theSimStateMachine.setInitialState(myStoppedState);

	// add actions
	QAction* myPlayAction = new QAction(thePlayIcon, tr("&Play"), NULL);
	QAction* myResetAction = new QAction(theResetIcon, tr("&Reset"), NULL);
	QAction* myFFAction = new QAction(theFwdIcon, tr("F&ast"), NULL);

	aMenuPtr->addAction(myPlayAction);
	aMenuPtr->addAction(myFFAction);
	aMenuPtr->addAction(myResetAction);

	anOverlayPtr->getPlayPtr()->setDefaultAction(myPlayAction);
	anOverlayPtr->getFFPtr()->setDefaultAction(myFFAction);
	anOverlayPtr->getResetPtr()->setDefaultAction(myResetAction);

	QLabel* myLabelPtr = anOverlayPtr->getStatusLabelPtr();

	// add transitions here
	myStoppedState->addTransition(myPlayAction,SIGNAL(triggered()),myRunningState);
	myRunningState->addTransition(myPlayAction, SIGNAL(triggered()), myPausedState);
	myRunningState->addTransition(myFFAction, SIGNAL(triggered()), myForwardState);
	myRunningState->addTransition(this, SIGNAL(failed()), myFailedState);
	myPausedState ->addTransition(myPlayAction, SIGNAL(triggered()), myRunningState);
	myPausedState ->addTransition(myResetAction, SIGNAL(triggered()), myStoppedState);
	myFailedState ->addTransition(myResetAction, SIGNAL(triggered()), myStoppedState);
	myForwardState->addTransition(myFFAction, SIGNAL(triggered()), myRunningState);
	myForwardState->addTransition(myPlayAction, SIGNAL(triggered()), myRunningState);
	myForwardState->addTransition(this, SIGNAL(failed()), myFailedState);

	// set the start conditions for the icons for each state
	// upon entering stopped state, FF=disable, P=play     R=disabled
	myStoppedState->assignProperty(myPlayAction, "icon", thePlayIcon);
	myStoppedState->assignProperty(myPlayAction, "enabled", true);
	myStoppedState->assignProperty(myFFAction,   "enabled", false);
	myStoppedState->assignProperty(myResetAction,"enabled", false);
	myStoppedState->assignProperty(myLabelPtr,   "pixmap",  theStopStatusPixmap);
	// upon entering running state, FF=enable, P=pause     R=disabled
	myRunningState->assignProperty(myPlayAction, "icon", thePauseIcon);
	myRunningState->assignProperty(myPlayAction, "enabled", true);
	myRunningState->assignProperty(myFFAction,   "enabled", true);
	myRunningState->assignProperty(myResetAction,"enabled", false);
	myRunningState->assignProperty(myLabelPtr,   "pixmap",  thePlayStatusPixmap);
	// upon entering paused  state, FF=disable, P=play     R=enabled
	myPausedState ->assignProperty(myPlayAction, "icon", thePlayIcon);
	myPausedState ->assignProperty(myPlayAction, "enabled", true);
	myPausedState ->assignProperty(myFFAction,   "enabled", false);
	myPausedState ->assignProperty(myResetAction,"enabled", true);
	myPausedState->assignProperty(myLabelPtr,   "pixmap",  thePauseStatusPixmap);
	// upon entering forward state, FF=enable, P=play      R=disabled
	myForwardState->assignProperty(myPlayAction, "icon", thePlayIcon);
	myForwardState->assignProperty(myPlayAction, "enabled", true);
	myForwardState->assignProperty(myFFAction,   "enabled", false);
	myForwardState->assignProperty(myResetAction,"enabled", false);
	myForwardState->assignProperty(myLabelPtr,   "pixmap",  theFFStatusPixmap);
	// upon entering failed  state, FF=disable, P=disable, R=enabled
	myFailedState ->assignProperty(myPlayAction, "icon", theStopIcon);
	myFailedState ->assignProperty(myPlayAction, "enabled", false);
	myFailedState ->assignProperty(myFFAction,   "enabled", true);
	myFailedState ->assignProperty(myResetAction,"enabled", false);
	myFailedState->assignProperty(myLabelPtr,   "pixmap",  theFailStatusPixmap);

	// TODO: hook up states to signals for ViewWorld/World

	emit theSimStateMachine.start();
}
