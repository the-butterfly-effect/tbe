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

#include <QLabel>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include "tbe_global.h"
#include "ViewWorld.h"

namespace Ui {
class GameControls;
}

// forward declarations:
class MainWindow;


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

    void setup(QMenu* aMenuPtr);

    void parentResize(const QSize& aSize);

//    void hookSignalsUp(ViewWorld* aViewWorld, ResizingGraphicsView *aRGVPtr);

signals:

public slots:

private:
    QAction* theForwardAction;
    QAction* thePlayAction;
    QAction* thePauseAction;
    QAction* theRealFastAction;
    QAction* theResetAction;
    QAction* theSlowAction;

    QIcon theForwardIcon;
    QIcon thePauseIcon;
    QIcon thePlayIcon;
    QIcon theResetIcon;

    QPixmap the4FStatusPixmap;
    QPixmap theForwardStatusPixmap;
    QPixmap theFailedStatusPixmap;
    QPixmap thePausedStatusPixmap;
    QPixmap theProblemStatusPixmap;
    QPixmap theRunningStatusPixmap;
    QPixmap theStoppedStatusPixmap;

    Ui::GameControls *ui;
};

#endif // GAMECONTROLS_H
