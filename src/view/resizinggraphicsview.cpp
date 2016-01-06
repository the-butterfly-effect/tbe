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
#include "GameResources.h"
#include "Level.h"
#include "MainWindow.h"
#include "PieMenu.h"
#include "Popup.h"
#include "RegressionTest.h"
#include "resizinggraphicsview.h"
#include "SimulationControls.h"
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
    theWinFailDialogPtr(nullptr),
    theFrameRateViewPtr(nullptr)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	setFrameStyle(QFrame::Plain + QFrame::NoFrame);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	theSimControlsPtr = new SimulationControls(this);
	theGameResourcesPtr = new GameResources(this);
	theRSGVPtr = this;
}


ResizingGraphicsView::~ResizingGraphicsView()
{
	delete theSimControlsPtr;
	theRSGVPtr = nullptr;
}


void ResizingGraphicsView::clearViewWorld(void)
{
	DEBUG3ENTRY;
	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	setScene(nullptr);
	emit theSimControlsPtr->hideYourself();
	QMatrix myMatrix;
	setMatrix(myMatrix);

	delete theWinFailDialogPtr;
	theWinFailDialogPtr = nullptr;
	delete theScenePtr;
	theScenePtr = nullptr;
}


GameResources* ResizingGraphicsView::getGameResourcesDialogPtr() const
{
	Q_ASSERT(theGameResourcesPtr != nullptr);
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
	theSimControlsPtr->parentResize(frameSize());
	PieMenuSingleton::setViewInSceneCoords(mapToScene(rect()));
}


void ResizingGraphicsView::setup(MainWindow* aMWPtr, QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr)
{
	theMainWindowPtr = aMWPtr;
	theSimControlsPtr->setup(anMenuControlsPtr);
	connect(CrossRegisterSingleton::me(), SIGNAL(signalNumberCrossesChanged(int)), theSimControlsPtr, SLOT(slotNumberOfCrossesChanged(int)));

	connect (theGameResourcesPtr, SIGNAL(signalReloadLevel()),
			 theMainWindowPtr, SLOT(reloadLevel()));

	// this one displays the frame rate counter if active
	theFrameRateViewPtr= aMenuBarPtr->addAction("");
}


void ResizingGraphicsView::setViewWorld(ViewWorld* aScenePtr,
                                        const QString& aLevelName)
{
	DEBUG1("MainWindow::setViewWorld(%p, \"%s\")", aScenePtr,
               ASCII(aLevelName));
	theScenePtr=aScenePtr;

	setScene(aScenePtr);
	fitInView(0, -aScenePtr->getHeight(),
			  aScenePtr->getWidth(), aScenePtr->getHeight());
	resizeEvent(nullptr);
	emit theSimControlsPtr->showYourself();
	theMainWindowPtr->setWindowTitle(APPNAME + " - " + aLevelName);

	// also set the startstopwatch view
	theSimControlsPtr->hookSignalsUp(aScenePtr);

	connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), this, SLOT(slot_levelWon()));
	connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), this, SLOT(slot_levelDeath()));
	connect(aScenePtr, SIGNAL(needReset()), theSimControlsPtr, SLOT(onReset()));

	if (theIsRunAsRegression)
	{
		connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), theMainWindowPtr->theRegressionTest, SLOT(slot_Won()));
		connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), theMainWindowPtr->theRegressionTest, SLOT(slot_Fail()));
	}

	QTimer::singleShot(100, theGameResourcesPtr, SLOT(appearAnimated()));
}


void ResizingGraphicsView::slot_actionChooseLevel()
{
	DEBUG3ENTRY;
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = nullptr;
	emit theMainWindowPtr->on_action_Open_Level_triggered();
}


void ResizingGraphicsView::slot_actionNextLevel()
{
	DEBUG3ENTRY;
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = nullptr;
	QString myNextLevelName = ChooseLevel::getNextLevelName();
	if (myNextLevelName.isEmpty()==false)
		theMainWindowPtr->loadLevel(myNextLevelName);
	else
		emit slot_actionChooseLevel();
}


void ResizingGraphicsView::slot_actionReplay()
{
	DEBUG3ENTRY;
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = nullptr;
	emit theScenePtr->slot_signalReset();
	emit theSimControlsPtr->onReset();
	emit theSimControlsPtr->showYourself();
}

void ResizingGraphicsView::slot_actionSkipLevel()
{
	DEBUG3ENTRY;
    if (theIsLevelEditor==false)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "skipped");
    }
    slot_actionNextLevel();
}


void ResizingGraphicsView::slot_editObjectDialog_destroyed()
{
    DEBUG3ENTRY;
	theObjectEditorPtr = nullptr;
}


void ResizingGraphicsView::slot_levelDeath(void)
{
	// only need to display the dialog once...
	if (theWinFailDialogPtr!=nullptr)
		return;
	DEBUG3ENTRY;
	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::DEATH, this);
	emit theSimControlsPtr->onFailed();
	emit theWinFailDialogPtr->appearAnimated();
}


void ResizingGraphicsView::slot_levelWon(void)
{
	// Only need to display the dialog once...
	if (theWinFailDialogPtr!=nullptr)
		return;
	DEBUG3ENTRY;

    if (theIsLevelEditor==false)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "done");
    }

	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::CONGRATS, this);
	emit theWinFailDialogPtr->appearAnimated();

	// Make the sim stop once the above animation is (almost) done...
	QTimer::singleShot(3000, theScenePtr, SLOT(slot_signalPause()));
}


void ResizingGraphicsView::slot_showEditObjectDialog(AbstractObjectPtr anAOPtr)
{
    if (theObjectEditorPtr!=nullptr)
        delete theObjectEditorPtr;
    theObjectEditorPtr = new EditObjectDialog(anAOPtr, this);
    connect(theObjectEditorPtr, SIGNAL(destroyed()), this, SLOT(on_objectEditor_destroyed()));
    theObjectEditorPtr->show();
}


void ResizingGraphicsView::slot_showGameResourcesDialog()
{
	QTimer::singleShot(100, theGameResourcesPtr, SLOT(appearAnimated()));
}
