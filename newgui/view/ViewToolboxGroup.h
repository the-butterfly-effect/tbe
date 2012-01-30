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

#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsTextItem>
#include "ToolboxGroup.h"
#include "UndoSingleton.h"

/** This class represents the 'button' for one or more objects
  * the user can select from the toolbox to add to the scene.
  */
class ViewToolboxGroup : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    ViewToolboxGroup(ToolboxGroup* aTBGPtr, QGraphicsItem *parent = 0);

    virtual ~ViewToolboxGroup();

    int getBigHeight(void)
    { return theBigHeight; }
    int getBigWidth(void)
    { return theBigWidth; }

    void updateCount(void);

protected:

    /// @note: overridden from qGraphicsRectItem to allow object highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent* event );
    /// @note: overridden from qGraphicsRectItem to allow highlighting
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent* event );

    /// @note: overridden from qGraphicsRectItem to detect mouse button presses
    /// When an object is clicked that still has objects left, insert one
    /// into the scene (through the creation of an UndoInsert)
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event);

signals:
    void hideMe();

private:
    ToolboxGroup* theTBGPtr;

    int theBigHeight;
    int theBigWidth;
    QGraphicsTextItem theCount;
    QGraphicsTextItem theName;
    QGraphicsTextItem theEmpty;
    QGraphicsPixmapItem* thePixmapPtr;
};

#endif // VIEWTOOLBOXGROUP_H
