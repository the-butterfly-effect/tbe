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

#ifndef GAMEFLOW_H
#define GAMEFLOW_H

#include "GameStateMachine.h"
#include "LevelList.h"
#include <QObject>
#include <QTime>
#include <QTimer>

class MainWindow;
class QAction;
class QMenuBar;
class QQuickItem;
class RequestDialog;
class World;

class GameFlow : public QObject
{
    Q_OBJECT

public:
    explicit GameFlow(MainWindow *parent, QMenuBar* aMenuBarPtr, RequestDialog *anRDPtr);

    GameStateMachine* theGameStateMachinePtr;

    /// @returns true if the simulation is not in 'stopped' mode.
    static bool getIsSimRunning();

    /// TODO/FIXME: parked here, called only from MainWindow
    /// @returns The name of the next level to play, or "" if no more levels.
    QString getNextLevelName(void);

    /// Constant defining the goal FPS (frames per second)
    /// @note: This does not say that the game achieves this number.
    ///        It however, will mean that the game will *attempt* not to render
    ///        more frames than this per second. No guarantees.
    static const int MAX_FPS;

    void setWorldPtr(World* aWorldPtr);

signals:

private slots:
    void slot_levelDeath(void);
    void slot_levelWon(void);

    void slot_onLevelIndexSelected(const QVariant& anIndex);

    void slot_InsertionDisallowed(bool isInsertionDisallowed);

private slots:
    /// called whenever a timer tick happens
    void on_timerTick(void);

    /// called whenever the framerate timer tick happens
    void on_framerateTimerTick(void);

public slots:
    void slot_clearDialog();
    void slot_makeAllDialogsDisappear();
    void slot_showLevelInfoDialog();
    void slot_showChooseLevelDialog();
    void slot_showChoosePhoneNumberDialog();

    /// To be called when the cursor changed position in the graphicsview,
    /// in order to display the coordinates.
    /// x and y hold the position of the cursor in world coordinates
    void slot_mouse_move(qreal x, qreal y);

public slots:
    // signals to start/stop/ffwd/reset the game
    void slot_signal4F();
    void slot_signalFF();
    void slot_signalPause();
    void slot_signalPlay();
    void slot_signalReset();
    void slot_signalSlow();

private:

    /// Initialise the WinFailDialog.
    void setupWinFail(bool isAWin);

    QQuickItem* theDialogPtr;
    MainWindow* theMainWindowPtr;
    RequestDialog* theRequestDialogItfPtr;
    World*      theWorldPtr;

    LevelList theLevelList;

    QTimer theTimer;
    QTimer theFramerateTimer;
    QTime  theSimulationTime;
    QTime  theGameStopwatch;

    /// current number of milliseconds per time step
    /// (note that TBE is configured to run at "half of reality" speed)
    qreal theSimSpeed;

    unsigned int theFramesPerSecond;

    QAction *theFrameRateViewPtr;
};

#endif // GAMEFLOW_H
