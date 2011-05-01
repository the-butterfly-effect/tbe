/* The Butterfly Effect 
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef DRAWWORLD_H
#define DRAWWORLD_H

#include "BaseObject.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QUndoStack>
#include <QPixmap>

#include "Box2D.h"

// forward declarations
class World;
class MainWindow;
class QDropEvent;
class StartStopWatch;
class UndoInsertCommand;

/** class DrawWorld
  * This class contains the Draw* objects and is the QGraphicsScene.
  * It contains the timers that run the simulation.
  * It also maintains the undo stack and handles drag-n-drop from the toolbox.
  * Lastly, it implements the debug callbacks from Box2D to draw outlines.
  */

class DrawWorld : public QGraphicsScene
		, public b2DebugDraw
{
public:
	
	Q_OBJECT

public:
	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	DrawWorld (MainWindow* aMainWindowPtr, World* aWorldPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawWorld ( );

	// Public accessor methods
	//
	
	qreal getWidth();
	qreal getHeight();

	QUndoStack* getTheUndoStackPtr(void)
		{ return &theUndoStack; }

	/// event handler called by the view for drops
	virtual void dropEvent (QGraphicsSceneDragDropEvent * event);

	/// sets the simulation speed. 1 = "real world" speed, 0.5 means slowed down, 2.0 means sped up
	void setSimSpeed(qreal theDivision)
	{ theSimSpeed = 1000.0 / theDivision; }

	/// @returns a pointer to an initialised StartStopWatch (which derives from QGraphicsScene)
	QGraphicsScene* getStartStopWatchPtr();

	/** iterate through all registered objects and make them create new
	  * bitmap caches.
	  */
	void invalidateCaching(void);

	/// deletes the outline
	void deleteOutline(void);

	/// (re-)draws the outline and the background
	void drawOutlineAndBackground(void);

	/** iterate over all DrawObjects and make them selectable.
	  * this member is called when a user switches over to level creator mode
	  */
	void makeAllObjectsSelectable();

protected:
	/// OVERRIDDEN from QGraphicsScene to handle drag&drop
	virtual void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
	/// OVERRIDDEN from QGraphicsScene to handle drag&drop
	virtual void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
	/// OVERRIDDEN from QGraphicsScene to handle drag&drop
	virtual void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );

	/// OVERRIDDEN from QGraphicsScene to handle backgrounds with both a
	/// gradient and an image - where the gradient is drawn over the image
	void drawBackground ( QPainter * painter, const QRectF & rect );
	void drawBGImage( QPainter* p, const QRectF & rect );

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

private:
		const static int theMaxNumberOfGraphicsListElements;
		typedef QList<QGraphicsItem*> GraphicsList;
		GraphicsList theGraphicsList;
		void addDebugDrawToList(QGraphicsItem* anItem);
		void clearGraphicsList(int aCount);

	/// (internal use only)
	/// draws two dots to indicate official maximum scene size.
	void drawDots(void);

signals:
	/// emitted once the level is won
	void levelWon(void);

public slots:
	void resetWorld(void);

	/// public slot: start (or continue) the simulation
	void startTimer();
	
	/// public slot: stop the simulation
	void stopTimer();

	/// public slot: used when clicked fastforward
	void goFast();

	/// public slot: used when clicked go slow
	void goSlow();

	/// called on winning by World.
	void on_winning(void);

	/// called on death of an object
	void on_death(void);

private slots:
	/// called by a oneshot timer after on_winning() was called
	void on_OneSecondAfterWinning(void);

	/// called whenever a timer tick happens
	void on_timerTick(void);

	/// called whenever the framerate timer tick happens
	void on_framerateTimerTick(void);

protected:
	/** overridden from QGraphicsScene:
	  * do not respond to UI when not allowed to
	  */
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );

private:
	// Private attributes
	//  

	MainWindow* theMainWindowPtr;
	World* theWorldPtr;

	QTimer theTimer;
	QTimer theFramerateTimer;
	QTime  theSimulationTime;
	
	QUndoStack	theUndoStack;

	bool isUserInteractionAllowed;

	/// milliseconds per time step ???
	qreal theSimSpeed;

	unsigned int theFramesPerSecond;

private:

	virtual void initAttributes ( ) ;

	/** the state machine handling the Start/Stop/Continue/Rest buttons
	 *  and the actual simulation timing
	 */
	StartStopWatch*	theSimStateMachine;

	/** as long as we are in drag&drop, we'll have to maintain
	  * a pointer to the UndoInsertCommand-in-progress
	  */
	UndoInsertCommand* theInsertUndoPtr;

	QList<QGraphicsItem*> theBasicDrawWorldItems;

	/** this local class displays the Congratulations or Death message
	  * with a nice SVG background
	  */
	class CongratDeathMessage : QGraphicsSvgItem
	{
	public:

		enum MessageType
		{
			CONGRATS,
			DEATH
		};

		CongratDeathMessage(MessageType aType, DrawWorld* aScene, MainWindow* aMainWindowPtr);
		virtual ~CongratDeathMessage();
	private:
		// I originally also wanted to hookup onClick events, but
		// that makes no sense as the DrawWorld is still in locked state:
		// you'll always get a message to reset the stopwatch.

		QPushButton* theButtons[3];
		QGraphicsScene* theScenePtr;
		QGraphicsSimpleTextItem* theTextPtr;

		void moveButtons(void);

		/// overriden so we can move the buttons around
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

	};

	CongratDeathMessage* theCongratDeathBoxPtr;

	QPixmap theBackgroundPixmap;
	void redrawBGPixmap(void);

	// prevent copy constructor / assignment operator
	DrawWorld(const DrawWorld&);
	const DrawWorld& operator= (const DrawWorld&);
};

#endif // DRAWWORLD_H
