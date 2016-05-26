/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#ifndef VIEWOBJECTACTIONDECORATOR_H
#define VIEWOBJECTACTIONDECORATOR_H

#include "ViewObjectPtr.h"
#include <QGraphicsPixmapItem>

// forward declaration
class ViewObjectActionDecorator;
class AbstractUndoCommand;
class QGraphicsSceneMouseEvent;


class CrossRegisterSingleton : public QObject
{
    Q_OBJECT

public:
    /// Only access to the actual pointer to the class,
    /// used by ResizingGraphicsView to hook up this class to the SimControls.
    /// @returns pointer to the instance of the singleton.
    static CrossRegisterSingleton *me(void);

    void updateCrossState(signed int anAddOrSubtract);

signals:
    /// Emitted whenever the number of crosses on the screen changes.
    /// @param  aNumber is the number of crosses currently present on the screen.
    void signalNumberCrossesChanged(int aNumber);

private:
    /// This is a singleton: by definition private constructor
    CrossRegisterSingleton(void);

    int theNumberOfCrosses;
};



/**
  *
  */
class ViewObjectActionDecorator : public QGraphicsPixmapItem
{
public:
    /// constructor
    ViewObjectActionDecorator();

    /// destructor
    ~ViewObjectActionDecorator();

    /// clear the pointer to the UndoCommand
    void clearUndoPointer()
    {
        theAUCPtr = nullptr;
    }

    /// Attach as a child to the parent ViewObject
    /// @param aParentPtr  pointer to the ViewObject to become a child to
    void setViewObject(ViewObject *aParentPtr);

    /// Set the Decorator proxy image and set the object to call
    /// upon mouse movements.
    /// @param aDecoratorName the proxy image to set (no path, no extension)
    /// @param anAbstractUndoCommandPtr  pointer to the undo class instance
    void setDecoratorImage(const QString       &aDecoratorName,
                           AbstractUndoCommand *anAbstractUndoCommandPtr);

    enum CrossState {
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
    virtual void    mouseMoveEvent ( QGraphicsSceneMouseEvent *event );

    /// overridden from QGraphicsSvgItem so we can send that info
    /// on to our AbstractUndoCommand boss to act on...
    virtual void    mousePressEvent ( QGraphicsSceneMouseEvent *event );

    /// overridden from QGraphicsSvgItem so we can send that info
    /// on to our AbstractUndoCommand boss to act on...
    virtual void    mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

    int impliesCross(CrossState aState)
    {
        return (aState == CROSS || aState == COMBINED) ? 1 : 0;
    }

private:
    AbstractUndoCommand *theAUCPtr;

    QPixmap theCombinedImage;
    QPixmap theCrossImage;
    QPixmap theProxyImage;

    CrossState theCurrentCrossState;
};

#endif // VIEWOBJECTACTIONDECORATOR_H
