/* The Butterfly Effect
 * This file copyright (C) 2014 Klaas van Gend
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

#ifndef TOOLBOXLISTWIDGETITEM_H
#define TOOLBOXLISTWIDGETITEM_H

#include "resizinggraphicsview.h"
#include "ToolboxGroup.h"

#include <QListWidgetItem>
#include <QObject>

class ToolboxListWidgetItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    explicit ToolboxListWidgetItem(ResizingGraphicsView* aRSGVPtr,
                                   ToolboxGroup* aTBGPtr,
                                   QListWidget *parent = 0);

signals:

public slots:
    void slotSelected(QListWidgetItem*);

private:
    ToolboxGroup* theTBGPtr;
    ResizingGraphicsView* theRSGVPtr;
    
};

#endif // TOOLBOXLISTWIDGETITEM_H
