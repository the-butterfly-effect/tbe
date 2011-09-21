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

#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"
#include "AbstractUndoCommand.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ViewObjectActionDecorator::ViewObjectActionDecorator(
        ViewObject* parent,
        const QString& aDecoratorName)
    : QGraphicsSvgItem(aDecoratorName, parent)
{
    qDebug() << Q_FUNC_INFO;

    // we need to have the same size as our parent
    QRectF parSize = parent->boundingRect();
    QRectF mySize = boundingRect();
    scale(parSize.width()/mySize.width(),parSize.height()/mySize.height());
}


void
ViewObjectActionDecorator::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{
    theAUCPtr->mousePressEvent(event->pos());
}
