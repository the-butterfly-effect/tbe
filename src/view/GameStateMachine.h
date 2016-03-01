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

#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include <QObject>
#include <QState>
#include <QStateMachine>
#include <QTimer>

/// This direct QState derivative was created to have meaningful
/// debug messages when SimulationControls switches state
class GameState : public QState
{
    Q_OBJECT
public:
    explicit GameState(QState* parent = 0, const QString& aStateName = "")
        : QState(parent), isActive(false), theName(aStateName) {}

    virtual ~GameState()
    {}

    bool isActive;

signals:
    /// emitted onEntry
    /// @param aPtr will contain the this pointer for the GameState that is
    ///        activated.
    void activated(GameState* aPtr);

protected:
    virtual void onEntry ( QEvent * event );
    virtual void onExit  ( QEvent * event );

private:
    QString theName;
};


class GameStateMachine : public QObject
{
    Q_OBJECT
public:
    explicit GameStateMachine(QObject *parent = 0);

    /// The states that the GameStateMachine can be in.
    /// @note That for some superstates (like running) all sub-states are in
    ///       this list instead, where for other states (like Won), only the
    ///       superstate is present.
    /// @note There is a one-to-one connection between these states and the
    ///       image that is shown by GameControls.
    enum States
    {
        FailedStatus,
        ForwardStatus,
        NormalStatus,
        PausedStatus,
        ProblemStatus,
        RealFastStatus,
        SlowStatus,
        StoppedStatus,
        WonStatus
    };

signals:
    /// retransmitted signal: that the simulation failed.
    void signal_Fail_happened();
    /// retransmitted signal: user triggered the Fast Forward action.
    void signal_Forward_triggered();
    /// retransmitted signal: user triggered the Pause action.
    void signal_Pause_triggered();
    /// retransmitted signal: user triggered the Play action.
    void signal_Play_triggered();
    /// retransmitted signal: user triggered the Really Fast forward action.
    void signal_RealFast_triggered();
    /// retransmitted signal: user triggered the Reset action.
    void signal_Reset_triggered();
    /// retransmitted signal: user triggered the Slow action.
    void signal_Slow_triggered();
    /// retransmitted signal: the simulation has reached all Won conditions.
    /// @note This signal is for inside use only and may be emitted dozens of
    ///       times.
    ///       Outside of GameStatemachine should not connect to this signal,
    ///       please connect to signal_Game_Is_Won instead.
    void signal_Won_happened();

    /// Signal indicates that all crosses have disappeared.
    void signal_Problems_solved();
    /// Signal indicates that one of the objects now has a cross decorator.
    void signal_Problems_arised();

    void signal_Stop_Gameplay();

    /// Emitted whenever the game is won. E.g. ResizingGraphicsView should
    /// connect to this signal to know when to throw the "Won" dialog.
    /// @note Signal should be emitted only once.
    void signal_Game_Is_Won();

    /// Emitted whenever the game has failed. E.g. ResizingGraphicsView should
    /// connect to this signal to know when to throw the "Failed" dialog.
    /// @note Signal should be emitted only once.
    void signal_Game_Failed();

    /// Emitted whenever the game state changes.
    /// The slot for this signal is mostly GameControls::slot_updateIcon().
    void signal_State_Changed(States aNewState);

public slots:
    /// Resizinggraphicsview connects CrossRegisterSingleton to this signal
    /// and it will be called whenever the user has a cross on one of his
    /// decorators - or removes one.
    void slot_NumberOfCrossesChanged(int aNewNumber);

private slots:
    /// On entry of WonRunningSubState, start a timer to stop simulation and
    /// transition to WonPausedSubState;
    void slot_SetWonRunningTimeout();

    /// Called whenever a state changes.
    void slot_State_Activated(GameState* aPtr);

private:
    QStateMachine theGameStateMachine;

    /// When a simulation fails, enter this state.
    /// Only exitable through 'reset'.
    GameState* theFailedState;

    /// When the user creates an illegal state (e.g. overlapping objects),
    /// enter this state. Game cannot be started until problem(s) resolved.
    GameState* theProblemState;

    /// State when simulation is ongoing.
    GameState* theRunningState;
    GameState* theRunningForwardSubState;
    GameState* theRunningNormalSubState;
    GameState* theRunningPausedSubState;
    GameState* theRunningRealFastSubState;
    GameState* theRunningSlowSubState;

    /// State when simulation is halted and the user can make changes to
    /// the scene. TODO: figure out if this is the parent of theProblemState.
    /// Hitting any simulationcontrol will trigger switch to theRunningState.
    GameState* theStoppedState;

    /// State when simulation has been won.
    /// The simulation will continue for a few seconds (RunningSubState) and
    /// then stop (PausedSubState).
    GameState* theWonState;
    GameState* theWonPausedSubState;
    GameState* theWonRunningSubState;

    /// Timer is used to time the WonRunningSubState and transition to
    /// WonPausedSubState.
    QTimer theWonRunningTimer;
};

#endif // GAMESTATEMACHINE_H
