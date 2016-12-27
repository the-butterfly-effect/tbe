/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2016 Klaas van Gend
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

#ifndef GAMEQCONTROLS_H
#define GAMEQCONTROLS_H

#include <QVector>
#include <QLabel>
#include <QAction>
#include <QActionGroup>
#include <QQuickItem>
#include "tbe_global.h"

#include "GameStateMachine.h"

// forward declarations:
class QMenu;


/** This is the UI of the controls how the simulation runs.
  * It creates inputs as QtC++ QActions for the menu bar and slots on its interface.
  * Outputs are the signals that it sends when states change.
  *
  * @note: TODO: This class needs to be revisited once we move the Menu structure to QML.
  */
class GameQControls : public QQuickItem
{
    Q_OBJECT

public:
    explicit GameQControls(QQuickItem *aParentPtr = Q_NULLPTR);

    virtual ~GameQControls();

    void setup(QMenu *aMenuPtr);

    /// TODO: REMOVE public accessor
    static GameQControls* me();

signals:
    /// User triggered the Fast Forward action.
    void signal_Forward_triggered();
    /// User triggered the Pause action.
    void signal_Pause_triggered();
    /// User triggered the Play action.
    void signal_Play_triggered();
    /// User triggered the Really Fast forward action.
    void signal_RealFast_triggered();
    /// User triggered the Reset action.
    void signal_Reset_triggered();
    /// User triggered the Slow action.
    void signal_Slow_triggered();

public slots:
    void slot_updateIcon(GameStateMachine::States aStatus);

protected:
    Q_INVOKABLE void button_forward_clicked();
    Q_INVOKABLE void button_pause_clicked();
    Q_INVOKABLE void button_play_clicked();
    Q_INVOKABLE void button_reset_clicked();

private:
    void updateIcon(int aNumber);

    QActionGroup theGameButtonGroup;
    QAction *theForwardAction;
    QAction *thePauseAction;
    QAction *thePlayAction;
    QAction *theRealFastAction;
    QAction *theResetAction;
    QAction *theSlowAction;

    QIcon theForwardIcon;
    QIcon thePauseIcon;
    QIcon thePlayIcon;
    QIcon theResetIcon;
};

#endif // GAMEQCONTROLS_H
