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

// forward declarations
class DrawWorld;

class MainWindow : public QMainWindow
{
     Q_OBJECT

public:
     MainWindow(QWidget *parent = 0);

     void setScene(DrawWorld* aScene);
     
private slots:
	void on_actionAbout_activated(void);
    void on_pushButton_Start_clicked(void);
    void on_pushButton_Stop_clicked(void);

private:
     Ui::MainWindow ui;
};

 
#endif /* MAINWINDOW_H_ */
