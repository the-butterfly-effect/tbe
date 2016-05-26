/* The Butterfly Effect
 * This file copyright (C) 2011,2013,2014,2015 Klaas van Gend
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QListWidget>
#include <QMainWindow>
#include <QTimer>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class GameStateMachine;
class Level;
class LevelCreator;
class QGraphicsRectWidget;
class QGraphicsScene;
class QToolbar;
class RegressionTest;
class ViewWorld;
class World;

/** Local class, specifically meant to have menu items in the "Insert" menu
  * (i.e. our game's objects) that can report their object name to our
  * MainWindow.
  */
class InsertMenuQAction : public QAction
{
    Q_OBJECT
public:
    InsertMenuQAction( const QString &text, QObject *parent )
        : QAction (text, parent)
    {
        connect (this, SIGNAL(triggered()), this, SLOT(sendTriggerName()));
    }

public slots:
    /// will be connected
    void sendTriggerName()
    {
        emit triggeredName(text());
    }

signals:
    void triggeredName(const QString &aName);
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool isMaximized, QWidget *parent = 0);
    ~MainWindow();

    static const QString getWelcomeMessage();

    /// deletes the existing Level instance and removes its view
    void purgeLevel();

    RegressionTest *theRegressionTest;
    LevelCreator    *theLevelCreator;

protected:
    virtual void changeEvent(QEvent *e) override;

    /// Internal function to redo scene.
    void repopulateScene();
    /// Internal function to redo toolbox.
    void repopulateToolbox();

    /// Adds dynamic menus, i.e. 'Edit' and 'Languages'.
    void setupView();

public slots:
    /// Loads the level specified by the file name.
    /// @param aFileName guess what: the file name. Duh.
    void loadLevel(const QString &aFileName);

    void loadLevelDelayed();

    /// Restarts the level already active.
    /// Because we have the filename already, no need to specify here.
    void reloadLevel();

    /// To be called when the cursor changed position in the graphicsview,
    /// in order to display the coordinates.
    /// x and y hold the world position of the cursor
    void slot_mouse_move(qreal x, qreal y);


private slots:
    /// Inserts one of the hints from the level into the Scene.
    /// @note: for now, this is an internal function, usable by the
    ///        regression system only. To make it work for users, we need
    ///        a lot more 'weird' handling, like applying a hint for an object
    ///        that is already in the scene (i.e. not insert but move)
    /// @note: internally uses the InsertUndoCommand mechanism
    bool slot_insertHint(unsigned int aHintNumber) const;

    // menu Help
    void on_action_About_triggered();
    void on_action_Bug_Reports_triggered();
    void on_action_Keyboard_Shortcuts_triggered();
    void on_action_Libraries_triggered();
    void on_action_New_Level_Ideas_triggered();
    void on_action_Quit_triggered();
    void on_action_Skip_Level_triggered();
    void on_action_Suggestions_triggered();

    // menu File
    void on_action_New_triggered();
    void on_action_Open_File_triggered();
public slots:
    void on_action_Open_Level_triggered();
private slots:
    void on_action_Reload_triggered();
    void on_action_Save_triggered();
    void on_action_Save_As_triggered();
    void on_action_Switch_to_Level_Editor_triggered();

    // menu Languages
    void on_switchLanguage(QString);

public slots:
    void slot_actionNextLevel();

private:
    /// set a checkmark in front of the appropriate language in the menu
    void setLanguageCheckmark();

private:
    Ui::MainWindow *ui;

    /// Pointer to the current level.
    Level *theLevelPtr;

    World *theWorldPtr;

    /// Label showing the world coordinates of the cursor. Only used in level creator
    QLabel *theMousePosLabelPtr;

    /// This ActionGroup ensures that only one language in the menu is checked.
    QActionGroup theLanguagesGroup;

    GameStateMachine *theGameStateMachinePtr;

    friend class LevelCreator;
    friend class RegressionTest;
};

#endif // MAINWINDOW_H
