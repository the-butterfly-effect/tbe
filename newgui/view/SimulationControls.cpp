#include "SimulationControls.h"
#include "ImageStore.h"

#include <QMenuBar>
#include <QPropertyAnimation>
#include <QSignalTransition>

#include <cstdio>

SimulationControls::SimulationControls(QObject *parent) :
    QObject(parent)
{
}

void SimulationControls::setup(QMenuBar* aMenuBarPtr)
{
	QState* theFailedState  = new QState(&theStateMachine);
	QState* theForwardState = new QState(&theStateMachine);
	QState* thePausedState  = new QState(&theStateMachine);
	QState* theRunningState = new QState(&theStateMachine);
	QState* theStoppedState = new QState(&theStateMachine);

	// add actions
	QAction* myPlayAction = new QAction(ImageRendererStore::getQIcon("ActionMenuPlay", QSize(32,32)), "&Play", NULL);
	QAction* myResetAction = new QAction(ImageRendererStore::getQIcon("ActionUndo", QSize(32,32)), "&Reset", NULL);
	myResetAction->setEnabled(false);
	QAction* myFFAction = new QAction(ImageRendererStore::getQIcon("ActionFastForward", QSize(32,32)), "F&ast", NULL);
	myFFAction->setEnabled(false);
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

	// TODO:
	// upon entering stopped state, FF=disable, P=play     R=disabled
	// upon entering running state, FF=enable, P=pause     R=disabled
	// upon entering paused  state, FF=disable, P=play     R=enabled
	// upon entering forward state, FF=enable, P=play      R=disabled
	// upon entering failed  state, FF=disable, P=disable, R=enabled
//	QPropertyAnimation* myFFEnableAnimationPtr = new QPropertyAnimation(myFFAction, "enabled");
//	myFFEnableAnimationPtr->setEndValue("true");
//	myT1->addAnimation(myFFEnableAnimationPtr);

//	QPropertyAnimation* myFFDisableAnimationPtr = new QPropertyAnimation(myFFAction, "enabled");
//	myFFEnableAnimationPtr->setEndValue("false");
//	myT1->addAnimation(myFFEnableAnimationPtr);

	// for debug purposes...
	connect(theForwardState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(thePausedState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theRunningState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theStoppedState, SIGNAL(entered()),this,SLOT(stateEntered()));
	connect(theFailedState, SIGNAL(entered()),this,SLOT(stateEntered()));

	theStateMachine.setInitialState(theStoppedState);

	printf("aantal: %d\n", theStateMachine.children().count());
	theStateMachine.start();
}


void SimulationControls::stateEntered(void)
{
	printf("state XXX entered\n");
}
