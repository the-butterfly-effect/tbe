/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef CHOOSEPHONENUMBER_H
#define CHOOSEPHONENUMBER_H

#include "animateddialog.h"

class DetonatorBox;

namespace Ui {
class ChoosePhoneNumber;
}

class ChoosePhoneNumber : public AnimatedDialog
{
    Q_OBJECT
public:
    ChoosePhoneNumber(DetonatorBox *aBoxPtr, ResizingGraphicsView *aParentPtr = 0);
    ~ChoosePhoneNumber();

public slots:
    /// all magic is done in this slot
    void on_comboBox_activated();

protected:
    virtual void changeEvent(QEvent *e) override;

private:
    Ui::ChoosePhoneNumber *m_ui;
    DetonatorBox *theDBPtr;
};

#endif // CHOOSEPHONENUMBER_H
