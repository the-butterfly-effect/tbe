/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "LevelCreator.h"
#include "ui_MainWindow.h"

#include "EditLevelProperties.h"
#include "EditObjectDialog.h"
#include "GoalEditor.h"
#include "ImageCache.h"
#include "InsertUndoCommand.h"
#include "Level.h"
#include "ObjectFactory.h"

#include <QToolBar>
#include <QTabWidget>

LevelCreator::LevelCreator(MainWindow *aParent) :
    QObject(aParent), theMWPtr(aParent), ui(aParent->ui)
{
    theLevelCreatorToolbarPtr = theMWPtr->addToolBar(tr("LevelCreator"));
    theLevelCreatorToolbarPtr->setMovable(false);

    // add "New", "Save" and "Save as" items to File menu
    typedef QList<QAction *> ActionList;
    ActionList myList = ui->menuFile->actions();
    for (auto i : myList) {
        if (!i->isVisible()) {
            i->setVisible(true);
            i->setEnabled(true);
        }
        if (i->data() == "Switch") {
            i->setVisible(false);
            i->setEnabled(false);
        }
    }
    theLevelCreatorToolbarPtr->addAction(ui->action_Open_File);
    theLevelCreatorToolbarPtr->addAction(ui->action_Save);
    theLevelCreatorToolbarPtr->addAction(ui->action_Save_As);
    theLevelCreatorToolbarPtr->addSeparator();

    // add new items to top menu "Edit"
    QAction *myCloneActionPtr = new QAction(tr("&Clone object"), nullptr);
    QIcon myTmpIcon  = ImageCache::getQIcon("ActionClone", QSize(64, 64));
    myCloneActionPtr->setIcon(myTmpIcon);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(myCloneActionPtr);
    connect (myCloneActionPtr, SIGNAL(triggered()), this, SLOT(on_action_Clone_triggered()));
    theCollisionOffActionPtr = new QAction(tr("&Collision OK"), nullptr);
    myTmpIcon  = ImageCache::getQIcon("ActionCollisionOK", QSize(64, 64));
    theCollisionOffActionPtr->setIcon(myTmpIcon);
    theCollisionOffActionPtr->setCheckable(true);
    theCollisionOffActionPtr->setChecked(!theIsCollisionOn);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(theCollisionOffActionPtr);
    connect (theCollisionOffActionPtr, SIGNAL(triggered()), this,
             SLOT(on_action_CollisionOff_triggered()));
    theCollisionOnActionPtr = new QAction(tr("&Prevent Collision"), nullptr);
    myTmpIcon  = ImageCache::getQIcon("ActionCollisionWrong", QSize(64, 64));
    theCollisionOnActionPtr->setIcon(myTmpIcon);
    theCollisionOnActionPtr->setCheckable(true);
    theCollisionOnActionPtr->setChecked(theIsCollisionOn);
    ui->menuEdit->addAction(theCollisionOnActionPtr);
    connect (theCollisionOnActionPtr, SIGNAL(triggered()), this,
             SLOT(on_action_CollisionOn_triggered()));
    theLevelCreatorToolbarPtr->addActions(ui->menuEdit->actions());
    theLevelCreatorToolbarPtr->addSeparator();

    // add new drop-down menu "Insert" (and put it before the "Controls" menu)
    QMenu *myInsertMenuPtr = new QMenu(tr("&Insert"), nullptr);
    ui->menuBar->insertMenu(ui->menuControls->menuAction(), myInsertMenuPtr);
    // add all objects into it
    ObjectFactory::ObjectFactoryList *myOFListPtr = ObjectFactory::getAllFactories();
    for (auto i : *myOFListPtr) {
        // Remove some objects without ViewObject from the list
        if (i->isObjectForInsertList()) {
            // TODO: add icons to the action
            InsertMenuQAction *myTempActionPtr = new InsertMenuQAction(i->getFactoryName(), nullptr);
            connect(myTempActionPtr, SIGNAL(triggeredName(QString)), this, SLOT(on_insert(QString)));
            myInsertMenuPtr->addAction(myTempActionPtr);
        }
    }
    delete myOFListPtr;
    myOFListPtr = nullptr;

    // add new top menu "Editors"
    QMenu *myEditorsMenuPtr = new QMenu(tr("E&ditors"), nullptr);
    ui->menuBar->insertMenu(ui->menu_Help->menuAction(), myEditorsMenuPtr);
    // TODO: add some of the original dialogs to it
    QAction *myGoalActionPtr = new QAction(tr("&Goal Editor..."), nullptr);
    myTmpIcon = ImageCache::getQIcon("ActionGoals", QSize(64, 64));
    myGoalActionPtr->setIcon(myTmpIcon);
    connect (myGoalActionPtr, SIGNAL(triggered()), this, SLOT(on_goalEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myGoalActionPtr);
    QAction *myLevPropActionPtr = new QAction(tr("&Size && Background Editor..."), nullptr);
    connect (myLevPropActionPtr, SIGNAL(triggered()), this,
             SLOT(on_levelPropertiesEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myLevPropActionPtr);
    myLevPropActionPtr->setIcon(QIcon::fromTheme("document-properties"));
    QAction *myLevNameActionPtr = new QAction(tr("&Name && Description Editor..."), nullptr);
    myLevNameActionPtr->setEnabled(false);
    myLevNameActionPtr->setIcon(QIcon::fromTheme("accessories-text-editor"));
    //TODO: connect (myLevNameActionPtr, SIGNAL(triggered()), this, SLOT(on_levelNameEditorAction_clicked()));
    myEditorsMenuPtr->addAction(myLevNameActionPtr);
    theLevelCreatorToolbarPtr->addActions(myEditorsMenuPtr->actions());
    theLevelCreatorToolbarPtr->addSeparator();

    // add new top menu "View"
    QMenu *myViewMenuPtr = new QMenu(tr("&View"), nullptr);
    ui->menuBar->insertMenu(ui->menu_Help->menuAction(), myViewMenuPtr);
    theDrawDebugActionPtr = new QAction(tr("&Draw Debug"), nullptr);
    myTmpIcon  = ImageCache::getQIcon("ActionDrawDebug", QSize(64, 64));
    theDrawDebugActionPtr->setCheckable(true);
    theDrawDebugActionPtr->setChecked(theDrawDebug);
    theDrawDebugActionPtr->setIcon(myTmpIcon);
    connect (theDrawDebugActionPtr, SIGNAL(triggered()), this, SLOT(on_action_DrawDebug_triggered()));
    myViewMenuPtr->addAction(theDrawDebugActionPtr);
    theDrawNormalActionPtr = new QAction(tr("&Draw Normal"), nullptr);
    myTmpIcon  = ImageCache::getQIcon("ActionDrawNormal", QSize(64, 64));
    theDrawNormalActionPtr->setCheckable(true);
    theDrawNormalActionPtr->setChecked(!theDrawDebug);
    theDrawNormalActionPtr->setIcon(myTmpIcon);
    connect (theDrawNormalActionPtr, SIGNAL(triggered()), this, SLOT(on_action_DrawNormal_triggered()));
    myViewMenuPtr->addAction(theDrawNormalActionPtr);
    theLevelCreatorToolbarPtr->addActions(myViewMenuPtr->actions());
    theLevelCreatorToolbarPtr->addSeparator();

    // Enable level editor mode
    theIsLevelCreator = true;
    ui->action_Switch_to_Level_Editor->setEnabled(false);
    ui->action_Open_File->setEnabled(true);
    ui->action_Open_File->setVisible(true);

    // Add EditObject pane to the UI
    auto myToolboxPtr = ui->listWidget;
    ui->verticalLayout->removeWidget(myToolboxPtr);
    auto myButtonsPtr = ui->theInfoEjectLayout;
    ui->verticalLayout->removeItem(myButtonsPtr);
    QTabWidget *myTabWidgetPtr = new QTabWidget();
    theEditObjectDialogPtr = new EditObjectDialog();
    myTabWidgetPtr->addTab(theEditObjectDialogPtr, tr("Object Properties"));
    myTabWidgetPtr->addTab(myToolboxPtr, tr("Toolbox"));
    ui->verticalLayout->addWidget(myTabWidgetPtr);
    ui->verticalLayout->addItem(myButtonsPtr);
    myTabWidgetPtr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}


void LevelCreator::on_action_Clone_triggered()
{
    DEBUG1ENTRY;
    // yay, we need to be careful here - theSelectedAOWeakPtr can have expired
    try {
        AbstractObjectPtr myAOPtr(theSelectedAOWeakPtr);
        if (nullptr == myAOPtr)
            return;
        Vector myDelta(0.5 * myAOPtr->getTheWidth(), -0.5 * myAOPtr->getTheHeight());
        AbstractObjectPtr myClonePtr = ObjectFactory::cloneObject(myAOPtr);
        myClonePtr->setOrigCenter(myAOPtr->getOrigCenter() + myDelta);
        InsertUndoCommand::createInsertUndoCommand(myClonePtr);
    } catch (std::bad_weak_ptr) {
        DEBUG1("ERROR: on_action_Clone_triggered() for outdated object...");
        return;
    }
}


void LevelCreator::on_action_CollisionOff_triggered()
{
    theIsCollisionOn = false;
    theCollisionOnActionPtr->setChecked(theIsCollisionOn);
    theCollisionOffActionPtr->setChecked(!theIsCollisionOn);
}


void LevelCreator::on_action_CollisionOn_triggered()
{
    theIsCollisionOn = true;
    theCollisionOnActionPtr->setChecked(theIsCollisionOn);
    theCollisionOffActionPtr->setChecked(!theIsCollisionOn);
}


void LevelCreator::on_action_DrawDebug_triggered()
{
    theDrawDebug = true;
    theDrawDebugActionPtr->setChecked(theDrawDebug);
    theDrawNormalActionPtr->setChecked(!theDrawDebug);
}


void LevelCreator::on_action_DrawNormal_triggered()
{
    theDrawDebug = false;
    theDrawDebugActionPtr->setChecked(theDrawDebug);
    theDrawNormalActionPtr->setChecked(!theDrawDebug);
}


void LevelCreator::on_goalEditorAction_clicked()
{
    // the Goals dialog is modeless, i.e. it can stay floating around
    GoalEditor *myGoalEditorPtr = new GoalEditor(theMWPtr->theLevelPtr->getTheWorldPtr(), theMWPtr);
    myGoalEditorPtr->show();
}


void LevelCreator::on_insert(const QString &anObjectName)
{
    DEBUG1ENTRY;
    // strip out ampersands in the name
    QString myFilteredName = anObjectName;
    myFilteredName.remove('&');
    InsertUndoCommand::createInsertUndoCommand(
        ObjectFactory::createObject(myFilteredName, Position(1, 1)));
}

void LevelCreator::on_levelPropertiesEditorAction_clicked()
{
    // this dialog is modal, i.e. user has to click OK/Cancel
    EditLevelProperties *myEditorPtr = new EditLevelProperties(theMWPtr->theLevelPtr, theMWPtr);
    myEditorPtr->show();
    // I don't care about the return.
    // If it was OK, the dialog already 'fixed' everything.
    myEditorPtr->exec();
}


void LevelCreator::on_objectEditorAction_clicked()
{
    // TODO: this cannot be true: first of all, can cause a nullptr dereference,
    // secondly, how can you enable an objectcreator by passing a nullptr???
    //emit dynamic_cast<ResizingGraphicsView*>(ui->graphicsView)->slot_updateObjectDialog(nullptr);
}


void LevelCreator::slot_updateEditObjectDialog(AbstractObjectPtr anAOPtr)
{
    theSelectedAOWeakPtr = anAOPtr;
    emit theEditObjectDialogPtr->updateAbstractObjectPtr(anAOPtr);
}
