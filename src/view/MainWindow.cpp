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
#include "LevelInfoDialog.h"
#include "StartStopWatch.h"
#include "GoalEditor.h"
#include "EditLevelProperties.h"

//////////////////////////////////////////////////////////////////////////////
// constructors & destructors

MainWindow::MainWindow(bool isMaximized, QWidget *parent)
	: QMainWindow(parent), theLevelPtr(NULL),
	  theScenePtr(NULL)
{                                      
	ui.setupUi(this);
	if (isMaximized)
		showMaximized();

	if (theIsLevelEditor)
		emit on_actionGo_To_Level_Editor_activated();

#ifdef XXXNDEBUG
	// hook up the menus - only in release mode when compiled
	// with static library - see ticket:77
	#warning compiling the on_action connects into MainWindow
	QObject::connect(this, SIGNAL(actionAbout()),             this, SLOT(on_actionAbout_activated()));
	QObject::connect(this, SIGNAL(actionBrand_names()),       this, SLOT(on_actionBrand_names_activated()));
	QObject::connect(this, SIGNAL(actionGo_To_Level_Editor()), this, SLOT(on_actionGo_To_Level_Editor_activated()));
	QObject::connect(this, SIGNAL(actionLibraries()),         this, SLOT(on_actionLibraries_activated()));
	QObject::connect(this, SIGNAL(actionOpen_custom_level()), this, SLOT(on_actionOpen_custom_level_activated()));
	QObject::connect(this, SIGNAL(actionOpen_level()),        this, SLOT(on_actionOpen_level_activated()));
	QObject::connect(this, SIGNAL(actionSave()),              this, SLOT(on_actionSave_activated()));
	QObject::connect(this, SIGNAL(action_quarter_speed()),    this, SLOT(on_action_quarter_speed_activated()));
	QObject::connect(this, SIGNAL(action_half_speed()),       this, SLOT(on_action_half_speed_activated()));
	QObject::connect(this, SIGNAL(action_normal_speed()),     this, SLOT(on_action_normal_speed_activated()));
	QObject::connect(this, SIGNAL(action_double_speed()),     this, SLOT(on_action_double_speed_activated()));
#endif
			
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
	if (theStartFileName.isEmpty()==false)
	{
		slot_splashScreen_clicked();
	}
	else
	{
		if (theIsLevelEditor==false)
		{
			QGraphicsSvgItem* myTitlePagePtr = new SplashScreen(
					IMAGES_DIRECTORY + "/title_page.svg" );
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
				"Licensed under GPL version 2 - <i>only</i>.<br><br>"
				"See http://%1/ for more info on this project.")
				.arg(QCoreApplication::instance()->organizationDomain()), this);
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
				"<b>Post-it</b> is a registered trademark of 3M. <br><br>"
				"'The Butterfly Effect' as a concept of chaos theory "
				"has existed since at least the mid 1950s. "
				"We are not affiliated with the 2004 movie in any way."
				""), this);
}

void MainWindow::on_actionGo_activated(void)
{
	emit reinterpret_cast<StartStopWatch*>(ui.StartStopView->scene())->keypress_go();
}

void MainWindow::on_actionGo_To_Level_Editor_activated(void)
{
	theIsLevelEditor=true;
	ui.actionGo_To_Level_Editor->setEnabled(false);

	// if a level is already loaded, let's populate the toolbox
	if (theScenePtr!=NULL)
		ui.theToolBoxView->fillFromObjectFactory();

	// add menu item File > New... - at the top!
	//: translators: the ... indicates that this opens a new dialog - keep in the translation!
	theNewLevelActionPtr = new QAction( tr("New Level ..."), this);
	ui.menuFile->insertAction(ui.menuFile->actions().first(), theNewLevelActionPtr);
	connect(theNewLevelActionPtr, SIGNAL(triggered(void)), this, SLOT(slot_newLevelAction_clicked()));
	ui.actionSave->setEnabled(true);
	ui.actionSave_As->setEnabled(true);

	// populate the view menu more
	ui.menuView->addSeparator();

	//: translators: the ... indicates that this opens a new dialog - keep in the translation!
	theGoalEditorActionPtr = new QAction( tr("Goal Editor ..."), this);
	ui.menuView->addAction(theGoalEditorActionPtr);
	connect(theGoalEditorActionPtr, SIGNAL(triggered(void)), this, SLOT(slot_goalEditorAction_clicked(void)));

	//: translators: the ... indicates that this opens a new dialog - keep in the translation!
	theLevelPropertiesEditorActionPtr = new QAction( tr("Level Settings Editor ..."), this);
	ui.menuView->addAction(theLevelPropertiesEditorActionPtr);
	connect(theLevelPropertiesEditorActionPtr, SIGNAL(triggered(void)), this, SLOT(slot_levelPropertiesEditorAction_clicked(void)));

	ui.menuView->addSeparator();
	theDrawDebugActionPtr = new QAction( tr("Draw Box2D debug in sim"), this);
	theDrawDebugActionPtr->setCheckable(true);
	theDrawDebugActionPtr->setChecked(theDrawDebug);
	ui.menuView->addAction(theDrawDebugActionPtr);
	connect(theDrawDebugActionPtr,SIGNAL(toggled(bool)), this, SLOT(slot_drawDebugAction_toggle(bool)));
	theDrawOutlineActionPtr = new QAction( tr("Draw PolyObject outlines"), this);
	theDrawOutlineActionPtr->setEnabled(false);
	theDrawOutlineActionPtr->setCheckable(true);
	theDrawOutlineActionPtr->setChecked(theDrawPolyOutline);
	ui.menuView->addAction(theDrawOutlineActionPtr);
	connect(theDrawOutlineActionPtr,SIGNAL(toggled(bool)), this, SLOT(slot_drawOutlineAction_toggle(bool)));
}

void MainWindow::on_actionLibraries_activated()
{
	Popup::Info(tr("<b>The Butterfly Effect - Libraries</b><br><br>"
				"The Butterfly Effect is a proud user of the Box2D "
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
	ChooseLevel myDialog(this);
	if (myDialog.exec()==QDialog::Rejected)
		return;

	QString myLevelName = myDialog.getCurrent();
	loadLevel(myLevelName);
}

void MainWindow::on_actionPause_activated(void)
{
	emit reinterpret_cast<StartStopWatch*>(ui.StartStopView->scene())->keypress_pause();
}

void MainWindow::on_actionReset_2_activated(void)
{
	emit reinterpret_cast<StartStopWatch*>(ui.StartStopView->scene())->keypress_reset();
}

void MainWindow::on_actionSave_activated()
{
	if (theLevelPtr==NULL)
		return;

	DEBUG5("MainWindow::on_actionSave_activated()\n");
	QFileInfo myFileInfo(theLevelPtr->getLevelFileName());
	if (theLevelPtr->save(myFileInfo.absoluteFilePath())==false)
		Popup::Warning(tr("File '%1' could not be saved.").arg(myFileInfo.absoluteFilePath()));
	else
		ui.statusbar->showMessage(tr("File '%1' saved.").arg(myFileInfo.absoluteFilePath()),5);
}

void MainWindow::on_actionSave_As_activated()
{
	assert(theLevelPtr);
	if (theLevelPtr==NULL)
		return;

	DEBUG5("MainWindow::on_actionSave_As_activated()\n");
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

	on_actionSave_activated();
}


void MainWindow::slot_splashScreen_clicked(void)
{
	if (theStartFileName.isEmpty())
		on_actionOpen_level_activated();
	else
		loadLevel(theStartFileName);
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
}

void MainWindow::setScene(DrawWorld* aScene, const QString& aLevelName)
{
	DEBUG5("MainWindow::setScene(%p, %s)\n", aScene, ASCII(aLevelName));
	theScenePtr=aScene;

	ui.graphicsView->setScene(aScene);
	aScene->setSimSpeed(theSimSpeed);

	QObject::connect(aScene, SIGNAL(levelWon()), this, SLOT(slot_levelWon()));

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
		QObject::disconnect(theScenePtr, SIGNAL(levelWon()), this, SLOT(slot_levelWon()));

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

void MainWindow::slot_drawDebugAction_toggle(bool isChecked)
{
	theDrawDebug = isChecked;
	theDrawDebugActionPtr->setChecked(theDrawDebug);
	theScenePtr->setDrawDebug();
}

void MainWindow::slot_drawOutlineAction_toggle(bool isChecked)
{
	theDrawPolyOutline = isChecked;
	theDrawOutlineActionPtr->setChecked(theDrawPolyOutline);
	ui.graphicsView->updatePixelsPerUnit();
}


void MainWindow::slot_goalEditorAction_clicked(void)
{
	// the Goals dialog is modeless, i.e. it can stay floating around
	GoalEditor* myGoalEditorPtr = new GoalEditor(theLevelPtr->getTheWorldPtr(), this);
	myGoalEditorPtr->show();
}

void MainWindow::slot_levelPropertiesEditorAction_clicked(void)
{
	// this dialog is modal, i.e. user has to click OK/Cancel
	EditLevelProperties* myEditorPtr = new EditLevelProperties(theLevelPtr, this);
	myEditorPtr->show();
	// I don't care about the return.
	// If it was OK, the dialog already 'fixed' everything.
	myEditorPtr->exec();
	emit ui.graphicsView->on_timerTick();
}

void MainWindow::slot_levelWon()
{
	if (theIsLevelEditor==false)
	{
		QString myLevelFileName = Level::getLevelFileName();
		QSettings mySettings;
		mySettings.setValue("completed/"+myLevelFileName, "done");
	}
}

void MainWindow::slot_newLevelAction_clicked(void)
{
	if (theLevelPtr!=NULL)
	{
		if (Popup::YesNoQuestion(tr("WARNING: you already have a level open. Do you want to discard that?"),this)==false)
			return;
		purgeLevel();
	}
	ui.theToolBoxView->clear();

	// and we have a clean slate :-)
	theLevelPtr = new Level();
	theLevelPtr->getTheWorldPtr()->createScene(this);
	theLevelPtr->setLevelFileName("./lala.xml");

	// set the level properties and first save
	slot_levelPropertiesEditorAction_clicked();
	on_actionSave_As_activated();

	// and fill up the toolbox again.
	ui.theToolBoxView->fillFromObjectFactory();
}

void MainWindow::slot_next_level(void)
{
	ChooseLevel myDialog(this, true);
	loadLevel(myDialog.getCurrent());
}

