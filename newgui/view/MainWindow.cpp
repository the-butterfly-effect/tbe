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


void MainWindow::purgeLevel(void)
{
	DEBUG1ENTRY;
	UndoSingleton::clear();
	delete theLevelPtr;
	theLevelPtr=NULL;
	ui->graphicsView->clearViewWorld();
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
