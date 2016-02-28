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

#ifndef GAMECONTROLS_H
#define GAMECONTROLS_H

#include <QObject>
#include <QState>
#include <QStateMachine>


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

protected:
    virtual void onEntry ( QEvent * event );
    virtual void onExit  ( QEvent * event );

private:
    QString theName;
};


class GameControls : public QObject
{
    Q_OBJECT
public:
    explicit GameControls(QObject *parent = 0);

signals:

public slots:
    void slot_Play_triggered();
    void slot_Pause_triggered();
    void slot_Forward_triggered();
    void slot_RealFast_triggered();
    void slot_Slow_triggered();

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
};

#endif // GAMECONTROLS_H
