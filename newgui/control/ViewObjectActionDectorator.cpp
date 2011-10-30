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
#include "MoveUndoCommand.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ViewObjectActionDecorator::ViewObjectActionDecorator(
        ViewObject* parent,
        const QString& aDecoratorName,
        AbstractUndoCommand* myAbstractUndoCommandPtr)
    : QGraphicsSvgItem(aDecoratorName, parent),
      theAUCPtr(myAbstractUndoCommandPtr)
{
    qDebug() << Q_FUNC_INFO;

    // we need to have the same size as our parent
    QRectF parSize = parent->boundingRect();
    QRectF mySize = boundingRect();
    scale(parSize.width()/mySize.width(),parSize.height()/mySize.height());
    setFlags(ItemIsMovable);
}


void
ViewObjectActionDecorator::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
    if (theAUCPtr->mouseMoveEvent(event)==false)
        QGraphicsSvgItem::mouseMoveEvent(event);
}


void
ViewObjectActionDecorator::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{
    if (theAUCPtr->mousePressEvent(event)==false)
        QGraphicsSvgItem::mousePressEvent(event);
}

void
ViewObjectActionDecorator::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
{
    if (theAUCPtr->mouseReleaseEvent(event)==false)
        QGraphicsSvgItem::mouseReleaseEvent(event);
    else
    {
        // TODO: finish up by killing myself and propagating that
    }

}
