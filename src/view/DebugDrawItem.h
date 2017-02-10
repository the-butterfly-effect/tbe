/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012,2017 Klaas van Gend
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

#ifndef DEBUGDRAWITEM_H
#define DEBUGDRAWITEM_H

#include "Box2D.h"

/** class DebugDrawItem
  * 
  * CURRENTLY NOT YET IN USE
  * 
  * This class contains the View* objects and is the QGraphicsScene.
  * It contains the timers that run the simulation.
  * It also contains the debug callbacks from Box2D to draw outlines.
  */
class DebugDrawItem : public b2Draw
{
    Q_OBJECT

public:
    explicit DebugDrawItem (World *aWorldPtr);

    virtual ~DebugDrawItem();


private:
    // keep this one last, it kills copy constructors & assignment operators
    Q_DISABLE_COPY ( DebugDrawItem );

    // the below is for the Debug drawing of Box2D
protected:
    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                                  const b2Color &color) override;
    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) override;
    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis,
                                 const b2Color &color) override;
    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform(const b2Transform &xf) override;

//    const static int theMaxNumberOfGraphicsListElements;
//    typedef QList<QGraphicsItem *> GraphicsList;
//    GraphicsList theGraphicsList;
//    void addDebugDrawToList(QGraphicsItem *anItem);
//    void clearGraphicsList(int aCount);
};

#endif // DEBUGDRAWITEM_H
