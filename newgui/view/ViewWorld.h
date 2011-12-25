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

#ifndef VIEWWORLD_H
#define VIEWWORLD_H

#include <QtGui/QGraphicsScene>
#include <QtCore/QTime>
#include <QtCore/QTimer>

class ResizingGraphicsView;
class World;

/** class ViewWorld
  * This class contains the View* objects and is the QGraphicsScene.
  * It contains the timers that run the simulation.
  * TODO/FIXME: to be re-implemented: the debug callbacks from Box2D to draw outlines.
  */
class ViewWorld : public QGraphicsScene
{
    Q_OBJECT

public:
	explicit ViewWorld (ResizingGraphicsView* aGraphicsViewPtr, World* aWorldPtr);

	// Public accessor methods
	//

	qreal getWidth();
	qreal getHeight();

	// QGraphicsScene events
	//

	virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );

signals:

public slots:
	/// public slot: reset the scene
	void slot_resetSim();

	// signals to set the game speed
	void slot_signalFF();
	void slot_signalPause();
	void slot_signalPlay();
	void slot_signalReset();

	/// public slot: start the simulation (at speed 1)
	void slot_startSim();

private slots:
	/// called by a oneshot timer after on_winning() was called
	//void on_OneSecondAfterWinning(void);

	/// called whenever a timer tick happens
	void on_timerTick(void);

	/// called whenever the framerate timer tick happens
	//void on_framerateTimerTick(void);

private:
	// Private attributes
	//

	World* theWorldPtr;

	QTimer theTimer;
//	QTimer theFramerateTimer;
	QTime  theSimulationTime;

	/// milliseconds per time step ???
	qreal theSimSpeed;

	unsigned int theFramesPerSecond;

	bool isSimRunning;

private:
	void initAttributes ( ) ;





    // keep this one last, it kills copy constructors & assignment operators
    Q_DISABLE_COPY ( ViewWorld );
};

#endif // VIEWWORLD_H
