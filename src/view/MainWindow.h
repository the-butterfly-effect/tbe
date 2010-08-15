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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ui_MainWindow.h"
#include <QUndoGroup>

// forward declarations
class DrawWorld;
class Level;
class QPushButton;

class MainWindow : public QMainWindow
{
     Q_OBJECT

public:
     MainWindow(QWidget *parent = 0);
	 virtual ~MainWindow();

     void setScene(DrawWorld* aScene, const QString& aLevelName);

private slots:
	void on_actionAbout_activated(void);
	void on_actionBrand_names_activated(void);
	void on_actionLibraries_activated(void);
	void on_actionOpen_custom_level_activated(void);
	void on_actionOpen_level_activated(void);

	// write the current level layout to disk
	void on_actionSave_activated(void);

	void slot_splashScreen_clicked(void);

	void on_action_quarter_speed_activated(void)
	{ setSimSpeed(0.25); }
	void on_action_half_speed_activated(void)
	{ setSimSpeed(0.5); }
	void on_action_normal_speed_activated(void)
	{ setSimSpeed(1); }
	void on_action_double_speed_activated(void)
	{ setSimSpeed(2.0); }

	/// called internally by all inside graphicsview
	void slot_clear_buttons(void);

	/// called by DrawWorld once alls goals are met
	void slot_levelWon(void);

	/// called internally by pushbutton "Next>" in slot_levelWon
	void slot_next_level(void);

private:
	Ui::MainWindow ui;

	Level* theLevelPtr;
	DrawWorld* theScenePtr;

	QUndoGroup theUndoGroup;
	QAction*	theUndoActionPtr;
	QAction*	theRedoActionPtr;
	QPushButton* theButtons[3];

	qreal theSimSpeed;

	/** loads the level specified by the file name
	 *
	 * @param aFileName guess what: the file name. Duh.
	 */
	void loadLevel(const QString& aFileName);

	/// deletes the existing Level instance and removes its view
	void purgeLevel(void);

	/// sets the simulation speed
	void setSimSpeed(qreal aSpeed);
};

 
#endif /* MAINWINDOW_H_ */
