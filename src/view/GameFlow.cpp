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

#include "GameFlow.h"
#include "Level.h"
#include "MainWindow.h"
#include "RequestDialog.h"
#include "tbe_global.h"

#include <QQuickItem>

GameFlow::GameFlow(MainWindow *parent, RequestDialog* anRDPtr)
    : QObject(parent),
      theGameStateMachinePtr(nullptr),
      theMainWindowPtr(parent),
      theNewWinFailDialogPtr(nullptr),
      theRequestDialogItfPtr(anRDPtr)
{
    theGameStateMachinePtr = new GameStateMachine(this);
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Is_Won()), this, SLOT(slot_levelWon()));
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Failed()), this, SLOT(slot_levelDeath()));

//    connect (this, SIGNAL(signal_actionChooseLevel()), theMainWindowPtr,
//             SLOT(on_action_Open_Level_triggered()));
//    connect (this, SIGNAL(signal_actionNextLevel()),   theMainWindowPtr, SLOT(slot_actionNextLevel()));
//    connect (this, SIGNAL(signal_actionReplay()),      theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));
//    connect (this, SIGNAL(signal_actionSkipLevel()),   theMainWindowPtr,
//             SLOT(on_action_Skip_Level_triggered()));

}


void GameFlow::setupWinFail(bool isAWin)
{
    theNewWinFailDialogPtr = theRequestDialogItfPtr->showWinFail(isAWin);
    connect(theNewWinFailDialogPtr, SIGNAL(chooseButton_clicked()), theMainWindowPtr, SLOT(on_action_Open_Level_triggered()));
    connect(theNewWinFailDialogPtr, SIGNAL(nextButton_clicked()),   theMainWindowPtr, SLOT(slot_actionNextLevel()));
    connect(theNewWinFailDialogPtr, SIGNAL(replayButton_clicked()), theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));
    connect(theNewWinFailDialogPtr, SIGNAL(skipButton_clicked()),   theMainWindowPtr, SLOT(on_action_Skip_Level_triggered()));
}


void GameFlow::slot_clearWinFailDialogPtr()
{
    theNewWinFailDialogPtr = nullptr;
}


void GameFlow::slot_levelDeath(void)
{
    DEBUG3ENTRY;
    setupWinFail(false);
}

void GameFlow::slot_levelWon(void)
{
    DEBUG1ENTRY;

    // Anti-cheat:
    // Don't label the level as complete when we're in level editor mode
    if (!theIsLevelCreator) {
        Level::setLevelStatus(Level::COMPLETED);
    }
    setupWinFail(true);
}


void GameFlow::slot_showGameResourcesDialog()
{
//    QTimer::singleShot(50, theGameResourcesPtr, SLOT(appearAnimated()));
}
