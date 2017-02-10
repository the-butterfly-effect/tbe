/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012 Klaas van Gend
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
#include "Popup.h"
#include "Position.h"
#include "resizinggraphicsview.h"
#include "ViewItem.h"
#include "ViewWorld.h"
#include "World.h"

#include <QAction>
#include <QGraphicsSceneMouseEvent>

#include "tbe_global.h"


ViewWorld::ViewWorld (ResizingGraphicsView *aGraphicsViewPtr, World *aWorldPtr)
    : QGraphicsScene(0, -THESCALE * aWorldPtr->getTheWorldHeight(),
                     THESCALE * aWorldPtr->getTheWorldWidth(), THESCALE * aWorldPtr->getTheWorldHeight()),
      theWorldPtr(aWorldPtr)
{
    aGraphicsViewPtr->setViewWorld(this, theWorldPtr->getName());

}

ViewWorld::~ViewWorld()
{
    DEBUG3ENTRY;
}


qreal ViewWorld::getHeight(void) const
{
    return THESCALE * theWorldPtr->getTheWorldHeight();
}


qreal ViewWorld::getWidth(void) const
{
    return THESCALE * theWorldPtr->getTheWorldWidth();
}


void ViewWorld::on_sizeAdjust(void)
{
    setSceneRect(0, -getHeight(), getWidth(), getHeight());
}


// ---------------------------------------------------------------------------
//                Below is the b2DebugDraw implementation
// ---------------------------------------------------------------------------

#if 0
const int ViewWorld::theMaxNumberOfGraphicsListElements = 200;

// Draw a closed polygon provided in CCW order.
void ViewWorld::DrawPolygon(UNUSED_ARG const b2Vec2 *vertices, UNUSED_ARG int32 vertexCount,
                            UNUSED_ARG const b2Color &color)
{
    DEBUG5ENTRY;
}

// Draw a solid closed polygon provided in CCW order.
void ViewWorld::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                                 UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::green, 1, Qt::SolidLine);
    QBrush brush(Qt::NoBrush);
    QPolygonF myPoly;
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].x < -0.5 || vertices[i].y < -0.5)
            return;
        myPoly << Vector(vertices[i]).toQPointF();
    }
    addDebugDrawToList(addPolygon(myPoly, pen, brush));
}

// Draw a circle.
void ViewWorld::DrawCircle(UNUSED_ARG const b2Vec2 &center,
                           UNUSED_ARG float32 radius,
                           UNUSED_ARG const b2Color &color)
{
    DEBUG5ENTRY;
}

// Draw a solid circle.
void ViewWorld::DrawSolidCircle(const b2Vec2 &center, float32 radius,
                                UNUSED_ARG const b2Vec2 &axis,
                                UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::green, 1, Qt::SolidLine);
    QBrush brush(Qt::NoBrush);

    Vector myPos = Vector(center) - Vector(radius, -radius);
    QPointF myPosQ = myPos.toQPointF();

    addDebugDrawToList(addEllipse(myPosQ.x(), myPosQ.y(),
                                  2.0 * THESCALE * radius, 2.0 * THESCALE * radius, pen, brush));
}

// Draw a line segment.
void ViewWorld::DrawSegment(UNUSED_ARG const b2Vec2 &p1,
                            UNUSED_ARG const b2Vec2 &p2,
                            UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::cyan, 1, Qt::SolidLine);

    QPointF myP1 = Vector(p1).toQPointF();
    QPointF myP2 = Vector(p2).toQPointF();

    addDebugDrawToList(addLine(myP1.x(), myP1.y(),
                               myP2.x(), myP2.y(), pen));
}

// Draw a transform. Choose your own length scale.
void ViewWorld::DrawTransform(UNUSED_ARG const b2Transform &xf)
{
    DEBUG5ENTRY;
}

void ViewWorld::addDebugDrawToList(QGraphicsItem *anItem)
{
    anItem->setZValue(150);
    theGraphicsList.push_back(anItem);
    clearGraphicsList(theMaxNumberOfGraphicsListElements);
}

void ViewWorld::clearGraphicsList(int aCount)
{
    while (theGraphicsList.count() > aCount) {
        QGraphicsItem *myItemPtr = theGraphicsList.first();
        theGraphicsList.pop_front();
        delete myItemPtr;
    }
}
#endif
