/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#ifndef GOALEDITOR_H
#define GOALEDITOR_H

#include <QtGui/QDialog>

namespace Ui {
    class GoalEditor;
}

class GoalEditor : public QDialog {
    Q_OBJECT
public:
    GoalEditor(QWidget *parent = 0);
    ~GoalEditor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GoalEditor *m_ui;
};

#endif // GOALEDITOR_H
