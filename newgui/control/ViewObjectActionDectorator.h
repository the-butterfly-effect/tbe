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
    /// constructor
    ViewObjectActionDecorator();

    /// Attach as a child to the parent ViewObject
    /// @param aParentPtr  pointer to the ViewObject to become a child to
    void setViewObject(ViewObject* aParentPtr);

    /// Set the Decorator proxy image and set the object to call
    /// upon mouse movements.
    /// @param aDecoratorName the proxy image to set (no path, no extension)
    /// @param anAbstractUndoCommandPtr  pointer to the undo class instance
    void setDecoratorImage(const QString&       aDecoratorName,
                           AbstractUndoCommand* anAbstractUndoCommandPtr);

    enum CrossState
    {
        NONE,       /// no image
        PROXY,      /// only proxy image
        CROSS,      /// only cross
        COMBINED,   /// proxy + cross
    };

    /// Set the Decorator to a state in the enum CrossState
    void setCrossState(CrossState aCrossState);

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
    AbstractUndoCommand* theAUCPtr;

    QPixmap theCombinedImage;
    QPixmap theCrossImage;
    QPixmap theProxyImage;
};

#endif // VIEWOBJECTACTIONDECORATOR_H
