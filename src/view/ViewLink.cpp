/* The Butterfly Effect
 * This file copyright (C) 2012  Klaas van Gend
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

#include "AbstractObject.h"
#include "ViewLink.h"
#include "tbe_global.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOption>


void ViewLine::mousePressEvent(QGraphicsSceneMouseEvent *anEvent)
{
    if (theIsLevelCreator)
        theParent->mousePressEvent(anEvent);
}


// Constructors/Destructors
//

ViewLink::ViewLink (AbstractObjectPtr aAbstractObjectPtr, const QString &anImageName)
    : ViewObject(aAbstractObjectPtr, "Empty"), theImageName(anImageName)
{
    // Everything is done in the ViewObject constructor...
    DEBUG1ENTRY;
    theLinePtr = nullptr;
}

ViewLink::~ViewLink ( )
{
    scene()->removeItem(theLinePtr);
}


void ViewLink::setEndpoints(const Vector &aFirstPoint,
                            const Vector &aSecondPoint)
{
    if (theLinePtr == nullptr) {
        // We expect the ImageName to contain the color information.
        // If no image name, invisible line

        theLinePtr = new ViewLine(this);
        QPen pen(QColor(theImageName), 2, Qt::SolidLine);
        theLinePtr->setPen(pen);

        scene()->addItem(theLinePtr);
        theLinePtr->setZValue(zValue());
    }

    theLinePtr->setLine(QLineF(aFirstPoint.toQPointF(),
                               aSecondPoint.toQPointF()));
    theLinePtr->setVisible(isVisible());
}
