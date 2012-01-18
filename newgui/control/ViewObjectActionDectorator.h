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

#ifndef VIEWOBJECTACTIONDECORATOR_H
#define VIEWOBJECTACTIONDECORATOR_H

#include <QGraphicsPixmapItem>

// forward declaration
class ViewObject;
class AbstractUndoCommand;
class QGraphicsSceneMouseEvent;

/**
  *
  */
class ViewObjectActionDecorator : public QGraphicsPixmapItem
{
public:
    ViewObjectActionDecorator(ViewObject* parent,
                              const QString& aDecoratorName,
                              AbstractUndoCommand* myAbstractUndoCommandPtr);

    void setCrossState(bool isCrossToBeSet);

protected:
    /// overridden from QGraphicsSvgItem so we can send that info
    /// on to our AbstractUndoCommand boss to act on...
    virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent* event );

    /// overridden from QGraphicsSvgItem so we can send that info
    /// on to our AbstractUndoCommand boss to act on...
    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent* event );

    /// overridden from QGraphicsSvgItem so we can send that info
    /// on to our AbstractUndoCommand boss to act on...
    virtual void	mouseReleaseEvent( QGraphicsSceneMouseEvent* event );


private:
    void displayCross(void);
    void displayNormal(void);

    AbstractUndoCommand* theAUCPtr;

    QPixmap theRegularImage;
    QPixmap theCrossImage;
};

#endif // VIEWOBJECTACTIONDECORATOR_H
