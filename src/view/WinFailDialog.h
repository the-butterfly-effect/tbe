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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef WINFAILDIALOG_H
#define WINFAILDIALOG_H

#include "animateddialog.h"

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
    enum MessageType {
        CONGRATS,
        DEATH
    };

    explicit WinFailDialog(MessageType aType, ResizingGraphicsView *aParent = 0);
    virtual ~WinFailDialog();

protected:
    void changeEvent(QEvent *e) override;

private:
    Ui::WinFailDialog *ui;
};

#endif // WINFAILDIALOG_H
