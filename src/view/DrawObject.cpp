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

#include "DrawObject.h"
#include "BaseObject.h"
#include "PieMenu.h"
#include "Anchors.h"
#include "ImageStore.h"
#include "DrawWorld.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>
#include <QUndoStack>

#include "UndoMoveCommand.h"

// set/get using static setter/getter in DrawObject
// as such it is available to DrawObject and derived classes
static QUndoStack* theUndoStackPtr = NULL;

//EditState DrawObject::theEditState;

// Constructors/Destructors
//  

DrawObject::DrawObject (BaseObject* aBaseObjectPtr)
	: theBaseObjectPtr(aBaseObjectPtr), theAnchorsPtr(NULL), theRenderer (NULL),
	theUndeleteDrawWorldPtr(NULL)
{
	initAttributes();
}

DrawObject::DrawObject (BaseObject* aBaseObjectPtr, const QString& anImageName)
	: theBaseObjectPtr(aBaseObjectPtr), theAnchorsPtr(NULL), theRenderer (NULL),
	theUndeleteDrawWorldPtr(NULL)
{
	initAttributes();
	theRenderer = ImageStore::getRenderer(anImageName);
}


DrawObject::~DrawObject ( )
{
	if (theAnchorsPtr)
	{
		delete theAnchorsPtr;
		theAnchorsPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

void DrawObject::setUndoStackPtr(QUndoStack* aPtr)
{
	assert(aPtr != NULL);
	theUndoStackPtr = aPtr;
}
    
QUndoStack* DrawObject::getUndoStackPtr(void)
{
	assert(theUndoStackPtr != NULL);
	return theUndoStackPtr;
}

// Other methods
//  

void DrawObject::advance(int)
{
	applyPosition();
}

void DrawObject::applyPosition(void)
{
	Position myPosition = theBaseObjectPtr->getTempCenter();
    // Qt has Y positive downwards, whereas all of the model has Y upwards.
    // that's what the minus is for :-)
    setPos(myPosition.x, -myPosition.y);
	rotate((-myPosition.angle-theOldAngle)*180/3.14);
	theOldAngle=-myPosition.angle;
}

QRectF DrawObject::boundingRect() const
{
	qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
	qreal adjust = 0.03;
	
    return QRectF(-myWidth/2-adjust, -myHeight/2-adjust, myWidth+2*adjust, myHeight+2*adjust);
}

void DrawObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (theBaseObjectPtr->isMovable())
	{
		PieMenu myMenu(this);
		myMenu.exec(event->screenPos());
	}
}

bool DrawObject::deregister()
{
	theUndeleteDrawWorldPtr = reinterpret_cast<DrawWorld*>(scene());
	scene()->removeItem(this);
	return true;
}

void DrawObject::focusInEvent ( QFocusEvent * event )
{
	DEBUG5("focusInEvent for %p with %d\n", this, event->reason());
	assert(theAnchorsPtr==NULL);
	theAnchorsPtr=new Anchors(this);
	isHighlighted=true;
}

void DrawObject::focusOutEvent ( QFocusEvent * event )
{
	DEBUG5("focusOutEvent for %p with %d\n", this, event->reason());
	assert(theAnchorsPtr!=NULL);
	delete theAnchorsPtr;
	theAnchorsPtr = NULL;
	isHighlighted=false;
}


void DrawObject::hoverMoveEvent ( QGraphicsSceneHoverEvent *)
{
	// TODO: hover should report to EditState - so it can create a HoverPointer for delete
}

void DrawObject::hoverLeaveEvent ( QGraphicsSceneHoverEvent *)
{
	// TODO: hover should report to EditState - so it can create a HoverPointer for delete
}

void DrawObject::initAttributes ( )
{
	// the objects sizes usually are less than a meter
	// however, that does not sit well with QPainter, which is still a
	// bitmap-oriented class - we're discussing images of less than one-by-one pixel.
	// that's what we need scaling for.
	//
	// theScale is set to 100.0 - that implies centimeters.
	scale(1.0/theScale, 1.0/theScale);

	// in radians!
	theOldAngle=0;//aBaseObjectPtr->getOrigCenter().angle;

	// make sure that this item is selectable & draggable
	// (if the object allows it - of course)
	if (theBaseObjectPtr->isMovable())
	{
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
		setAcceptHoverEvents(true);
	}

	isHighlighted = false;

//    setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(128,128));
	setToolTip(theBaseObjectPtr->getToolTip());

	applyPosition();

}

void DrawObject::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseMoveEvent(%d)\n", event->type());

	// TODO: problem: if you click an object near the side, it will still register as if you
	// clicked in the exact center - with an unvoluntary movement as a result

	Position myOrgPos = theBaseObjectPtr->getTempCenter();
	QPointF myPos=event->scenePos ();
	if (theAnchorsPtr)
	{
		delete theAnchorsPtr;
		theAnchorsPtr = NULL;
	}
	
	if ( (myPos.x()-theBaseObjectPtr->getTheWidth()/2.0) >= 0.0 
			&& (myPos.y()+theBaseObjectPtr->getTheHeight()/2.0) <= 0.0)
	{
		// TODO FIXME: no angle yet
		theBaseObjectPtr->setTempCenter(Position(myPos.x(), -myPos.y(), 0.0));
		applyPosition();
		
		// if the new position collides with another, reset the position to the original one
		if (!scene()->collidingItems(this).isEmpty())
		{
			theBaseObjectPtr->setTempCenter(Position(myOrgPos.x, myOrgPos.y, 0.0));
			applyPosition();
		}
	}
}

void DrawObject::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseReleaseEvent(%d)\n", event->type());
	// TODO: FIXME: for now, we're not discriminative for left or right mousebutton...
	DEBUG5("for button %d\n", event->button());

	// is the position any different?
	// TODO: if position is same, no need to do undomove

	// create and file an UndoMoveCommand
	// note that the MoveCommand also pushes the move to BaseObject
	UndoMoveCommand* myCommandPtr = new UndoMoveCommand(this, theBaseObjectPtr);
	getUndoStackPtr()->push(myCommandPtr);
	QGraphicsItem::mouseReleaseEvent(event);
	if (theAnchorsPtr==NULL)
		theAnchorsPtr= new Anchors(this);
}

void DrawObject::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

	DEBUG5("DrawObject::paint for %p: @(%f,%f)\n", this, myWidth, myHeight);

	paintHighlighted(myPainter);

	if (theRenderer == NULL)
	{
		QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
		// Body
//		myPainter->drawRect(myRect);
		myPainter->setBrush(color);
		myPainter->drawEllipse(-myWidth/2, -myHeight/2, myWidth, myHeight);
	}
	else
	{
		theRenderer->render(myPainter, myRect);
		//myPainter->drawRect(myRect);
	}

}

void DrawObject::paintHighlighted(QPainter* myPainter)
{
	if (isHighlighted)
	{
		Position myC = theBaseObjectPtr->getOrigCenter();
		qreal myW = theBaseObjectPtr->getTheWidth();
		qreal myH = theBaseObjectPtr->getTheHeight();

		// calculate the box in Scene coordinates:
		QRectF myBox(myC.x-0.5*myW, -myC.y-0.5*myH, myW, myH);

		// but we draw in item coordinates:
		QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
		myPainter->setBrush(color);
		myPainter->drawPolygon(mapFromScene(myBox));
	}
}

bool DrawObject::reregister()
{
	assert(theUndeleteDrawWorldPtr);
	theUndeleteDrawWorldPtr->addItem(this);
	return true;
}
