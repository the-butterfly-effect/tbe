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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class Level;
class QGraphicsRectWidget;
class QGraphicsScene;
class ViewWorld;
class World;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool isMaximized, QWidget *parent = 0);
    ~MainWindow();

	/// deletes the existing Level instance and removes its view
	void purgeLevel(void);

protected:
	void changeEvent(QEvent *e);

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

	// menu Help
	void on_action_About_activated(void);
	void on_action_Bug_Reports_activated(void);
	void on_action_Libraries_activated(void);
	void on_action_New_Levels_activated(void);
	void on_actionO_pen_File_activated(void);
	void on_action_Quit_activated(void);
	void on_action_Suggestions_activated(void);

	// menu File
	void on_action_Open_Level_triggered();

private:
	Ui::MainWindow *ui;

	/// Pointer to the current level.
	Level* theLevelPtr;

	World* theWorldPtr;
};

#endif // MAINWINDOW_H
