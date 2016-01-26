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

#ifndef LEVELCREATOR_H
#define LEVELCREATOR_H

#include <QObject>
#include "MainWindow.h"

/// class handling
class LevelCreator : public QObject
{
    Q_OBJECT
public:
    explicit LevelCreator(MainWindow* aParent);

    // note how there's no public members here :-)

signals:

public slots:
    // menu Insert (Level Creator)
    void on_insert(const QString &anObjectName);

    // menu Editors (Level Creator)
    void on_goalEditorAction_clicked();
    void on_levelPropertiesEditorAction_clicked();
    void on_objectEditorAction_clicked();

    // menu Edit (Level Creator)
    void on_action_Clone_triggered();
    void on_action_DrawDebug_triggered();
    void on_action_DrawNormal_triggered();
    void on_action_CollisionOff_triggered();
    void on_action_CollisionOn_triggered();

private:
    MainWindow* theMWPtr;
    Ui::MainWindow *ui;

    QToolBar* theLevelCreatorToolbarPtr;

    QAction* theCollisionOffActionPtr;
    QAction* theCollisionOnActionPtr;

    QAction* theDrawDebugActionPtr;
    QAction* theDrawNormalActionPtr;

};

#endif // LEVELCREATOR_H
