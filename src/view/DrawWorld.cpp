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
#include "DrawObject.h"
#include "ToolBoxItemListModel.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDropEvent>


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
	
	// announce my UndoStack to all future DrawObjects:
	DrawObject::setUndoStackPtr(&theUndoStack);

#ifdef DRAWDEBUG
	SetFlags (e_shapeBit | e_prismaticJoint | e_revoluteJoint);
#endif

	setAcceptDrops(true);
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
qreal DrawWorld::getWidth()
{
	return theWorldPtr->getTheWorldWidth(); 
}

qreal DrawWorld::getHeight()
{
	return theWorldPtr->getTheWorldHeight(); 
}


// Other methods
//  

void DrawWorld::dropEventFromView (const QPointF& aDropPos, QDropEvent* event)
{
	DEBUG4("void DrawWorld::dropEvenFromView\n");
	if (event->mimeData()->hasFormat(ToolBoxItemListModel::ToolboxMimeType))
	{
		QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
		QDataStream stream(&pieceData, QIODevice::ReadOnly);
		QString myObjectName;
		stream >> myObjectName;

		DEBUG4("  the object is: '%s' @ %f,%f\n", myObjectName.toAscii().constData(), aDropPos.x(), aDropPos.y());

		// now we know the ID of the object, let's retrieve a copy from the Toolbox
		BaseObject* myObjectPtr = theMainWindowPtr->askToolBoxForCopyOf(myObjectName);
		if (myObjectPtr != NULL)
		{
			myObjectPtr->setOrigCenter(Position(aDropPos));
			myObjectPtr->createPhysicsObject();

			theWorldPtr->addObject(myObjectPtr);

			event->setDropAction(Qt::MoveAction);
			event->accept();
			return;
		}
	}

	event->ignore();
}

void DrawWorld::initAttributes ( ) 
{
	DEBUG5("void DrawWorld::initAttributes\n");
	theSimSpeed = 1000.0;
}

void DrawWorld::on_timerTick()
{
	while(theSimulationTime < QTime::currentTime())
	{
		theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * theSimSpeed);
	}
	advance();
}

void DrawWorld::resetWorld( )
{
	DEBUG5("RESET WORLD\n");
	theWorldPtr->reset();
	// and redraw
	advance();
	setAcceptDrops(true);
#ifdef DRAWDEBUG
	clearGraphicsList(0);
#endif
}

void DrawWorld::setAcceptDrops(bool isOn)
{
	QGraphicsView* myView = views()[0];
	DEBUG5("DrawWorld::setAcceptDrops(%d) for view %p\n", isOn, myView);
	if (myView != NULL)
	{
		myView->setAcceptDrops(isOn);
		DEBUG5("new view drop state: %d\n", myView->acceptDrops());
	}
}

void DrawWorld::startTimer(void)
{
	DEBUG5("DrawWorld::startTimer(void)\n");

	// TODO: upon reset, the old object gets focus again - thanks, Qt!
	DrawObject myTemp(NULL);
	setFocusItem(&myTemp);

	setAcceptDrops(false);

	theTimer.start(1000/25);
	theSimulationTime = QTime::currentTime();
}

void DrawWorld::stopTimer(void)
{
	DEBUG5("DrawWorld::stopTimer(void)\n");
	theTimer.stop();
}





#ifdef DRAWDEBUG

/// Draw a closed polygon provided in CCW order.
void DrawWorld::DrawPolygon(UNUSED_ARG const b2Vec2* vertices, UNUSED_ARG int32 vertexCount, UNUSED_ARG const b2Color& color)
{
	DEBUG5("DrawWorld::DrawPolygon\n");
}
/// Draw a solid closed polygon provided in CCW order.
void DrawWorld::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
								 UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 0.01, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);
	QPolygonF myPoly;
	for (int i=0; i<vertexCount; i++)
	{
		if (vertices[i].x < -0.5 || vertices[i].y < -0.5)
			return;
		myPoly << QPointF(vertices[i].x, -vertices[i].y);
	}
	addDebugDrawToList(addPolygon(myPoly, pen, brush));
}
/// Draw a circle.
void DrawWorld::DrawCircle(UNUSED_ARG const b2Vec2& center,
						   UNUSED_ARG float32 radius,
						   UNUSED_ARG const b2Color& color)
{
	DEBUG5("DrawWorld::DrawCircle\n");
}
/// Draw a solid circle.
void DrawWorld::DrawSolidCircle(const b2Vec2& center, float32 radius,
								UNUSED_ARG const b2Vec2& axis,
								UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 0.01, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);
	addDebugDrawToList(addEllipse(center.x-radius,-center.y-radius, 2.0*radius,2.0*radius, pen, brush));
}
/// Draw a line segment.
void DrawWorld::DrawSegment(UNUSED_ARG const b2Vec2& p1,
							UNUSED_ARG const b2Vec2& p2,
							UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::cyan, 0.01, Qt::SolidLine);
	addDebugDrawToList(addLine(p1.x,-p1.y, p2.x, -p2.y, pen));
}
/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void DrawWorld::DrawXForm(UNUSED_ARG const b2XForm& xf)
{
	DEBUG5("DrawWorld::DrawXForm\n");
}

void DrawWorld::addDebugDrawToList(QGraphicsItem* anItem)
{
	theGraphicsList.push_back(anItem);
	clearGraphicsList(theMaxNumberOfGraphicsListElements);
}

void DrawWorld::clearGraphicsList(int aCount)
{
	while (theGraphicsList.count() > aCount)
	{
		QGraphicsItem* myItemPtr = theGraphicsList.first();
		theGraphicsList.pop_front();
		delete myItemPtr;
	}
}
#endif
