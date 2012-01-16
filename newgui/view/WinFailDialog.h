/* The Butterfly Effect
 * This file copyright (C) 2012 Klaas van Gend
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

#ifndef WINFAILDIALOG_H
#define WINFAILDIALOG_H

//#include "resizinggraphicsview.h"
#include "animateddialog.h"
#include <QWidget>

namespace Ui {
class WinFailDialog;
}

/** This dialog is shown whenever the user fails or wins a level.
  * It contains options to go to the next level, to replay or to choose a
  * specific level.
  */
class WinFailDialog : public AnimatedDialog
{
    Q_OBJECT

public:
	enum MessageType
	{
		CONGRATS,
		DEATH
	};

    explicit WinFailDialog(MessageType aType, ResizingGraphicsView* aParent = 0);
    ~WinFailDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WinFailDialog *ui;

    ResizingGraphicsView* theParentPtr;
};

#endif // WINFAILDIALOG_H
