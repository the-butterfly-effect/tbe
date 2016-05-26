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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include <cassert>
#include "AbstractUndoCommand.h"
#include "ImageCache.h"
#include "MoveUndoCommand.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"

#include <QtCore/QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>



CrossRegisterSingleton::CrossRegisterSingleton()
    : theNumberOfCrosses(0)
{
    // nothing further to do...
}


CrossRegisterSingleton *CrossRegisterSingleton::me(void)
{
    static CrossRegisterSingleton *theCrossRegisterSingletonPtr = nullptr;
    if (theCrossRegisterSingletonPtr == nullptr)
        theCrossRegisterSingletonPtr = new CrossRegisterSingleton();
    return theCrossRegisterSingletonPtr;
}


void CrossRegisterSingleton::updateCrossState(signed int anAddOrSubtract)
{
    theNumberOfCrosses += anAddOrSubtract;
    emit signalNumberCrossesChanged(theNumberOfCrosses);
}






// ---------------------------------------------------------------------------
//                         ViewObjectActionDecorator
// ---------------------------------------------------------------------------




ViewObjectActionDecorator::ViewObjectActionDecorator()
    : QGraphicsPixmapItem(nullptr),
      theAUCPtr(nullptr),
      theCurrentCrossState(NONE)
{
    // nothing to do here
}

ViewObjectActionDecorator::~ViewObjectActionDecorator()
{
    setCrossState(NONE);
}


void
ViewObjectActionDecorator::setViewObject(ViewObject *aParentPtr)
{
    setParentItem(aParentPtr);

    ImageCache::getPixmap("BigCross", &theCrossImage);
    setPixmap(theCrossImage);

    // we need to have the same size as our parent
    QRectF parSize = aParentPtr->boundingRect();
    QRectF mySize = boundingRect();
    setTransform(QTransform::fromScale(parSize.width() / mySize.width(),
                                       parSize.height() / mySize.height()), true);
    setFlags(ItemIsMovable);
    setVisible(false);
}


void
ViewObjectActionDecorator::mouseMoveEvent ( QGraphicsSceneMouseEvent *event )
{
    if (theAUCPtr->mouseMoveEvent(event) == false)
        QGraphicsPixmapItem::mouseMoveEvent(event);
}


void
ViewObjectActionDecorator::mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
    // if there is no undo registered, let's delegate to someone else
    // (i.e. our parent)
    if (theAUCPtr == nullptr) {
        event->ignore();
        return;
    }
    if (theAUCPtr->mousePressEvent(event) == false)
        QGraphicsPixmapItem::mousePressEvent(event);
}


void
ViewObjectActionDecorator::mouseReleaseEvent ( QGraphicsSceneMouseEvent *event )
{
    if (theAUCPtr->mouseReleaseEvent(event) == false)
        QGraphicsPixmapItem::mouseReleaseEvent(event);
    else {
        // TODO: finish up by killing myself and propagating that
    }

}


void ViewObjectActionDecorator::setCrossState(CrossState aCrossState)
{
    // no need to change to the same
    if (aCrossState == theCurrentCrossState)
        return;
    switch (aCrossState) {
    case NONE:
        break;
    case PROXY:
        setPixmap(theProxyImage);
        break;
    case COMBINED:
        setPixmap(theCombinedImage);
        break;
    case CROSS:
        setPixmap(theCrossImage);
        break;
    }
    setVisible(aCrossState != NONE);

    // And tell the singleton where we are
    CrossRegisterSingleton::me()->updateCrossState(impliesCross(aCrossState)
                                                   - impliesCross(theCurrentCrossState));
    theCurrentCrossState = aCrossState;
}


void
ViewObjectActionDecorator::setDecoratorImage(
    const QString &aDecoratorName,
    AbstractUndoCommand *anAbstractUndoCommandPtr)
{
    DEBUG5ENTRY;
    theAUCPtr = anAbstractUndoCommandPtr;

    ImageCache::getPixmap(aDecoratorName, &theProxyImage);
    setPixmap(theProxyImage);

    // Paint the combined image, where the cross is drawn
    // on top of the proxy image.
    theCombinedImage = theProxyImage;
    QPainter myPainter;
    myPainter.begin(&theCombinedImage);
    myPainter.setRenderHint(QPainter::Antialiasing);
    myPainter.drawPixmap(theCombinedImage.rect(), theCrossImage);
    myPainter.end();

    setCrossState(PROXY);
}
