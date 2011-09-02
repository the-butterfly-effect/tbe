#include "tbe_global.h"
#include "SimulationControls.h"
#include "ImageStore.h"

#include <QMenuBar>
#include <QPropertyAnimation>
#include <QSignalTransition>

#include <cstdio>

void SimState::onEntry ( QEvent * event )
{
	printf("state %s onEntry!\n", ASCII(theName));
	QState::onEntry(event);
}


SimulationControls::SimulationControls(QObject *parent) :
    QObject(parent)
{
	thePlayIcon  = ImageRendererStore::getQIcon("ActionMenuPlay", QSize(32,32));
	thePauseIcon = ImageRendererStore::getQIcon("ActionMenuPause", QSize(32,32));
	theStopIcon  = ImageRendererStore::getQIcon("ActionMenuStop", QSize(32,32));
	theResetIcon = ImageRendererStore::getQIcon("ActionUndo", QSize(32,32));
	theFwdIcon   = ImageRendererStore::getQIcon("ActionFastForward", QSize(32,32));
}

void SimulationControls::setup(QMenuBar* aMenuBarPtr)
{
	QState* theFailedState  = new SimState(&theSimStateMachine, "Failed");
	QState* theForwardState = new SimState(&theSimStateMachine, "Forward");
	QState* thePausedState  = new SimState(&theSimStateMachine, "Paused");
	QState* theRunningState = new SimState(&theSimStateMachine, "Running");
	QState* theStoppedState = new SimState(&theSimStateMachine, "Stopped");
	theSimStateMachine.setInitialState(theStoppedState);
	theSimStateMachine.setErrorState(theFailedState);

	// add actions
	QAction* myPlayAction = new QAction(thePlayIcon, "&Play", NULL);
	QAction* myResetAction = new QAction(theResetIcon, "&Reset", NULL);
//	myResetAction->setEnabled(false);
	QAction* myFFAction = new QAction(theFwdIcon, "F&ast", NULL);
//	myFFAction->setEnabled(false);
	aMenuBarPtr->addAction(myPlayAction);
	aMenuBarPtr->addAction(myFFAction);
	aMenuBarPtr->addAction(myResetAction);

	// add transitions here
	theStoppedState->addTransition(myPlayAction,SIGNAL(triggered()),theRunningState);
	theRunningState->addTransition(myPlayAction, SIGNAL(triggered()), thePausedState);
	theRunningState->addTransition(myFFAction, SIGNAL(triggered()), theForwardState);
	theRunningState->addTransition(this, SIGNAL(failed()), theFailedState);
	thePausedState->addTransition(myPlayAction, SIGNAL(triggered()), theRunningState);
	thePausedState->addTransition(myResetAction, SIGNAL(triggered()), theStoppedState);
	theForwardState->addTransition(myFFAction, SIGNAL(triggered()), theRunningState);
	theForwardState->addTransition(myPlayAction, SIGNAL(triggered()), theRunningState);
	theForwardState->addTransition(this, SIGNAL(failed()), theFailedState);

	// set the start conditions for the icons for each state
	// upon entering stopped state, FF=disable, P=play     R=disabled
	theStoppedState->assignProperty(myPlayAction, "icon", thePlayIcon);
	theStoppedState->assignProperty(myFFAction,   "enabled", false);
	theStoppedState->assignProperty(myResetAction,"enabled", false);
	// upon entering running state, FF=enable, P=pause     R=disabled
	theRunningState->assignProperty(myPlayAction, "icon", thePauseIcon);
	theRunningState->assignProperty(myFFAction,   "enabled", true);
	theRunningState->assignProperty(myResetAction,"enabled", false);
	// upon entering paused  state, FF=disable, P=play     R=enabled
	thePausedState ->assignProperty(myPlayAction, "icon", thePlayIcon);
	thePausedState ->assignProperty(myFFAction,   "enabled", false);
	thePausedState ->assignProperty(myResetAction,"enabled", true);
	// upon entering forward state, FF=enable, P=play      R=disabled
	theForwardState->assignProperty(myPlayAction, "icon", thePlayIcon);
	theForwardState->assignProperty(myFFAction,   "enabled", false);
	theForwardState->assignProperty(myResetAction,"enabled", false);
	// upon entering failed  state, FF=disable, P=disable, R=enabled
	theFailedState ->assignProperty(myPlayAction, "icon", thePauseIcon);
	theFailedState ->assignProperty(myFFAction,   "enabled", true);
	theFailedState ->assignProperty(myResetAction,"enabled", false);

	// for debug purposes...
/*	connect(&theStateMachine, SIGNAL(started()), this, SLOT(stateEntered()));
	connect(theForwardState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(thePausedState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theRunningState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theStoppedState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theFailedState, SIGNAL(entered()),this,SLOT(stateEntered()));
*/
	emit stateEntered();
	QTimer::singleShot(1000, this, SLOT(stateEntered()));
	emit theSimStateMachine.start();
	printf("do we ever return?\n");
	emit stateEntered();
}


void SimulationControls::stateEntered(void)
{
	printf("state XXX entered\n");
	printf("error: %d\n", theSimStateMachine.error());
	printf("isrun: %d\n", theSimStateMachine.isRunning());
}
