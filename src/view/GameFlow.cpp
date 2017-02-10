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
#include "tbe_global.h"
#include "ViewWorldItem.h"
#include "tbe_paths.h"
#include "World.h"

#include <QMenuBar>
#include <QQuickItem>
#include <QQmlContext>

static bool isSimRunning = false;

const int GameFlow::MAX_FPS = 60;


GameFlow::GameFlow(MainWindow *parent, QMenuBar* aMenuBarPtr, RequestDialog* anRDPtr)
    : QObject(parent),
      theGameStateMachinePtr(nullptr),
      theDialogPtr(nullptr),
      theMainWindowPtr(parent),
      theRequestDialogItfPtr(anRDPtr),
      theLevelList(LEVELS_DIRECTORY, "levels.xml"),
      theSimSpeed(1000)
{
    theGameStateMachinePtr = new GameStateMachine(this);
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Is_Won()),
             this, SLOT(slot_levelWon()));
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Failed()),
             this, SLOT(slot_levelDeath()));
    connect (theGameStateMachinePtr, SIGNAL(signal_InsertionDisallowed(bool)),
             this, SLOT(slot_InsertionDisallowed(bool)));

    // this one displays the frame rate counter if active
    theFrameRateViewPtr = aMenuBarPtr->addAction("");

    connect(&theFramerateTimer, SIGNAL(timeout()), this, SLOT(on_framerateTimerTick()));
    connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));
    isSimRunning = false;
}

bool GameFlow::getIsSimRunning()
{
    return isSimRunning;
}

QString GameFlow::getNextLevelName()
{
    return theLevelList.getNextToPlayLevel();
}


void GameFlow::on_timerTick()
{
    QTime myCurrentTime = QTime::currentTime();
    // whatever happens, draw every 25 frames
    for (int i = 0; i < 25 && theSimulationTime < myCurrentTime; i++) {
        theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * 2 * theSimSpeed);
    }

    // iterate through all known objects to update the graphics part
    theWorldPtr->updateViewWorld(true);
    theFramesPerSecond++;
}

void GameFlow::on_framerateTimerTick()
{
// Updating the framerate on MacOSX results in no fps on screen, yet we get
// "QMenu: No OSMenuRef created for popup menu" warnings on the console.
// That's why we do not even try to put it on screen on Mac.
#ifndef Q_WS_MAC
    theFrameRateViewPtr->setText(tr("    %1 fps; %2 s").arg(theFramesPerSecond).arg(
                                     theGameStopwatch.elapsed() / 1000));
#endif
    theFramesPerSecond = 0;
}


void GameFlow::setupWinFail(bool isAWin)
{
    theDialogPtr = theRequestDialogItfPtr->showWinFail(isAWin);
    connect(theDialogPtr, SIGNAL(chooseButton_clicked()), theMainWindowPtr, SLOT(on_action_Open_Level_triggered()));
    connect(theDialogPtr, SIGNAL(nextButton_clicked()),   theMainWindowPtr, SLOT(slot_actionNextLevel()));
    connect(theDialogPtr, SIGNAL(replayButton_clicked()), theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));
    connect(theDialogPtr, SIGNAL(skipButton_clicked()),   theMainWindowPtr, SLOT(on_action_Skip_Level_triggered()));
}

void GameFlow::setWorldPtr(World *aWorldPtr)
{
    assert(nullptr != aWorldPtr);
    theWorldPtr = aWorldPtr;
}

void GameFlow::slot_clearDialog()
{
    if (theDialogPtr)
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

void GameFlow::slot_makeAllDialogsDisappear()
{
    // TODO
}

void GameFlow::slot_onLevelIndexSelected(const QVariant& anIndex)
{
    LevelList::LevelMetaInfo myLevelInfo = theLevelList.getLevelMetaInfo(anIndex.toInt());
    slot_clearDialog();
    emit theMainWindowPtr->loadLevel(myLevelInfo.theFileName);
}

void GameFlow::slot_InsertionDisallowed(bool isInsertionDisallowed)
{
    ViewWorldItem::me()->setIsModifyAllowed(!isInsertionDisallowed);
}

void GameFlow::slot_showChooseLevelDialog()
{
    slot_clearDialog();

    RowList& myList = theLevelList.generateLevelList();
    theRequestDialogItfPtr->setContextProperty("theLevelList", QVariant::fromValue(myList));
    theDialogPtr = theRequestDialogItfPtr->showChooseLevel();
    connect(theDialogPtr, SIGNAL(cancelButton_clicked()),
            this, SLOT(slot_clearDialog()));
    QMetaObject::invokeMethod(theDialogPtr, "setActive", Q_ARG(QVariant, theLevelList.getFirstSelectableLevel()));
    connect(theDialogPtr, SIGNAL(goButton_clicked(QVariant)),
            this, SLOT(slot_onLevelIndexSelected(QVariant)));
}

void GameFlow::slot_showChoosePhoneNumberDialog()
{
    QStringList myNumbers;
    myNumbers << "555-1234" << "555-4567";
    theDialogPtr = theRequestDialogItfPtr->showChoosePhoneNumber(myNumbers);
}

void GameFlow::slot_showLevelInfoDialog()
{
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

void GameFlow::slot_signalFF()
{
    if (isSimRunning == false)
        slot_signalPlay();
    theSimSpeed = 250;
    emit theTimer.start();
}


void GameFlow::slot_signal4F()
{
    if (isSimRunning == false)
        slot_signalPlay();
    theSimSpeed = 60;
    emit theTimer.start();
}


void GameFlow::slot_signalPause()
{
    emit theTimer.stop();
    emit theFramerateTimer.stop();
}


void GameFlow::slot_signalPlay()
{
    // remove any dialogs when user starts playing
//    AnimatedDialog::makeAllAnimatedDialogsDisappear();
    slot_clearDialog();

    if (isSimRunning == false)
        theWorldPtr->createPhysicsWorld();
    isSimRunning = true;
    theSimulationTime = QTime::currentTime();
    theSimSpeed = 1000;
    theFramesPerSecond = 0;
    emit theTimer.start(1000 / MAX_FPS);

    if (theDisplayFramerate) {
        // update framerate every second
        theFramesPerSecond = 0;
        theFramerateTimer.start(1000);
        theGameStopwatch.start();
    } else
        theFrameRateViewPtr->setText("");
}


void GameFlow::slot_signalReset()
{
    isSimRunning = false;
    emit theTimer.stop();
    emit theFramerateTimer.stop();
//    if (theDrawDebug)
//        clearGraphicsList(0);
    theWorldPtr->deletePhysicsWorld();
    theWorldPtr->updateViewWorld(false);
}


void GameFlow::slot_signalSlow()
{
    if (isSimRunning == false)
        slot_signalPlay();
    theSimSpeed = 3000;
    emit theTimer.start();
}
