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


#include "DropDownWindow.h"
#include "AbstractObject.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "UndoSingleton.h"

#include "SimulationControls.h"
#include "PieMenu.h"



MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow),
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


void MainWindow::setupMenu(void)
{
}

void MainWindow::setupView()
{
	// setup UndoGroup's QActions and add them to Edit menu
	// note that this doesn't enable them yet, our ViewWorld handles that :-)
	QAction* myUndoActionPtr = UndoSingleton::createUndoAction(this, tr("&Undo"));
	myUndoActionPtr->setShortcut(tr("Ctrl+Z"));
	ui->menuEdit->addAction(myUndoActionPtr);
	QAction* myRedoActionPtr = UndoSingleton::createRedoAction(this, tr("&Redo"));
	QList<QKeySequence> redoShortcuts;
	redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
	myRedoActionPtr->setShortcuts(redoShortcuts);
	ui->menuEdit->addAction(myRedoActionPtr);

	assert(theScenePtr==NULL);
	theScenePtr = new ViewWorld(0, -300, 300, 300);
	theScenePtr->setBackgroundBrush(Qt::blue);
	theScenePtr->addRect(0,0,300,-300);
	ui->graphicsView->setScene(theScenePtr);

	DropDownWindow* theDropDown = new DropDownWindow;
	theScenePtr->addItem(theDropDown);
	theDropDown->setup(ui->menuBar);

	AbstractObject* theAOPtr = new AbstractObject();
	theAOPtr->setTheHeight(80);
	theAOPtr->setTheWidth(80);
	theAOPtr->setOrigCenter(Position(60,60,1.57));
	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr);

	theScenePtr->addRect(20,-100,80,80);

	AbstractObject* theAOPtr2 = new AbstractObject();
	theAOPtr2->setTheHeight(180);
	theAOPtr2->setTheWidth(180);
	theAOPtr2->setOrigCenter(Position(230,130,1.0));
	ViewObject* theVOPtr2 = new ViewObject(theAOPtr2, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr2);

	SimulationControls* myControls = new SimulationControls;
	myControls->setup(ui->menuBar);
}
