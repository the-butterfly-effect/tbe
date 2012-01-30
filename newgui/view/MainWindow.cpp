/* The Butterfly Effect
 * This file copyright (C) 2009,2011,2012 Klaas van Gend
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
#include "ChooseLevel.h"
#include "Level.h"
#include "Popup.h"
#include "UndoSingleton.h"
#include "World.h"




MainWindow::MainWindow(bool isMaximized, QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow),
	  theLevelPtr(NULL),
	  theWorldPtr(NULL)
{
	ui->setupUi(this);

	setupMenu();
	setupView();
	if (isMaximized)
		showMaximized();
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
	QString myErrorMessage = theLevelPtr->load(aFileName,
								   ui->graphicsView->getGameResourcesDialogPtr());
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
	theLevelPtr->getTheWorldPtr()->createScene(ui->graphicsView);
}


void MainWindow::on_action_About_activated()
{
	//: translators: <b> and <br> are statements for bold and newline, respectively
	Popup::Info(tr("<b>The Butterfly Effect</b><br><br>"
				"An open source game that uses realistic physics"
				" simulations to combine lots of simple mechanical elements"
				" to achieve a simple goal in the most complex way possible.<br><br>"
				"(C) 2009,2010,2011,2012 Peter van Ginneken and Klaas van Gend<br>"
				"Licensed under GPL version 2 - <i>only</i>.<br><br>"
				"See http://%1/ for more info on this project.")
				.arg(QCoreApplication::instance()->organizationDomain()), this);
}


void MainWindow::on_action_Bug_Reports_activated()
{
	//: translators: <b> and <br> are statements for bold and newline, respectively
	Popup::Info(tr("<b>The Butterfly Effect - Bug Reports</b><br><br>"
				   "Of course, this game is not bug free yet.<br>"
				   "If you come across anything that you think should not "
				   "happen, please let us know. Go to our ticket website:"
				   "<br><a href=\""
				   "http://sourceforge.net/apps/trac/tbe/newticket\">"
				   "http://sourceforge.net/apps/trac/tbe/newticket</a><br>"
				   "Please tell us at least the name of the level, what you "
				   "expected to happen and what did happen. If you want to "
				   "learn how we fix your issue, please provide a valid "
				   "e-mail address."
				""), this);
}


void MainWindow::on_action_Libraries_activated()
{
	//: translators: <b> and <br> are statements for bold and newline, respectively
	Popup::Info(tr("<b>The Butterfly Effect - Libraries</b><br><br>"
				"The Butterfly Effect is a proud user of the Box2D "
				"Physics Library. Please refer to http://www.box2d.org/ ."
				"<br>"
				"The Butterfly Effect uses the Qt GUI toolkit. "
				"Please refer to http://qt-project.org/ ."
				""), this);
}


void MainWindow::on_action_New_Levels_activated(void)
{
	//: translators: <b> and <br> are statements for bold and newline, respectively
	Popup::Info(tr("<b>The Butterfly Effect - Create New Levels</b><br><br>"
				   "We know you can design better levels than we do!<br>"
				   "Use the Level Creator to build your levels and please submit them to us.<br>"
				   "Even if your level is not finished yet, don't hestitate to share it with us! "
				   "Of course, define how you think it should work so others can join in."
				   "<br><br>This is our level mailing list:<br>"
				   "<a href=\"mailto:tbe-levels@lists.sf.net\">tbe-levels@lists.sf.net</a><br>"
				   ""), this);
}


void MainWindow::on_action_Suggestions_activated()
{
	Popup::Info(tr("<b>The Butterfly Effect - Suggestions</b><br><br>"
				   "If you have great ideas for new features in the game, "
				   "please go to our shiny forums at: <br><a href=\""
				   "http://sourceforge.net/projects/tbe/forums/forum/953128\">"
				   "http://sourceforge.net/projects/tbe/forums/forum/953128</a>"
				   "<br>to share your ideas with the world."
				""), this);
}


void MainWindow::purgeLevel(void)
{
	DEBUG1ENTRY;
	UndoSingleton::clear();
	delete theLevelPtr;
	theLevelPtr=NULL;
	ui->graphicsView->clearViewWorld();
}


void MainWindow::reloadLevel(void)
{
	DEBUG1ENTRY;
	if (theLevelPtr==NULL)
		return;
	QString myLevelName = theLevelPtr->getLevelFileName();
	purgeLevel();
	loadLevel(myLevelName);
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

	ui->graphicsView->setup(this, ui->menuBar, ui->menuControls);
	if (theStartFileName.isEmpty())
	{
		ChooseLevel myDialog(NULL, true);
		QString myNextLevelName = myDialog.getCurrent();
		if (myNextLevelName.isEmpty()==false)
			loadLevel(myNextLevelName);
	}
	else
		loadLevel(theStartFileName);
}

void MainWindow::on_action_Open_Level_triggered()
{
	ChooseLevel* myDialogPtr = new ChooseLevel(ui->graphicsView);
	connect(myDialogPtr, SIGNAL(loadLevel(QString)),
			this, SLOT(loadLevel(QString)));
}
