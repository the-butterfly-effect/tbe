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
#include "ViewWorld.h"
#include "WinFailDialog.h"
#include "World.h"

ResizingGraphicsView::ResizingGraphicsView(QWidget *aParentPtr) :
	QGraphicsView(aParentPtr),
	theGameResourcesPtr(NULL),
	theMainWindowPtr(NULL),
	theWinFailDialogPtr(NULL),
	theFrameRateViewPtr(NULL)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	theSimControlsPtr = new SimulationControls(this);
	theGameResourcesPtr = new GameResources(this);
}


ResizingGraphicsView::~ResizingGraphicsView()
{
	delete theSimControlsPtr;
}


void ResizingGraphicsView::clearViewWorld(void)
{
	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	setScene(NULL);
	hideSimControls();
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


void ResizingGraphicsView::hideSimControls(void)
{
	emit theSimControlsPtr->hideYourself();
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
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

	theGameResourcesPtr->setup(aMenuBarPtr);

	// this one displays the frame rate counter if active
	theFrameRateViewPtr= aMenuBarPtr->addAction("");
}


void ResizingGraphicsView::setViewWorld(ViewWorld* aScenePtr, const QString& aLevelName)
{
	DEBUG3("MainWindow::setScene(%p, \"%s\")", aScenePtr, ASCII(aLevelName));
	theScenePtr=aScenePtr;

	setScene(aScenePtr);
	fitInView(0, -aScenePtr->getHeight(),
								aScenePtr->getWidth(), aScenePtr->getHeight());
	showSimControls();
//	parent()->setWindowTitle(APPNAME " - " + aLevelName);

	// also set the startstopwatch view
	theSimControlsPtr->hookSignalsUp(aScenePtr);

	connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), this, SLOT(slot_levelWon()));
//	connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath())), this, SLOT(slot_death());

	QTimer::singleShot(10, theGameResourcesPtr, SLOT(appearAnimated()));
}


void ResizingGraphicsView::showSimControls(void)
{
	emit theSimControlsPtr->showYourself();
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

	ChooseLevel myDialog(this, true);
	QString myNextLevelName = myDialog.getCurrent();
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
	emit showSimControls();
	delete theWinFailDialogPtr;
	theWinFailDialogPtr = NULL;
}


void ResizingGraphicsView::slot_levelDeath(void)
{
	// only need to display the dialog once...
	if (theWinFailDialogPtr!=NULL)
		return;
	theWinFailDialogPtr = new WinFailDialog(WinFailDialog::DEATH, this);
	emit theSimControlsPtr->onFailed();
//	emit hideSimControls();
	emit theScenePtr->slot_signalPause();
}


void ResizingGraphicsView::slot_levelWon(void)
{
	// only need to display the dialog once...
	if (theWinFailDialogPtr!=NULL)
		return;

    if (theIsLevelEditor==false)
    {
        QString myLevelFileName = Level::getLevelFileName();
        QSettings mySettings;
        mySettings.setValue("completed/"+myLevelFileName, "done");
    }

    theWinFailDialogPtr = new WinFailDialog(WinFailDialog::CONGRATS, this);

	// FIXME/TODO: must emit 'won' to simcontrols
	// also need to update simcontrols for this!
	emit hideSimControls();
	theWinFailDialogPtr->appearAnimated();

	// also make the sim stop once the above animation is (almost) done...
	QTimer::singleShot(1300, theScenePtr, SLOT(slot_signalPause()));
}
