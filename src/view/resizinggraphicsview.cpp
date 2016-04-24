/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2013 Klaas van Gend
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

#include "ChooseLevel.h"
#include "EditObjectDialog.h"
#include "GameControls.h"
#include "GameResources.h"
#include "Level.h"
#include "LevelCreator.h"
#include "MainWindow.h"
#include "PieMenu.h"
#include "Popup.h"
#include "RegressionTest.h"
#include "resizinggraphicsview.h"
#include "Translator.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "WinFailDialog.h"
#include "World.h"

#include <QMenuBar>
#include <QSettings>

static ResizingGraphicsView* theRSGVPtr = nullptr;


ResizingGraphicsView::ResizingGraphicsView(QWidget *aParentPtr) :
    QGraphicsView(aParentPtr),
    theGameResourcesPtr(nullptr),
    theMainWindowPtr(nullptr),
    theObjectEditorPtr(nullptr),
    theGameStateMachinePtr(nullptr),
    theWinFailDialogPtr(nullptr),
    theFrameRateViewPtr(nullptr)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	setFrameStyle(QFrame::Plain + QFrame::NoFrame);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    theGameControlsPtr = new GameControls(this);
	theGameResourcesPtr = new GameResources(this);
	theRSGVPtr = this;
}


ResizingGraphicsView::~ResizingGraphicsView()
{
    delete theGameControlsPtr;
	theRSGVPtr = nullptr;
}


void ResizingGraphicsView::clearViewWorld(void)
{
	DEBUG3ENTRY;
	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	setScene(nullptr);
	QMatrix myMatrix;
	setMatrix(myMatrix);

    slot_clearWinFailDialogPtr();
    delete theScenePtr;
	theScenePtr = nullptr;
}


GameResources* ResizingGraphicsView::getGameResourcesDialogPtr()
{
	Q_ASSERT(theGameResourcesPtr != nullptr);
    delete 	theGameResourcesPtr;
    theGameResourcesPtr = new GameResources(this);
	return theGameResourcesPtr;
}


ResizingGraphicsView* ResizingGraphicsView::me(void)
{
	Q_ASSERT(theRSGVPtr!=nullptr);
	return theRSGVPtr;
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
	if (event!=nullptr)
		QGraphicsView::resizeEvent(event);
	fitInView(sceneRect(), Qt::KeepAspectRatio);
    theGameControlsPtr->parentResize(frameSize());
	PieMenuSingleton::setViewInSceneCoords(mapToScene(rect()));
}

void ResizingGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if (event!=nullptr) {
		QGraphicsView::mouseMoveEvent(event);

		QPointF mousePos = this->mapToScene(event->pos());
		Position p = Position(mousePos, 0);
		emit theMainWindowPtr->slot_mouse_move(p.x, p.y);
        }
}

void ResizingGraphicsView::setup(MainWindow* aMWPtr, GameStateMachine* aGSMPtr, QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr)
{
	theMainWindowPtr = aMWPtr;
    theGameControlsPtr->setup(anMenuControlsPtr);
    connect(CrossRegisterSingleton::me(), SIGNAL(signalNumberCrossesChanged(int)), aGSMPtr, SLOT(slot_NumberOfCrossesChanged(int)));

    connect (aGSMPtr, SIGNAL(signal_State_Changed(GameStateMachine::States)),
             theGameControlsPtr, SLOT(slot_updateIcon(GameStateMachine::States)));

    theGameStateMachinePtr = aGSMPtr;
    connect (theGameControlsPtr, SIGNAL(signal_Forward_triggered()),  aGSMPtr, SIGNAL(signal_Forward_triggered()));
    connect (theGameControlsPtr, SIGNAL(signal_Pause_triggered()),    aGSMPtr, SIGNAL(signal_Pause_triggered()));
    connect (theGameControlsPtr, SIGNAL(signal_Play_triggered()),     aGSMPtr, SIGNAL(signal_Play_triggered()));
    connect (theGameControlsPtr, SIGNAL(signal_RealFast_triggered()), aGSMPtr, SIGNAL(signal_RealFast_triggered()));
    connect (theGameControlsPtr, SIGNAL(signal_Reset_triggered()),    aGSMPtr, SIGNAL(signal_Reset_triggered()));
    connect (theGameControlsPtr, SIGNAL(signal_Slow_triggered()),     aGSMPtr, SIGNAL(signal_Slow_triggered()));
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Is_Won()),    this,    SLOT(slot_levelWon()));
    connect (theGameStateMachinePtr, SIGNAL(signal_Game_Failed()),    this,    SLOT(slot_levelDeath()));

    connect (this, SIGNAL(signal_actionChooseLevel()), theMainWindowPtr, SLOT(on_action_Open_Level_triggered()));
    connect (this, SIGNAL(signal_actionNextLevel()),   theMainWindowPtr, SLOT(slot_actionNextLevel()));
    connect (this, SIGNAL(signal_actionReplay()),      aGSMPtr, SIGNAL(signal_Reset_triggered()));
    connect (this, SIGNAL(signal_actionSkipLevel()),   theMainWindowPtr, SLOT(on_action_Skip_Level_triggered()));

    // this one displays the frame rate counter if active
	theFrameRateViewPtr= aMenuBarPtr->addAction("");
}


void ResizingGraphicsView::setViewWorld(ViewWorld* aScenePtr,
                                        const QString& aLevelName)
{
    DEBUG1("ResizingGraphicsView::setViewWorld(%p, \"%s\")", aScenePtr,
               ASCII(aLevelName));
	theScenePtr=aScenePtr;

	setScene(aScenePtr);
	fitInView(0, -aScenePtr->getHeight(),
			  aScenePtr->getWidth(), aScenePtr->getHeight());
	resizeEvent(nullptr);
	theMainWindowPtr->setWindowTitle(APPNAME + " - " + TheGetText(aLevelName));

    connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()),   theGameStateMachinePtr, SIGNAL(signal_Won_happened()));
    connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), theGameStateMachinePtr, SIGNAL(signal_Fail_happened()));

	if (theIsRunAsRegression)
	{
		connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), theMainWindowPtr->theRegressionTest, SLOT(slot_Won()));
		connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), theMainWindowPtr->theRegressionTest, SLOT(slot_Fail()));
	}

    connect(theGameStateMachinePtr, SIGNAL(signal_Forward_triggered()),  aScenePtr, SLOT(slot_signalFF()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Play_triggered()),     aScenePtr, SLOT(slot_signalPlay()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Pause_triggered()),    aScenePtr, SLOT(slot_signalPause()));
    connect(theGameStateMachinePtr, SIGNAL(signal_RealFast_triggered()), aScenePtr, SLOT(slot_signal4F()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()),    aScenePtr, SLOT(slot_signalReset()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Slow_triggered()),     aScenePtr, SLOT(slot_signalSlow()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Stop_Gameplay()),      aScenePtr, SLOT(slot_signalPause()));

    slot_showGameResourcesDialog();
}


void ResizingGraphicsView::slot_clearWinFailDialogPtr()
{
    delete theWinFailDialogPtr;
    theWinFailDialogPtr = nullptr;
}


// TODO/FIXME: logic is now ok, should be triggered by GameStateMachine::signal_Game_Failed()
void ResizingGraphicsView::slot_levelDeath(void)
{
	DEBUG3ENTRY;
	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::DEATH, this);
	emit theWinFailDialogPtr->appearAnimated();
}


//TODO/FIXME: Part of this logic doesn't belong here
void ResizingGraphicsView::slot_levelWon(void)
{
    DEBUG1ENTRY;

    // Anti-cheat:
    // Don't label the level as complete when we're in level editor mode
    if (!theIsLevelCreator)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "done");
    }

	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::CONGRATS, this);
	emit theWinFailDialogPtr->appearAnimated();
}


void ResizingGraphicsView::slot_showGameResourcesDialog()
{
    QTimer::singleShot(100, theGameResourcesPtr, SLOT(appearAnimated()));
}
