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

#ifndef GAMECONTROLS_H
#define GAMECONTROLS_H

#include <QVector>
#include <QLabel>
#include <QIcon>
#include <QAction>
#include <QActionGroup>
#include "tbe_global.h"

#include "GameStateMachine.h"

namespace Ui {
class GameControls;
}

// forward declarations:
//class MainWindow;
class QMenu;


/** This state machine controls how the simulation runs
  * inputs are the QActions that it creates in the menu bar and slots on its interface
  * outputs are the signals that it sends when states change
  */
class GameControls : public QLabel
{
    Q_OBJECT


public:
    explicit GameControls(QWidget *parent = 0);

    virtual ~GameControls();

    void setup(QMenu *aMenuPtr);

    void parentResize(const QSize &aSize);

//    void hookSignalsUp(ViewWorld* aViewWorld, ResizingGraphicsView *aRGVPtr);

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

private:
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

    QVector<QPixmap> thePixmaps;

    Ui::GameControls *ui;
};

#endif // GAMECONTROLS_H
