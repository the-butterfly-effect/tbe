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

#ifndef VIEWWORLD_H
#define VIEWWORLD_H

#include <QGraphicsScene>

#include "Box2D.h"
#include "AbstractObjectPtr.h"

class ResizingGraphicsView;
class World;

/** class ViewWorld
  * This class contains the View* objects and is the QGraphicsScene.
  * It contains the timers that run the simulation.
  * It also contains the debug callbacks from Box2D to draw outlines.
  */
class ViewWorld : public QGraphicsScene //, public b2Draw
{
    Q_OBJECT

public:
    explicit ViewWorld (ResizingGraphicsView *aGraphicsViewPtr, World *aWorldPtr);

    virtual ~ViewWorld();

    // Public accessor methods
    //

    const World *getWorldPtr() const
    {
        return theWorldPtr;
    }

    qreal getWidth(void) const;
    qreal getHeight(void) const;

    // QGraphicsScene events
    //


signals:
    /// any ViewObject that has changes to mention to EditObjectDialog will
    /// emit a signal that is sent here.
    /// ViewWorld will re-emit a signal for it.
    /// @param anAOPtr std::shared_ptr to the AbstractObject underneath.
    void signal_updateEditObjectDialog(AbstractObjectPtr anAOPtr);

public slots:
    void on_sizeAdjust(void);
    void setupBackground(void);

    /// any ViewObject that has changes to mention to EditObjectDialog will
    /// emit a signal that is sent here.
    /// ViewWorld will immediately re-emit a signal for it.
    /// @param anAOPtr std::shared_ptr to the AbstractObject underneath.
    void slot_updateEditObjectDialog(AbstractObjectPtr anAOPtr)
    {
        emit signal_updateEditObjectDialog(anAOPtr);
    }

private:
    // Private attributes
    //

    World *theWorldPtr;


private:
    // keep this one last, it kills copy constructors & assignment operators
    Q_DISABLE_COPY ( ViewWorld );

    // the below is for the Debug drawing of Box2D
protected:
/*    /// Draw a closed polygon provided in CCW order.
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

    const static int theMaxNumberOfGraphicsListElements;
    typedef QList<QGraphicsItem *> GraphicsList;
    GraphicsList theGraphicsList;
    void addDebugDrawToList(QGraphicsItem *anItem);
    void clearGraphicsList(int aCount);
*/
};

#endif // VIEWWORLD_H
