/* The Butterfly Effect
 * This file copyright (C) 2013 Klaas van Gend
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

#ifndef ToolboxGroupIcon_H
#define ToolboxGroupIcon_H

#include <QtGui/QtGui>
#include "ToolboxGroup.h"

class GameResources;

/** This class represents the 'button' for one or more objects
  * the user can select from the toolbox to add to the scene.
  */
class ToolboxGroupIcon : public QWidget
{
    Q_OBJECT

public:
    ToolboxGroupIcon(ToolboxGroup* aTBGPtr,
                     GameResources* aGRPtr,
                     QWidget* aParentPtr=0);

    void updateCount(void);

    /// overridden in order to actually paint something
    virtual void paintEvent ( QPaintEvent *  );

    QSize getSize(void)
    { return theIconSize; }

private:
    ToolboxGroup* theTBGPtr;
    QTransform theTransformMatrix;

    QPixmap thePixmap;
    QSize theIconSize;
    qreal theOriginalM11;
    QSize theMinSize;

    int max2(int a, int b)
    { return (a >= b) ? a : b; }
};

#endif // ToolboxGroupIcon_H
