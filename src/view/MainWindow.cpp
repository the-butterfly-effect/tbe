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

//////////////////////////////////////////////////////////////////////////////
// constructors & destructors

MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent), theLevelPtr(NULL), theSimStateMachine(&ui)
{                                      
	ui.setupUi(this);
	
	QGraphicsSvgItem* myTitlePagePtr = new SplashScreen("images/illustrations/title_page.svg");
	QGraphicsScene* mySplashScenePtr = new QGraphicsScene(NULL);

	mySplashScenePtr->addItem(myTitlePagePtr);
	
	// basic properties of the view

	// set my splash screen scene in view and make it fit nicely
	ui.graphicsView->setScene(mySplashScenePtr);
	QRectF myRect = myTitlePagePtr->boundingRect ();
	// TODO: FIXME: this /32.0 is wrong. very wrong :-(
	myRect.setWidth(myRect.width()/20.0);
	myRect.setHeight(myRect.height()/20.0);
	ui.graphicsView->fitInView(myRect, Qt::KeepAspectRatio);
	connect(myTitlePagePtr, SIGNAL(clicked()), 
			this, SLOT(on_splashScreen_clicked()));
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
	loadLevel(myFileName);
}
 
void MainWindow::on_splashScreen_clicked(void)
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
}

void MainWindow::setScene(DrawWorld* aScene, const QString& aLevelName)
{
	DEBUG5("MainWindow::setScene(%p, %s)\n", aScene, aLevelName.toAscii().constData());

	ui.graphicsView->setScene(aScene);

    QObject::connect(&theSimStateMachine, SIGNAL(startSim()), aScene, SLOT(startTimer()));
    QObject::connect(&theSimStateMachine, SIGNAL(stopSim()),  aScene, SLOT(stopTimer()));
    QObject::connect(&theSimStateMachine, SIGNAL(resetSim()), aScene, SLOT(resetWorld()));
    
    setWindowTitle(APPNAME " - " + aLevelName);
}

void MainWindow::purgeLevel(void)
{
	DEBUG5("MainWindow::purgeLevel(void)\n");
	if (theLevelPtr==NULL)
		return;
	
QMatrix myMatrix = ui.graphicsView->matrix();
DEBUG5("matrix: %f %f - %f %f - %f %f\n",
		myMatrix.m11(), myMatrix.m12(), 
		myMatrix.m21(), myMatrix.m22(),
		myMatrix.dx(), myMatrix.dy());

	// disconnect & delete the Scene//DrawWorld
	// keep in mind that we have a view that's not happy now!
	QGraphicsScene* myScene = ui.graphicsView->scene();
	if (myScene != NULL)
	{
		ui.graphicsView->setScene(NULL);
		QMatrix myMatrix;
		ui.graphicsView->setMatrix(myMatrix);
		delete myScene;
	}
		
	// disconnect the World
	delete theLevelPtr;
	theLevelPtr=NULL;
}
