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

	// Constructors/Destructors
	//  

public:
	/**
	 * Empty Constructor
	 */
	DrawWorld (MainWindow* aMainWindowPtr, World* aWorldPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawWorld ( );

	float getWidth()
	{ return 5.0; };
	
	float getHeight()
	{ return 4.0; };
	
	
public slots:
	void timeStep(void);
	
protected:
    // Protected attribute accessor methods
	//  


protected:

	// Protected attributes
	//  

private:

	// Private attributes
	//  
	MainWindow* theMainWindowPtr;
	World* theWorldPtr;

public:
	QGraphicsRectItem* theBackGroundRectPtr;

private:


	void initAttributes ( ) ;

};

#endif // DRAWOBJECT_H
