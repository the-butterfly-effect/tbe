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

#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>

#include "tbe_global.h"
#include "MainWindow.h"
#include "Popup.h"
#include "ChooseLevel.h"
#include "Level.h"
#include "World.h"
#include "DrawWorld.h"
#include "SplashScreen.h"

#include "toolbox.h"
#include "SaveLevelInfo.h"

//////////////////////////////////////////////////////////////////////////////
// constructors & destructors

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), theLevelPtr(NULL),
	  theScenePtr(NULL)
{                                      
	ui.setupUi(this);
	showMaximized();


	// setup UndoGroup's QActions and add them to Edit menu
	theUndoActionPtr = theUndoGroup.createUndoAction(this, tr("&Undo"));
	theUndoActionPtr->setShortcut(tr("Ctrl+Z"));
	ui.menuEdit->addAction(theUndoActionPtr);
	
	theRedoActionPtr = theUndoGroup.createRedoAction(this, tr("&Redo"));
	QList<QKeySequence> redoShortcuts;
	redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
	theRedoActionPtr->setShortcuts(redoShortcuts);
	ui.menuEdit->addAction(theRedoActionPtr);

	setSimSpeed(0.5);

	// if level specified on command line, don't display splash screen
	QStringList myArguments = QApplication::arguments();
	if (myArguments.count()>1)
	{
		slot_splashScreen_clicked();
	}
	else
	{
		// TODO: fixme: hardcoded path here!
		QGraphicsSvgItem* myTitlePagePtr = new SplashScreen("images/title_page.svg");
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
	}

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
				"(C) 2009-2010 Peter van Ginneken and Klaas van Gend<br>"
				"Licensed under GPL version 2 - <i>only</i>."), this);
}

void MainWindow::on_actionBrand_names_activated()
{
	Popup::Info(tr("<b>The Butterfly Effect - Brand names</b><br><br>"
				"<b>Coke</b>, diet Coke, Coca Cola and the bottle shape are "
				"world-wide registered trademarks of the Coca Cola company."
				"<br>"
				"<b>Mentos</b> is a registered trademark of Perfetti Van Melle."
				"<br>"
				"The use of a Mentos in diet Coke is widely described - and "
				"apparently works best with diet Coke and Mentos. <br><br>"
				"'The Butterfly Effect' as a concept of chaos theory "
				"has existed since at least the mid 1950s. "
				"We are not affiliated with the 2004 movie in any way."
				""), this);
}

void MainWindow::on_actionLibraries_activated()
{
	Popup::Info(tr("<b>The Butterfly Effect - Libraries</b><br><br>"
				"The Butterfly Effect is a proud user of the Box2D"
				"Physics Library. Please refer to http://www.box2d.org/ ."
				"<br>"
				"The Butterfly Effect uses Nokia's Qt as GUI toolkit. "
				"We are happy with the continued support of Qt by Nokia. "
				"Please refer to http://qt.nokia.com/ ."
				""), this);
}

void MainWindow::on_actionOpen_custom_level_activated()
{
	QString myFileName = QFileDialog::getOpenFileName(this,
		 tr("Open level"), ".", tr("TBE levels (*.xml)"));
	if (myFileName.isEmpty())
		return;
	loadLevel(myFileName);
}

void MainWindow::on_actionOpen_level_activated()
{
	// in ChooseLevel.ui, the dialog is made ApplicationGlobal.
	ChooseLevel myDialog;
	if (myDialog.exec()==QDialog::Rejected)
		return;

	QString myLevelName = myDialog.getCurrent();
	loadLevel(myLevelName);
}

void MainWindow::on_actionSave_activated()
{
	DEBUG5("MainWindow::on_actionSave_activated()\n");
	SaveLevelInfo mySaveLevel(theLevelPtr,this);
	int myReturnCode = mySaveLevel.exec();
	if (myReturnCode == QDialog::Rejected)
		return;

	// for now we ignore the return code...
	if (mySaveLevel.commitToLevel()==false)
		Popup::Warning(tr("You did not fill in all fields - but level saved anyway\n"));

	QFileInfo myFileInfo(mySaveLevel.getFileName());

	DEBUG5("File '%s' is readable: %d, writeable: %d\n",
		   ASCII(myFileInfo.absoluteFilePath()),
		   myFileInfo.isReadable(), myFileInfo.isWritable());

	theLevelPtr->save(myFileInfo.absoluteFilePath());
}


void MainWindow::slot_splashScreen_clicked(void)
{
	QStringList myArguments = QApplication::arguments();
	if (myArguments.count()>1)
		loadLevel(myArguments[1]);
	else
		on_actionOpen_level_activated();
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
				ASCII(aFileName),
				ASCII(myErrorMessage) );
		exit(1);
	}
	theLevelPtr->getTheWorldPtr()->createScene(this);

	ui.theToolBoxView->clear();

	if (theIsLevelEditor)
		ui.theToolBoxView->fillFromObjectFactory();
	else
		ui.theToolBoxView->fillFromDomNode(theLevelPtr->getToolboxDomNode());
}

void MainWindow::on_levelWon()
{
	{
		QString myLevelFileName = Level::getLevelFileName();
		QSettings mySettings;
		mySettings.setValue("completed/"+myLevelFileName, "done");
	}
	emit on_actionOpen_level_activated();
}

void MainWindow::setScene(DrawWorld* aScene, const QString& aLevelName)
{
	DEBUG5("MainWindow::setScene(%p, %s)\n", aScene, ASCII(aLevelName));
	theScenePtr=aScene;

	ui.graphicsView->setScene(aScene);
	aScene->setSimSpeed(theSimSpeed);

	QObject::connect(aScene, SIGNAL(levelWon()), this, SLOT(on_levelWon()));

    theUndoGroup.addStack(aScene->getTheUndoStackPtr());
    theUndoGroup.setActiveStack(aScene->getTheUndoStackPtr());
    
    setWindowTitle(APPNAME " - " + aLevelName);

	// also set the startstopwatch view
	ui.StartStopView->setScene(aScene->getStartStopWatchPtr());
	ui.StartStopView->fitInView(aScene->getStartStopWatchPtr()->itemsBoundingRect(), Qt::KeepAspectRatio);
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
		QObject::disconnect(theScenePtr, SIGNAL(levelWon()), this, SLOT(on_levelWon()));

	    // Destroying theScene (which is a DrawWorld) will automatically
	    // destroy the associated UndoStack. The UndoStack will de-register 
	    // itself with the UndoGroup - no need to do anything myself here :-)

	    delete theScenePtr;
		theScenePtr=NULL;
	}
		
}


void MainWindow::setSimSpeed(qreal aSpeed)
{
	DEBUG5("MainWindow::setSimSpeed(%f)\n", aSpeed);

	ui.action_quarter_speed->setChecked(aSpeed == 0.25);
	ui.action_half_speed->setChecked(aSpeed == 0.5);
	ui.action_normal_speed->setChecked(aSpeed == 1.0);
	ui.action_double_speed->setChecked(aSpeed == 2.0);
	theSimSpeed = aSpeed;
	if (theScenePtr)
		theScenePtr->setSimSpeed(theSimSpeed);
}
