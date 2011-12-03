/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore>
#include <QtGui>
#include <cassert>


#include "AbstractObject.h"
#include "DropDownWindow.h"
#include "Level.h"
#include "Popup.h"
#include "PieMenu.h"
#include "SimulationControls.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "World.h"




MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow),
	  theLevelPtr(NULL),
	  theScenePtr(NULL),
	  theDropDown(NULL)
{
	ui->setupUi(this);

	setupMenu();
	setupView();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::loadLevel(const QString& aFileName)
{
	DEBUG1ENTRY;
	if (theLevelPtr != NULL)
		purgeLevel();

	// create level and display in main window
	theLevelPtr = new Level();
	QString myErrorMessage = theLevelPtr->load(aFileName);
	if (!myErrorMessage.isEmpty())
	{
		QChar myFirst = myErrorMessage[0];
		myErrorMessage = myErrorMessage.mid(1);
		if (myFirst == 'E')
		{
			Popup::Critical(tr("ERROR during reading file '%1': '%2'\n")
							.arg(aFileName).arg(myErrorMessage), this );
			exit(1);
		}
		if (myFirst == 'W')
		{
			Popup::Warning(tr("Non-fatal problem reading file '%1': '%2'.\n"
							  "This may affect playability, though!")
							.arg(aFileName).arg(myErrorMessage), this );
		}
	}
	theLevelPtr->getTheWorldPtr()->createScene(this);

#if 0
	ui.theToolBoxView->clear();

	if (theIsLevelEditor)
		ui.theToolBoxView->fillFromObjectFactory();
	else
		ui.theToolBoxView->fillFromDomNode(theLevelPtr->getToolboxDomNode());

	// display the level info
	if (!theIsLevelEditor)
	{
		LevelInfoDialog* myInfoDialog = new LevelInfoDialog(theLevelPtr, this);
		myInfoDialog->setAutoFillBackground(true);
		QSize myDialogSize = myInfoDialog->size();
		QSize myViewSize = ui.graphicsView->size();
		myInfoDialog->move((myViewSize.width()-myDialogSize.width())/2,
						   (myViewSize.height()-myDialogSize.height())/2);
		myInfoDialog->show();
	}
#endif
}


void MainWindow::setScene(ViewWorld* aScenePtr, const QString& aLevelName)
{
	DEBUG3("MainWindow::setScene(%p, \"%s\")", aScenePtr, ASCII(aLevelName));
	theScenePtr=aScenePtr;

	ui->graphicsView->setScene(aScenePtr);
	ui->graphicsView->fitInView(0, -aScenePtr->getHeight(),
								aScenePtr->getWidth(), aScenePtr->getHeight());
//	aScenePtr->setSimSpeed(theSimSpeed);

//	QObject::connect(aScenePtr, SIGNAL(levelWon()), this, SLOT(slot_levelWon()));

	setWindowTitle(APPNAME " - " + aLevelName);

	// also set the startstopwatch view
//	ui.StartStopView->setScene(aScenePtr->getStartStopWatchPtr());
//	ui.StartStopView->fitInView(aScenePtr->getStartStopWatchPtr()->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void MainWindow::purgeLevel(void)
{
	DEBUG1ENTRY;
	if (theLevelPtr==NULL)
		return;

	// disconnect the World
	delete theLevelPtr;
	theLevelPtr=NULL;

	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	ui->graphicsView->setScene(NULL);
	QMatrix myMatrix;
	ui->graphicsView->setMatrix(myMatrix);
#if 0
	if (theScenePtr != NULL)
	{
		QObject::disconnect(theScenePtr, SIGNAL(levelWon()), this, SLOT(slot_levelWon()));

		// Destroying theScene (which is a DrawWorld) will automatically
		// destroy the associated UndoStack. The UndoStack will de-register
		// itself with the UndoGroup - no need to do anything myself here :-)

		delete theScenePtr;
		theScenePtr=NULL;
	}
#endif
}


void MainWindow::setupMenu(void)
{
}

void MainWindow::setupView()
{
	// setup UndoGroup's QActions and add them to Edit menu
	// note that this doesn't enable them yet, our ViewWorld should handle that...
	// TODO/FIXME: that needs work
	QAction* myUndoActionPtr = UndoSingleton::createUndoAction(this, tr("&Undo"));
	myUndoActionPtr->setShortcut(tr("Ctrl+Z"));
	ui->menuEdit->addAction(myUndoActionPtr);
	QAction* myRedoActionPtr = UndoSingleton::createRedoAction(this, tr("&Redo"));
	QList<QKeySequence> redoShortcuts;
	redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
	myRedoActionPtr->setShortcuts(redoShortcuts);
	ui->menuEdit->addAction(myRedoActionPtr);

	loadLevel("newguitest.xml");

	DropDownWindow* theDropDown = new DropDownWindow(theScenePtr);
	theScenePtr->addItem(theDropDown);
	theDropDown->setup(ui->menuBar);

//	AbstractObject* theAOPtr = new AbstractObject();
//	theAOPtr->theProps.setProperty(Property::ROTATABLE_STRING, "true");
//	theAOPtr->setTheHeight(80);
//	theAOPtr->setTheWidth(80);
//	theAOPtr->setOrigCenter(Position(60,60, 0));
//	theAOPtr->resizableInfo = AbstractObject::HORIZONTALRESIZE;
//	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
//	theScenePtr->addItem(theVOPtr);

//	AbstractObject* theAOPtr2 = new AbstractObject();
//	theAOPtr2->theProps.setProperty(Property::ROTATABLE_STRING, "true");
//	theAOPtr2->setTheHeight(80);
//	theAOPtr2->setTheWidth(180);
//	theAOPtr2->setOrigCenter(Position(230,100, 0.1));
//	theAOPtr2->resizableInfo = AbstractObject::TOTALRESIZE;
//	ViewObject* theVOPtr2 = new ViewObject(theAOPtr2, "../images/QuarterArc.png");
//	theScenePtr->addItem(theVOPtr2);

	SimulationControls* myControls = new SimulationControls;
	myControls->setup(ui->menuBar);
}
