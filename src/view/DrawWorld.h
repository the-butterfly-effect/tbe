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

#include "ode/ode.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>

// forward declarations
class World;
class MainWindow;


/** class DrawObject
  * This class abstracts the actual drawing of objects
  */

class DrawWorld : public QGraphicsScene
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

	// TODO - This info should not be static, but come from Level or World ?!?
	dReal getWidth()
	{ return 6.0; };
	
	// TODO - This info should not be static, but come from Level or World ?!?
	dReal getHeight()
	{ return 3.0; };
	
	
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
	
public:
	QGraphicsRectItem* theBackGroundRectPtr;

private:

	void initAttributes ( ) ;

};

#endif // DRAWWORLD_H
