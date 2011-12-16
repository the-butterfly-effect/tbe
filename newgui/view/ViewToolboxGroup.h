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

#ifndef VIEWTOOLBOXGROUP_H
#define VIEWTOOLBOXGROUP_H

#include <QGraphicsRectItem>
#include "ToolboxGroup.h"

#include <QGraphicsTextItem>

class ViewToolboxGroup : public QGraphicsRectItem
{
public:
    ViewToolboxGroup(ToolboxGroup* aTBGPtr, QGraphicsItem *parent = 0);

    int getBigHeight(void)
    { return theBigHeight; }

protected:
//    void updateCount(void);

    /// overridden to allow object highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent* event );
    /// overridden to allow highlighting
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent* event );

private:
    ToolboxGroup* theTBGPtr;

    int theBigHeight;
    int theBigWidth;
    QGraphicsTextItem theCount;
    QGraphicsTextItem theName;
    QGraphicsPixmapItem* thePixmapPtr;
};

#endif // VIEWTOOLBOXGROUP_H
