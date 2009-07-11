/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#include "tbe_global.h"
#include "MainWindow.h"
#include "Popup.h"

#include "Level.h"
#include "World.h"
#include "DrawWorld.h"
#include "SplashScreen.h"

#include "ToolBoxItemListModel.h"

//////////////////////////////////////////////////////////////////////////////
// constructors & destructors

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), theLevelPtr(NULL),
	  theScenePtr(NULL), theSimStateMachine(&ui)
{                                      
	ui.setupUi(this);

	// TODO: fixme: hardcoded path here!
	QGraphicsSvgItem* myTitlePagePtr = new SplashScreen("images/illustrations/title_page.svg");
	QGraphicsScene* mySplashScenePtr = new QGraphicsScene(NULL);

	mySplashScenePtr->addItem(myTitlePagePtr);
	
	// set my splash screen scene in view and make it fit nicely
	ui.graphicsView->setScene(mySplashScenePtr);
	QRectF myRect = myTitlePagePtr->boundingRect ();
	// TODO: FIXME: this /32.0 is wrong. very wrong :-(
	myRect.setWidth(myRect.width()/20.0);
	myRect.setHeight(myRect.height()/20.0);
	ui.graphicsView->fitInView(myRect, Qt::KeepAspectRatio);
	connect(myTitlePagePtr, SIGNAL(clicked()), 
			this, SLOT(slot_splashScreen_clicked()));
	
	// setup UndoGroup's QActions and add them to Edit menu
	theUndoActionPtr = theUndoGroup.createUndoAction(this, tr("&Undo"));
	theUndoActionPtr->setShortcut(tr("Ctrl+Z"));
	ui.menuEdit->addAction(theUndoActionPtr);
	
	theRedoActionPtr = theUndoGroup.createRedoAction(this, tr("&Redo"));
	QList<QKeySequence> redoShortcuts;
	redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
	theRedoActionPtr->setShortcuts(redoShortcuts);
	ui.menuEdit->addAction(theRedoActionPtr);
}                           

MainWindow::~MainWindow()
{
	purgeLevel();
}

//////////////////////////////////////////////////////////////////////////////
// public slots - alphabetical

void MainWindow::on_actionAbout_activated()
{
	Popup::Info(tr("<b>The Butterfly Effect</b><br><br>"
				"An open source game that uses realistic physics"
				" simulations to combine lots of simple mechanical elements"
				" to achieve a simple goal in the most complex way possible.<br><br>"
				"(C) 2009 Peter van Ginneken and Klaas van Gend<br>"
				"Licensed under GPL version 2"), this);
}

void MainWindow::on_actionOpen_level_activated()
{
	// TODO this code is more-or-less temporary 
	// - it should be replaced by a dashboard style interface 
	QString myFileName = QFileDialog::getOpenFileName(this,
	     tr("Open Level"), ".", tr("TBE Levels (*.xml *.tbe)"));
	if (myFileName.isEmpty())
		return;
	loadLevel(myFileName);
}

void MainWindow::slot_splashScreen_clicked(void)
{
	// TODO: FIXME: hardcoded level name!!!
	loadLevel("levels/draft/001_bowling_for_butterflies.xml");
}

//////////////////////////////////////////////////////////////////////////////
// public accessor methods

void MainWindow::loadLevel(const QString& aFileName)
{
	if (theLevelPtr != NULL)
		purgeLevel();
	
	// create level and display in main window
    theLevelPtr = new Level();
    QString myErrorMessage = theLevelPtr->load(aFileName); 
    if (!myErrorMessage.isEmpty())
    {
    	// TODO: popup and such
    	DEBUG1("ERROR during reading file '%s': %s\n",
    			aFileName.toAscii().constData(),
    			myErrorMessage.toAscii().constData() );
    	exit(1);
    }
    theLevelPtr->getTheWorldPtr()->createScene(this);
    theSimStateMachine.goToState(StartStopReset::NOTSTARTED);

	ToolBoxItemListModel* theNewToolbox = new ToolBoxItemListModel();
	theNewToolbox->fillFromObjectFactory();
	ui.theToolBoxView->setModel(theNewToolbox);
	ui.theToolBoxView->setViewMode(QListView::IconMode);
	ui.theToolBoxView->setIconSize(QSize(32, 32));
	ui.theToolBoxView->setGridSize(QSize(100, 50));
	ui.theToolBoxView->setMovement(QListView::Snap);
	ui.theToolBoxView->setAcceptDrops(true);
	ui.theToolBoxView->setDragEnabled(true);
}

void MainWindow::setScene(DrawWorld* aScene, const QString& aLevelName)
{
	DEBUG5("MainWindow::setScene(%p, %s)\n", aScene, aLevelName.toAscii().constData());
	theScenePtr=aScene;

	ui.graphicsView->setScene(aScene);

    QObject::connect(&theSimStateMachine, SIGNAL(startSim()), aScene, SLOT(startTimer()));
    QObject::connect(&theSimStateMachine, SIGNAL(stopSim()),  aScene, SLOT(stopTimer()));
    QObject::connect(&theSimStateMachine, SIGNAL(resetSim()), aScene, SLOT(resetWorld()));
    
    theUndoGroup.addStack(aScene->getTheUndoStackPtr());
    theUndoGroup.setActiveStack(aScene->getTheUndoStackPtr());
    
    setWindowTitle(APPNAME " - " + aLevelName);
}

void MainWindow::purgeLevel(void)
{
	DEBUG5("MainWindow::purgeLevel(void)\n");
	if (theLevelPtr==NULL)
		return;
	
	// disconnect the World
	delete theLevelPtr;
	theLevelPtr=NULL;

	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	ui.graphicsView->setScene(NULL);
	QMatrix myMatrix;
	ui.graphicsView->setMatrix(myMatrix);
	if (theScenePtr != NULL)
	{
	    QObject::disconnect(&theSimStateMachine, SIGNAL(startSim()), theScenePtr, SLOT(startTimer()));
	    QObject::disconnect(&theSimStateMachine, SIGNAL(stopSim()),  theScenePtr, SLOT(stopTimer()));
	    QObject::disconnect(&theSimStateMachine, SIGNAL(resetSim()), theScenePtr, SLOT(resetWorld()));

	    // Destroying theScene (which is a DrawWorld) will automatically
	    // destroy the associated UndoStack. The UndoStack will de-register 
	    // itself with the UndoGroup - no need to do anything myself here :-)

	    delete theScenePtr;
		theScenePtr=NULL;
	}
		
}
