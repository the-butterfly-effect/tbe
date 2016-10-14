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
      theDialogPtr(nullptr),
      theMainWindowPtr(parent),
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
    theDialogPtr = theRequestDialogItfPtr->showWinFail(isAWin);
    connect(theDialogPtr, SIGNAL(chooseButton_clicked()), theMainWindowPtr, SLOT(on_action_Open_Level_triggered()));
    connect(theDialogPtr, SIGNAL(nextButton_clicked()),   theMainWindowPtr, SLOT(slot_actionNextLevel()));
    connect(theDialogPtr, SIGNAL(replayButton_clicked()), theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));
    connect(theDialogPtr, SIGNAL(skipButton_clicked()),   theMainWindowPtr, SLOT(on_action_Skip_Level_triggered()));
}


void GameFlow::slot_clearDialog()
{
    theDialogPtr->deleteLater();
    theDialogPtr = nullptr;
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


void GameFlow::slot_showLevelInfoDialog()
{
    if (theDialogPtr)
        slot_clearDialog();

    theDialogPtr = theRequestDialogItfPtr->showLevelInfo();
    theDialogPtr->setProperty("levelName", theMainWindowPtr->theLevelPtr->theLevelName);
    theDialogPtr->setProperty("description", theMainWindowPtr->theLevelPtr->theLevelDescription);
    theDialogPtr->setProperty("author", theMainWindowPtr->theLevelPtr->theLevelAuthor);

    connect(theDialogPtr, SIGNAL(resetButton_clicked()),
            theMainWindowPtr, SLOT(reloadLevel()));
    connect(theDialogPtr, SIGNAL(okButton_clicked()),
            this, SLOT(slot_clearDialog()));
}