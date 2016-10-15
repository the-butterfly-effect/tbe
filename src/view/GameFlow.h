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

class MainWindow;
class QQuickItem;
class RequestDialog;


class GameFlow : public QObject
{
    Q_OBJECT

public:
    explicit GameFlow(MainWindow *parent, RequestDialog *anRDPtr);

    GameStateMachine* theGameStateMachinePtr;

    /// TODO/FIXME: parked here, called only from MainWindow
    /// @returns The name of the next level to play, or "" if no more levels.
    QString getNextLevelName(void);


signals:

private slots:
    void slot_levelDeath(void);
    void slot_levelWon(void);

    void slot_onLevelIndexSelected(const QVariant& anIndex);


public slots:
    void slot_clearDialog();
    void slot_showLevelInfoDialog();
    void slot_showChooseLevelDialog();

private:

    /// Initialise the WinFailDialog.
    void setupWinFail(bool isAWin);

    QQuickItem* theDialogPtr;
    MainWindow* theMainWindowPtr;
    RequestDialog* theRequestDialogItfPtr;

    LevelList theLevelList;
};

#endif // GAMEFLOW_H
