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

#include "AbstractUndoCommand.h"
#include "ImageCache.h"
#include "MoveUndoCommand.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"

#include <QtCore/QDebug>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>

ViewObjectActionDecorator::ViewObjectActionDecorator()
    : QGraphicsPixmapItem(NULL),
      theAUCPtr(NULL)
{
    DEBUG3ENTRY;
}


void
ViewObjectActionDecorator::setViewObject(ViewObject* aParentPtr)
{
    setParentItem(aParentPtr);

    ImageCache::getPixmap("BigCross", &theCrossImage);
    setPixmap(theCrossImage);

    // we need to have the same size as our parent
    QRectF parSize = aParentPtr->boundingRect();
    QRectF mySize = boundingRect();
    scale(parSize.width()/mySize.width(),parSize.height()/mySize.height());
    setFlags(ItemIsMovable);
    setVisible(false);
}


void
ViewObjectActionDecorator::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
    if (theAUCPtr->mouseMoveEvent(event)==false)
        QGraphicsPixmapItem::mouseMoveEvent(event);
}


void
ViewObjectActionDecorator::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{
    // if there is no undo registered, let's delegate to someone else
    // (i.e. our parent)
    if (theAUCPtr==NULL)
    {
        event->ignore();
        return;
    }
    if (theAUCPtr->mousePressEvent(event)==false)
        QGraphicsPixmapItem::mousePressEvent(event);
}

void
ViewObjectActionDecorator::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
{
    if (theAUCPtr->mouseReleaseEvent(event)==false)
        QGraphicsPixmapItem::mouseReleaseEvent(event);
    else
    {
        // TODO: finish up by killing myself and propagating that
    }

}


void ViewObjectActionDecorator::setCrossState(CrossState aCrossState)
{
    switch (aCrossState)
    {
    case NONE:
        // TODO/FIXME: emit "cross present signal"
        break;
    case PROXY:
        setPixmap(theProxyImage);
        // TODO/FIXME: emit "no cross signal"
        break;
    case COMBINED:
        setPixmap(theCombinedImage);
        // TODO/FIXME: emit "cross present signal"
        break;
    case CROSS:
        setPixmap(theCrossImage);
        // TODO/FIXME: emit "cross present signal"
        break;
    }
    setVisible(aCrossState!=NONE);
}


void
ViewObjectActionDecorator::setDecoratorImage(
        const QString& aDecoratorName,
        AbstractUndoCommand* anAbstractUndoCommandPtr)
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
