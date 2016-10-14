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
#include "LevelList.h"
#include "MainWindow.h"
#include "RequestDialog.h"
#include "Translator.h"
#include "tbe_global.h"
#include "tbe_paths.h"

#include <QQuickItem>
#include <QQmlContext>


ListRow::ListRow(const QString &aNumber, const QString &aTitle, const QString& aFileName)
    : QObject(nullptr),
      theNumber(aNumber), theTitle(aTitle), theFileName(aFileName)
{
}


GameFlow::GameFlow(MainWindow *parent, RequestDialog* anRDPtr)
    : QObject(parent),
      theGameStateMachinePtr(nullptr),
      theDialogPtr(nullptr),
      theMainWindowPtr(parent),
      theRequestDialogItfPtr(anRDPtr),
      theLevelList(new LevelList(LEVELS_DIRECTORY, "levels.xml"))
{
    theGameStateMachinePtr = new GameStateMachine(this);
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Is_Won()), this, SLOT(slot_levelWon()));
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Failed()), this, SLOT(slot_levelDeath()));
}


void GameFlow::generateLevelList()
{
    theLevelStringList.clear();
//    bool isFirst = true;
//    bool hasSkipped = false;
    QString myNextName = theLevelList->getFirstLevel();
    int myNr = 0;
    do {
        ++myNr;
        LevelList::LevelMetaInfo myMeta = theLevelList->getLevelMetaInfo(myNextName);
        ListRow* myRowPtr = new ListRow("", TheGetText(myMeta.theTitle), myMeta.theFileName);
//      item->setToolTip(TITLE_COLUMN, TheGetText(myMeta.theDescription));
        switch (myMeta.theStatus) {
        case Level::COMPLETED:
            myRowPtr->theNumber = tr("done");
            break;
        case Level::SKIPPED:
            myRowPtr->theNumber = tr("skipped");
//            if (!hasSkipped) {
//                hasSkipped = true;
//                firstSkipped = item;
//            }
            break;
        default:
            myRowPtr->theNumber = QString::number(myNr);
//            if (isFirst)
//                m_ui->theTreeWidget->setCurrentItem(item);
//            isFirst = false;
            break;
        }

        theLevelStringList.append(myRowPtr);
        myNextName = theLevelList->getNextLevel(myNextName);
    } while (!myNextName.isEmpty() );
}


QString GameFlow::getNextLevelName()
{
    return theLevelList->getNextToPlayLevel();
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

void GameFlow::slot_showChooseLevelDialog()
{
    if (theDialogPtr)
        slot_clearDialog();

    generateLevelList();
    theRequestDialogItfPtr->setContextProperty("theLevelList", QVariant::fromValue(theLevelStringList));
    theDialogPtr = theRequestDialogItfPtr->showChooseLevel();
    connect(theDialogPtr, SIGNAL(cancelButton_clicked()),
            this, SLOT(slot_clearDialog()));
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
