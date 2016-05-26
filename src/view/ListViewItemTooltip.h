/* The Butterfly Effect
 * This file copyright (C) 2011,2014 Klaas van Gend
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

#ifndef LISTVIEWITEMTOOLTIP_H
#define LISTVIEWITEMTOOLTIP_H

#include "animateddialog.h"
#include "ToolboxGroup.h"
#include "resizinggraphicsview.h"
//#include <QWidget>

namespace Ui {
class ListViewItemTooltip;
}

class ListViewItemTooltip : public AnimatedDialog
{
    Q_OBJECT

public:
    /// explicit constructor
    /// @param aTBGPtr
    /// @param parent   resizinggraphicsview*
    explicit ListViewItemTooltip(
        ToolboxGroup *aTBGPtr,
        ResizingGraphicsView *parent = 0);
    ~ListViewItemTooltip();

    /// @param aVPos    vertical position in pixels to align top of tooltip to
    void adjustVPos(int aVPos);

public slots:
    void on_buttonRemove_clicked();
    void on_buttonObjectImage_clicked();

private:
    /// TODO
    void addActionIcon(const QString &anIconName, const QString &aToolTip);

    Ui::ListViewItemTooltip *ui;
    ToolboxGroup *theTBGPtr;

    ResizingGraphicsView *thRSGVPtr;
};

#endif // LISTVIEWITEMTOOLTIP_H
