/* The Butterfly Effect
 * This file copyright (C) 2009,2011,2012,2013 Klaas van Gend
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
#include "EditLevelProperties.h"
#include "GoalEditor.h"
#include "InsertUndoCommand.h"
#include "Level.h"
#include "ObjectFactory.h"
#include "Popup.h"
#include "SaveLevelInfo.h"
#include "UndoSingleton.h"
#include "World.h"


MainWindow::MainWindow(bool isMaximized, QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow),
	  theLevelPtr(NULL),
	  theWorldPtr(NULL)
{
    ui->setupUi(this);

    setupView();
    if (isMaximized)
        showMaximized();
    if (theIsLevelEditor)
        on_action_Switch_to_Level_Editor_activated();
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


void MainWindow::loadLevelDelayed()
{
	DEBUG1ENTRY;
	loadLevel(theStartFileName);
}


void MainWindow::on_action_About_activated()
{
    //: translators: <b> and <br> are statements for bold and newline, respectively
    Popup::Info(tr("<b>The Butterfly Effect</b><br><br>"
                   "An open source game that uses realistic physics"
                   " simulations to combine lots of simple mechanical elements"
                   " to achieve a simple goal in the most complex way possible.<br><br>"
                   "(C) 2009,2010,2011,2012,2013 Peter van Ginneken and Klaas van Gend<br>"
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


void MainWindow::on_action_Keyboard_Shortcuts_activated()
{
    //: translators: <b> and <br> are statements for bold and newline, respectively, please make sure to
    //: please make sure to match the statements in this dialog with your shortcuts
    Popup::Info(tr("<b>The Butterfly Effect - Keyboard shortcuts</b><br><br>"
                   "The following keys can be used to accelerate actions you'd have to do with the mouse otherwise"
                   "<table cellpadding=\"4\">"
                   "<tr><th align=\"left\">Key</th><th align=\"left\">Function </th></tr> "
                   "<tr><td>Space</td><td>start / stop simulation </td></tr> "
                   "<tr><td>f</td><td>(during simulation) fast forward / slowd down </td></tr> "
                   "<tr><td>shift+f</td><td>(during simulation) really fast forward </td></tr> "
                   "<tr><td>r</td><td>reset the simulation </td></tr> "
                   "<tr><td>t</td><td>show /hide toolbox </td></tr> "
                   "<tr><td>Ctrl-Z</td><td> undo last action </td></tr> "
                   "<tr><td>Ctrl-Y</td><td> redo last action </td></tr> "
                   "</table>"), this);
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


void MainWindow::on_action_New_activated(void)
{
}


void MainWindow::on_action_New_Level_Ideas_activated(void)
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


void MainWindow::on_action_Open_Level_triggered()
{
    ChooseLevel* myDialogPtr = new ChooseLevel(ui->graphicsView);
    connect(myDialogPtr, SIGNAL(loadLevel(QString)),
            this, SLOT(loadLevel(QString)));
}


void MainWindow::on_action_Open_File_activated(void)
{
    QString myFileName = QFileDialog::getOpenFileName(this,
                                                      tr("Open level"), ".", tr("TBE levels (*.xml)"));
    if (myFileName.isEmpty())
        return;
    loadLevel(myFileName);
}


void MainWindow::on_action_Quit_activated(void)
{
    if (Popup::YesNoQuestion(tr("really?"), this))
        QApplication::exit(0);
}


void MainWindow::on_action_Save_activated(void)
{
    DEBUG1ENTRY;
    if (theLevelPtr==NULL)
        return;

    QFileInfo myFileInfo(theLevelPtr->getLevelFileName());
    if (theLevelPtr->save(myFileInfo.absoluteFilePath())==false)
        Popup::Warning(tr("File '%1' could not be saved.").arg(myFileInfo.absoluteFilePath()));
    else
        DEBUG2("File '%s' saved.",myFileInfo.absoluteFilePath().toAscii().constData());
}


void MainWindow::on_action_Save_As_activated(void)
{
    DEBUG1ENTRY;
    assert(theLevelPtr);
    if (theLevelPtr==NULL)
        return;

    SaveLevelInfo mySaveLevel(theLevelPtr,this);
    int myReturnCode = mySaveLevel.exec();
    if (myReturnCode == QDialog::Rejected)
        return;

    // for now we ignore the return code...
    if (mySaveLevel.commitToLevel()==false)
        Popup::Warning(tr("You did not fill in all fields - but level saved anyway\n"));

    QFileInfo myFileInfo(theLevelPtr->getLevelFileName());

    DEBUG5("File '%s' is readable: %d, writeable: %d\n",
           ASCII(myFileInfo.absoluteFilePath()),
           myFileInfo.isReadable(), myFileInfo.isWritable());

    on_action_Save_activated();

}


void MainWindow::on_action_Skip_Level_activated(void)
{
    if (Popup::YesNoQuestion(tr("Mark this level 'skipped' and continue with the next level?"), this))
        ui->graphicsView->slot_actionSkipLevel();
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


void MainWindow::on_action_Switch_to_Level_Editor_activated()
{
    // add "New", "Save" and "Save as" items to File menu
    typedef QList<QAction*> ActionList;
    ActionList myList = ui->menuFile->actions();
    for (auto i : myList)
    {
        if (!i->isVisible())
        {
            i->setVisible(true);
            i->setEnabled(true);
        }
        if (i->data()=="Switch")
        {
            i->setVisible(false);
            i->setEnabled(false);
        }
    }

    // add new drop-down menu "Insert" (and put it before the "Controls" menu)
    QMenu* myInsertMenuPtr = new QMenu(tr("&Insert"), NULL);
    ui->menuBar->insertMenu(ui->menuControls->menuAction(), myInsertMenuPtr);
    // add all objects into it
    ObjectFactory::ObjectFactoryList* myOFListPtr = ObjectFactory::getAllFactories();
    for (auto i : *myOFListPtr)
    {
        // TODO: remove the Link-derived ones from the list
        // TODO: add icons to the action
        InsertMenuQAction* myTempActionPtr = new InsertMenuQAction(i->getFactoryName(), NULL);
        connect(myTempActionPtr, SIGNAL(triggeredName(QString)), this, SLOT(on_insert(QString)));
        myInsertMenuPtr->addAction(myTempActionPtr);
    }
    delete myOFListPtr;
    myOFListPtr = NULL;

    // add new top menu "Editors"
    QMenu* myEditorsMenuPtr = new QMenu(tr("&Editors"), NULL);
    ui->menuBar->insertMenu(ui->menuControls->menuAction(), myEditorsMenuPtr);
    // TODO: add some of the original dialogs to it
    QAction* myGoalActionPtr = new QAction(tr("&Goal Editor..."), NULL);
    connect (myGoalActionPtr, SIGNAL(triggered()), this, SLOT(on_goalEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myGoalActionPtr);
    QAction* myLevPropActionPtr = new QAction(tr("&Level Properties..."), NULL);
    connect (myLevPropActionPtr, SIGNAL(triggered()), this, SLOT(on_levelPropertiesEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myLevPropActionPtr);
    QAction* myEditObjectActionPtr = new QAction(tr("&Object Editor..."), NULL);
    connect (myEditObjectActionPtr, SIGNAL(triggered()), this, SLOT(on_objectEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myEditObjectActionPtr);
    // Enable level editor mode
    theIsLevelEditor = true;
    // TODO: it would be marvellous to have Cut/Copy/Paste in the Edit menu!
}


void MainWindow::on_goalEditorAction_clicked(void)
{
    // the Goals dialog is modeless, i.e. it can stay floating around
    GoalEditor* myGoalEditorPtr = new GoalEditor(theLevelPtr->getTheWorldPtr(), this);
    myGoalEditorPtr->show();
}


void MainWindow::on_insert(const QString& anObjectName)
{
    DEBUG1ENTRY;
    InsertUndoCommand::createInsertUndoCommand(
                ObjectFactory::createObject(anObjectName, Position(1,1)));
}


void MainWindow::on_levelPropertiesEditorAction_clicked(void)
{
    // this dialog is modal, i.e. user has to click OK/Cancel
    EditLevelProperties* myEditorPtr = new EditLevelProperties(theLevelPtr, this);
    myEditorPtr->show();
    // I don't care about the return.
    // If it was OK, the dialog already 'fixed' everything.
    myEditorPtr->exec();
}


void MainWindow::on_objectEditorAction_clicked(void)
{
    emit dynamic_cast<ResizingGraphicsView*>(ui->graphicsView)->slot_showEditObjectDialog(NULL);
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


void MainWindow::setupView()
{
    // setup UndoGroup's QActions and add them to Edit menu
    // note that this doesn't enable them yet, our ViewWorld should handle that...
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
        theStartFileName = ChooseLevel::getNextLevelName();

    QTimer::singleShot(200, this, SLOT(loadLevelDelayed()));
}
