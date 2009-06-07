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

#include "DrawWorld.h"
#include "World.h"
#include "MainWindow.h"
#include "tbe_global.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>


/** the Dot class is a helper - it's a true QGraphicsItem, it's
 *  just invisible (nothing is drawn) and very very small
 *  However - it does affect to what size the Scene streches.
 */
class Dot : public QGraphicsItem
{
public:
	Dot(qreal x, qreal y)
	{
		setPos(x,y);
		setFlags(0);
		setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(128,128));
	}

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const
    {    return QRectF(0,0, 0.001, 0.001); }
	
	/// overriden from QGraphicsItem
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget *)
    { /* nothing! */ }
};


// Constructors/Destructors
//  

DrawWorld::DrawWorld (MainWindow* aMainWindowPtr, World* aWorldPtr)
	: theMainWindowPtr(aMainWindowPtr), theWorldPtr(aWorldPtr)
{
	initAttributes();
	
	aMainWindowPtr->setScene(this, theWorldPtr->getName());
	
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));

	// draw a box as the outline of the World
	addLine(0,0,                     getWidth(),0);
	addLine(getWidth(),0,            getWidth(),-getHeight());
	addLine(getWidth(),-getHeight(), 0,-getHeight());
	addLine(0,-getHeight(),          0,0);
	
	// two dots on top-right and bottom-left to make sure that the Scene
	// is rendered in total
	addItem(new Dot(getWidth()+0.01, -getHeight()-0.01));
	addItem(new Dot(          -0.01,             +0.01));
}



DrawWorld::~DrawWorld ( ) 
{
	DEBUG5("DrawWorld::~DrawWorld ( )\n");
}

//  
// Methods
//  


// Accessor methods
//  
dReal DrawWorld::getWidth()
{
	return theWorldPtr->getTheWorldWidth(); 
}

dReal DrawWorld::getHeight()
{
	return theWorldPtr->getTheWorldHeight(); 
}


// Other methods
//  

void DrawWorld::initAttributes ( ) 
{
	DEBUG5("void DrawWorld::initAttributes\n");
}

void DrawWorld::on_timerTick()
{
	while(theSimulationTime < QTime::currentTime())
	{
		theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep()*1000.0);
	}
	advance();
}

void DrawWorld::resetWorld( )
{
	DEBUG5("RESET WORLD\n");
	theWorldPtr->reset();
	// and redraw
	advance();
}

void DrawWorld::startTimer(void)
{
	DEBUG5("DrawWorld::startTimer(void)\n");
	theTimer.start(1000/25);
	theSimulationTime = QTime::currentTime();
}

void DrawWorld::stopTimer(void)
{
	DEBUG5("DrawWorld::stopTimer(void)\n");
	theTimer.stop();
}
