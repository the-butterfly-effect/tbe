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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef VIEWWORLD_H
#define VIEWWORLD_H

#include <QtGui/QGraphicsScene>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include "Box2D.h"

class QAction;
class ResizingGraphicsView;
class World;

/** class ViewWorld
  * This class contains the View* objects and is the QGraphicsScene.
  * It contains the timers that run the simulation.
  * It also contains the debug callbacks from Box2D to draw outlines.
  */
class ViewWorld : public QGraphicsScene, public b2DebugDraw
{
    Q_OBJECT

public:
	explicit ViewWorld (ResizingGraphicsView* aGraphicsViewPtr, World* aWorldPtr);

	// Public accessor methods
	//

	const World* getWorldPtr() const
	{ return theWorldPtr; }

	qreal getWidth(void) const;
	qreal getHeight(void) const;

	/// @returns true if the simulation is not in 'stopped' mode.
	static bool getIsSimRunning();

	// QGraphicsScene events
	//

	virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );

signals:
	void needReset();

public slots:
        // signals to start/stop/ffwd/reset the game
        void slot_signal4F();
        void slot_signalFF();
        void slot_signalPause();
        void slot_signalPlay();
        void slot_signalReset();

        void setupBackground(void);

private slots:
	/// called whenever a timer tick happens
	void on_timerTick(void);

	/// called whenever the framerate timer tick happens
	void on_framerateTimerTick(void);

private:
	// Private attributes
	//

	World* theWorldPtr;

	QTimer theTimer;
	QTimer theFramerateTimer;
	QTime  theSimulationTime;

        /// current number of milliseconds per time step
        /// (note that TBE is configured to run at "half of reality" speed)
	qreal theSimSpeed;

	unsigned int theFramesPerSecond;

	QAction* theFrameRateViewPtr;

private:
    // keep this one last, it kills copy constructors & assignment operators
    Q_DISABLE_COPY ( ViewWorld );



	// the below is for the Debug drawing of Box2D
protected:
	/// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf);

	const static int theMaxNumberOfGraphicsListElements;
	typedef QList<QGraphicsItem*> GraphicsList;
	GraphicsList theGraphicsList;
	void addDebugDrawToList(QGraphicsItem* anItem);
	void clearGraphicsList(int aCount);

};

#endif // VIEWWORLD_H
