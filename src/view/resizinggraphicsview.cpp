/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "ChooseLevel.h"
#include "GameResources.h"
#include "Level.h"
#include "MainWindow.h"
#include "PieMenu.h"
#include "Popup.h"
#include "resizinggraphicsview.h"
#include "SimulationControls.h"
#include "ViewObjectActionDectorator.h"
#include "ViewWorld.h"
#include "WinFailDialog.h"
#include "World.h"


static ResizingGraphicsView* theRSGVPtr = NULL;


ResizingGraphicsView::ResizingGraphicsView(QWidget *aParentPtr) :
	QGraphicsView(aParentPtr),
	theGameResourcesPtr(NULL),
	theMainWindowPtr(NULL),
	theWinFailDialogPtr(NULL),
	theFrameRateViewPtr(NULL)
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
	theRSGVPtr = NULL;
}


void ResizingGraphicsView::clearViewWorld(void)
{
	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	setScene(NULL);
	emit theSimControlsPtr->hideYourself();
	QMatrix myMatrix;
	setMatrix(myMatrix);

	if (theScenePtr != NULL)
	{
		QObject::disconnect(theScenePtr, SIGNAL(levelWon()), this, SLOT(slot_levelWon()));
		delete theScenePtr;
		theScenePtr=NULL;
	}
}


GameResources* ResizingGraphicsView::getGameResourcesDialogPtr() const
{
	Q_ASSERT(theGameResourcesPtr != NULL);
	return theGameResourcesPtr;
}


ResizingGraphicsView* ResizingGraphicsView::me(void)
{
	Q_ASSERT(theRSGVPtr!=NULL);
	return theRSGVPtr;
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
	if (event!=NULL)
		QGraphicsView::resizeEvent(event);
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	theSimControlsPtr->parentResize(frameSize());
	theGameResourcesPtr->parentResize(transform());
	PieMenuSingleton::setViewInSceneCoords(mapToScene(rect()));
}


void ResizingGraphicsView::setup(MainWindow* aMWPtr, QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr)
{
	theMainWindowPtr = aMWPtr;
	theSimControlsPtr->setup(anMenuControlsPtr);
	connect(CrossRegisterSingleton::me(), SIGNAL(signalNumberCrossesChanged(int)), theSimControlsPtr, SLOT(slotNumberOfCrossesChanged(int)));

        theGameResourcesPtr->setup(anMenuControlsPtr);
	connect (theGameResourcesPtr, SIGNAL(signalReloadLevel()),
			 theMainWindowPtr, SLOT(reloadLevel()));

	// this one displays the frame rate counter if active
	theFrameRateViewPtr= aMenuBarPtr->addAction("");
}


void ResizingGraphicsView::setViewWorld(ViewWorld* aScenePtr,
                                        const QString& aLevelName)
{
        DEBUG3("MainWindow::setViewWorld(%p, \"%s\")", aScenePtr,
               ASCII(aLevelName));
	theScenePtr=aScenePtr;

	setScene(aScenePtr);
        fitInView(0, -aScenePtr->getHeight(),
                  aScenePtr->getWidth(), aScenePtr->getHeight());
	resizeEvent(NULL);
	emit theSimControlsPtr->showYourself();
        theMainWindowPtr->setWindowTitle(APPNAME " - " + aLevelName);

	// also set the startstopwatch view
	theSimControlsPtr->hookSignalsUp(aScenePtr);

	connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), this, SLOT(slot_levelWon()));
	connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), this, SLOT(slot_levelDeath()));
	connect(aScenePtr, SIGNAL(needReset()), theSimControlsPtr, SLOT(onReset()));

	QTimer::singleShot(200, theGameResourcesPtr, SLOT(appearAnimated()));
}


void ResizingGraphicsView::slot_actionChooseLevel()
{
	DEBUG3ENTRY;
	emit theMainWindowPtr->on_action_Open_Level_triggered();
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = NULL;
}


void ResizingGraphicsView::slot_actionNextLevel()
{
	DEBUG3ENTRY;
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = NULL;

        QString myNextLevelName = ChooseLevel::getNextLevelName();
	DEBUG1("theMainWindowPtr=%p\n", theMainWindowPtr);
	if (myNextLevelName.isEmpty()==false)
		theMainWindowPtr->loadLevel(myNextLevelName);
	else
		emit slot_actionChooseLevel();
}


void ResizingGraphicsView::slot_actionReplay()
{
	DEBUG3ENTRY;
	emit theScenePtr->slot_signalReset();
	emit theSimControlsPtr->onReset();
	emit theSimControlsPtr->showYourself();
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = NULL;
}

void ResizingGraphicsView::slot_actionSkipLevel()
{
    DEBUG1ENTRY;
    if (theIsLevelEditor==false)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "skipped");
    }
    slot_actionNextLevel();
}


void ResizingGraphicsView::slot_levelDeath(void)
{
	// only need to display the dialog once...
	if (theWinFailDialogPtr!=NULL)
		return;
	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::DEATH, this);
	emit theSimControlsPtr->onFailed();
	emit theWinFailDialogPtr->appearAnimated();
}


void ResizingGraphicsView::slot_levelWon(void)
{
	// Only need to display the dialog once...
	if (theWinFailDialogPtr!=NULL)
		return;

    if (theIsLevelEditor==false)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "done");
    }

    theWinFailDialogPtr = new WinFailDialog(WinFailDialog::CONGRATS, this);
	emit theSimControlsPtr->hideYourself();
	emit theWinFailDialogPtr->appearAnimated();

	// Make the sim stop once the above animation is (almost) done...
	QTimer::singleShot(3000, theScenePtr, SLOT(slot_signalPause()));
}
