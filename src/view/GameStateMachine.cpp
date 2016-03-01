/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

#include "GameStateMachine.h"
#include "tbe_global.h"

///---------------------------------------------------------------------------
///------------------------ GameState ----------------------------------------
///---------------------------------------------------------------------------

void GameState::onEntry ( QEvent * event )
{
    DEBUG4("GameControls-GameState %s onEntry!", ASCII(theName));
    QState::onEntry(event);
    isActive = true;
}

void GameState::onExit ( QEvent * event )
{
    QState::onExit(event);
    isActive = false;
}



///---------------------------------------------------------------------------
///------------------------ GameStateMachine ---------------------------------
///---------------------------------------------------------------------------

GameStateMachine::GameStateMachine(QObject *parent) :
    QObject(parent)
{
    // ****************************************************
    // initialize all states and substates
    theFailedState             = new GameState(&theGameStateMachine, "FailedState");

    theProblemState            = new GameState(&theGameStateMachine, "ProblemState");

    theRunningState            = new GameState(&theGameStateMachine, "RunningState");
    theRunningForwardSubState  = new GameState( theRunningState,      "RunningForwardSubState");
    theRunningNormalSubState   = new GameState( theRunningState,      "RunningNormalSubState");
    theRunningPausedSubState   = new GameState( theRunningState,      "RunningPausedSubState");
    theRunningRealFastSubState = new GameState( theRunningState,      "RunningRealFastSubState");
    theRunningSlowSubState     = new GameState( theRunningState,      "RunningSlowSubState");
    theRunningState->setInitialState(theRunningNormalSubState);

    theStoppedState            = new GameState(&theGameStateMachine, "StoppedState");

    theWonState                = new GameState(&theGameStateMachine, "WonState");
    theWonPausedSubState       = new GameState( theWonState,          "WonPausedSubState");
    theWonRunningSubState      = new GameState( theWonState,          "WonRunningSubState");
    theWonState->setInitialState(theWonRunningSubState);

    theGameStateMachine.setInitialState(theStoppedState);

    // ****************************************************
    // setup all state transitions

    theFailedState->addTransition(this, SIGNAL(signal_Reset_triggered()), theStoppedState);
    connect(theFailedState, SIGNAL(entered()), this, SIGNAL(signal_Stop_Gameplay()));
    connect(theFailedState, SIGNAL(entered()), this, SIGNAL(signal_Game_Failed()));

    theProblemState->addTransition(this, SIGNAL(signal_Problems_solved()), theStoppedState);

    theRunningState->addTransition(this, SIGNAL(signal_Fail_happened()), theFailedState);
    theRunningState->addTransition(this, SIGNAL(signal_Won_happened()), theWonState);
    theRunningState->addTransition(this, SIGNAL(signal_Reset_triggered()), theStoppedState);

    //theRunningForwardSubState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    theRunningForwardSubState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    theRunningForwardSubState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    theRunningForwardSubState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    theRunningForwardSubState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);

    theRunningNormalSubState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    theRunningNormalSubState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    //theRunningNormalSubState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    theRunningNormalSubState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    theRunningNormalSubState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);

    theRunningPausedSubState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    //theRunningPausedSubState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    theRunningPausedSubState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    theRunningPausedSubState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    theRunningPausedSubState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);

    theRunningRealFastSubState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    theRunningRealFastSubState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    theRunningRealFastSubState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    //theRunningRealFastSubState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    theRunningRealFastSubState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);

    theRunningSlowSubState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    theRunningSlowSubState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    theRunningSlowSubState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    theRunningSlowSubState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    //theRunningSlowSubState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);

    theStoppedState->addTransition(this, SIGNAL(signal_Problems_arised()), theProblemState);
    // TODO: figure out if immediately jump to substate or to master & retransmit
    theStoppedState->addTransition(this, SIGNAL(signal_Forward_triggered()), theRunningForwardSubState);
    theStoppedState->addTransition(this, SIGNAL(signal_Pause_triggered()), theRunningPausedSubState);
    theStoppedState->addTransition(this, SIGNAL(signal_Play_triggered()), theRunningNormalSubState);
    theStoppedState->addTransition(this, SIGNAL(signal_RealFast_triggered()), theRunningRealFastSubState);
    theStoppedState->addTransition(this, SIGNAL(signal_Slow_triggered()), theRunningSlowSubState);
    connect(theStoppedState, SIGNAL(entered()), this, SIGNAL(signal_Stop_Gameplay()));

    theWonState->addTransition(this, SIGNAL(signal_Reset_triggered()), theStoppedState);
    theWonRunningSubState->addTransition(this, SIGNAL(todo), theWonPausedSubState);
    connect(theWonState, SIGNAL(entered()), this, SLOT(slot_SetWonRunningTimeout()));
    connect(theWonRunningSubState, SIGNAL(entered()), this, SIGNAL(signal_Game_Is_Won()));
    theWonPausedSubState->addTransition(this, SIGNAL(signal_Reset_triggered()), theStoppedState);

    emit theGameStateMachine.start();
}


void GameStateMachine::slot_NumberOfCrossesChanged(int aNewNumber)
{
    if (aNewNumber > 0)
        emit signal_Problems_arised();
    else
        emit signal_Problems_solved();
}

void GameStateMachine::slot_SetWonRunningTimeout()
{
    theWonRunningTimer.setSingleShot(true);
    /// This number represents how many miliseconds the simulation keeps
    /// running after the level has been won.
    /// TODO/FIXME: magic number here!!!
    theWonRunningTimer.start(3000);
}
