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

#include <QtGui/QtGui>
#include "ToolboxGroup.h"

class GameResources;

/** This class represents the 'button' for one or more objects
  * the user can select from the toolbox to add to the scene.
  */
class ViewToolboxGroup : public QPushButton
{
    Q_OBJECT

public:
    ViewToolboxGroup(ToolboxGroup* aTBGPtr,
                     GameResources* aGRPtr,
                     QWidget* aParentPtr=0);

    void updateCount(const QTransform& aTransformMatrix);

//    virtual QSize minimumSizeHint () const
//    { return theMinSize; }
//    virtual QSize sizeHint () const
//    { return theMinSize; }

signals:
    void hideMe();

private slots:
    void onClicked ( void );

private:
    ToolboxGroup* theTBGPtr;
    QLabel theCountLabel;
    QLabel theDescriptionLabel;
    QVBoxLayout theVBoxLayout;

    QIcon theIcon;
    QSize theIconSize;
    qreal theOriginalM11;
    QSize theMinSize;

    int max2(int a, int b)
    { return (a >= b) ? a : b; }
};

#endif // VIEWTOOLBOXGROUP_H
