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

#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QListWidget>
#include <QtCore/QTimer>

namespace Ui {
class MainWindow;
}

class Level;
class QGraphicsRectWidget;
class QGraphicsScene;
class RegressionTest;
class ViewWorld;
class World;


/** local class, specifically meant to have menu items that can report
  * their name to our MainWindow
  */
class InsertMenuQAction : public QAction
{
    Q_OBJECT
public:
    InsertMenuQAction( const QString & text, QObject * parent )
        : QAction (text, parent)
    {
        connect (this, SIGNAL(triggered()), this, SLOT(sendTriggerName()));
    }

public slots:
    /// will be connected
    void sendTriggerName() {
        emit triggeredName(text());
    }

signals:
    void triggeredName(const QString& aName);
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(bool isMaximized, QWidget *parent = 0);
    ~MainWindow();

    /// deletes the existing Level instance and removes its view
    void purgeLevel(void);

	RegressionTest* theRegressionTest;

protected:
	virtual void changeEvent(QEvent *e) override;

    void setupMenu(void);
    void setupView(void);

public slots:
    /// Loads the level specified by the file name.
    /// @param aFileName guess what: the file name. Duh.
    void loadLevel(const QString& aFileName);

    void loadLevelDelayed(void);

    /// Restarts the level already active.
    /// Because we have the filename already, no need to specify here.
    void reloadLevel(void);

private slots:
	/// Inserts one of the hints from the level into the Scene.
	/// @note: for now, this is an internal function, usable by the
	///        regression system only. To make it work for users, we need
	///        a lot more 'weird' handling, like applying a hint for an object
	///        that is already in the scene (i.e. not insert but move)
	/// @note: internally uses the InsertUndoCommand mechanism
	bool slot_insertHint(unsigned int aHintNumber) const;

	// menu Help
    void on_action_About_activated(void);
    void on_action_Bug_Reports_activated(void);
    void on_action_Keyboard_Shortcuts_activated(void);
    void on_action_Libraries_activated(void);
    void on_action_New_Level_Ideas_activated(void);
    void on_action_Quit_activated(void);
    void on_action_Skip_Level_activated(void);
    void on_action_Suggestions_activated(void);

    // menu File
    void on_action_New_activated(void);
    void on_action_Open_File_activated(void);
public slots:
    void on_action_Open_Level_triggered();
private slots:
    void on_action_Save_activated(void);
    void on_action_Save_As_activated(void);
    void on_action_Switch_to_Level_Editor_activated();

#ifdef QT_DEBUG
    // menu Insert (Level Creator)
    void on_insert(const QString &anObjectName);

    // menu Editors (Level Creator)
    void on_goalEditorAction_clicked(void);
    void on_levelPropertiesEditorAction_clicked(void);
    void on_objectEditorAction_clicked(void);
#endif

private:
    Ui::MainWindow *ui;

    /// Pointer to the current level.
    Level* theLevelPtr;

    World* theWorldPtr;

	friend class RegressionTest;
};

#endif // MAINWINDOW_H
