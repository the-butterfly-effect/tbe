/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QUndoStack>

#define DRAWDEBUG

#ifdef DRAWDEBUG
 #include "Box2D.h"
#endif

// forward declarations
class World;
class MainWindow;


/** class DrawObject
  * This class abstracts the actual drawing of objects
  */

class DrawWorld : public QGraphicsScene
#ifdef DRAWDEBUG
		, public b2DebugDraw
#endif
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


#ifdef DRAWDEBUG
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
		virtual void DrawXForm(const b2XForm& xf);
#endif
	
public slots:
	void resetWorld(void);

	/// public slot: start (or continue) the simulation
	void startTimer();
	
	/// public slot: stop the simulation
	void stopTimer();

private slots:
	/// called whenever a timer tick happens
	void on_timerTick(void);
	
private:
	// Private attributes
	//  

	MainWindow* theMainWindowPtr;
	World* theWorldPtr;

	QTimer theTimer;
	QTime  theSimulationTime;
	
	QUndoStack	theUndoStack;
	
private:

	void initAttributes ( ) ;

};

#endif // DRAWWORLD_H
