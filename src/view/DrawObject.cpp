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

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>
#include <QUndoStack>

#include "UndoMoveCommand.h"

// set/get using static setter/getter in DrawObject
// as such it is available to DrawObject and derived classes
static QUndoStack* theUndoStackPtr = NULL;

// Constructors/Destructors
//  

DrawObject::DrawObject (BaseObject* aBaseObjectPtr)
	: theBaseObjectPtr(aBaseObjectPtr), theAnchorsPtr(NULL), theRenderer (NULL)
{
	initAttributes();
}

DrawObject::DrawObject (BaseObject* aBaseObjectPtr, const QString& anImageName)
	: theBaseObjectPtr(aBaseObjectPtr), theAnchorsPtr(NULL), theRenderer (NULL)
{
	initAttributes();
	theRenderer = ImageStore::getRenderer(anImageName);
}


DrawObject::~DrawObject ( ) { }

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
    const dReal *pos1 = dGeomGetPosition (theBaseObjectPtr->getTheGeomID());
    const dReal *ang  = dGeomGetRotation (theBaseObjectPtr->getTheGeomID());

    // TODO: this can be done a lot nicer with QTranform
    qreal myAngle = atan2(ang[1], ang[0]);
    
    // Qt has Y positive downwards, whereas all of the model has Y upwards.
    // that's what the minus is for :-)
    setPos(pos1[0], -pos1[1]);
    rotate((myAngle-theOldAngle)*180/3.14);
    theOldAngle=myAngle;
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


void DrawObject::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
	DEBUG5("HOVER START\n");
	if (theAnchorsPtr==NULL)
	{
		theAnchorsPtr = new Anchors(this);
	}
}

void DrawObject::hoverLeaveEvent ( QGraphicsSceneHoverEvent *)
{
	DEBUG5("HOVER LEAVE\n");
	if (theAnchorsPtr!=NULL)
	{
		delete theAnchorsPtr;
		theAnchorsPtr=NULL;
	}
}

void DrawObject::initAttributes ( )
{
	theBodyID = theBaseObjectPtr->getTheBodyID();

	// the objects sizes usually are less than a meter
	// however, that does not sit well with QPainter, which is still a
	// bitmap-oriented class - we're discussing images of less than one-by-one pixel.
	// that's what we need scaling for.
	//
	// theScale is set to 100.0 - that implies centimeters.
	scale(1.0/theScale, 1.0/theScale);

	// in radians!
	theOldAngle=0;//aBaseObjectPtr->getTheCenter().angle;

	// make sure that this item is selectable & draggable
	// (if the object allows it - of course)
	if (theBaseObjectPtr->isMovable())
	{
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		setAcceptHoverEvents(true);
	}

//    setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(128,128));
	setToolTip(theBaseObjectPtr->getToolTip());

	applyPosition();

}

void DrawObject::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseMoveEvent(%d)\n", event->type());

	dReal orgposx = dGeomGetPosition(theBaseObjectPtr->getTheGeomID())[0];
	dReal orgposy = dGeomGetPosition(theBaseObjectPtr->getTheGeomID())[1];
	
	QPointF myPos=event->scenePos ();
	
	if ( (myPos.x()-theBaseObjectPtr->getTheWidth()/2.0) >= 0.0 
			&& (myPos.y()+theBaseObjectPtr->getTheHeight()/2.0) <= 0.0)
	{
		dGeomSetPosition(theBaseObjectPtr->getTheGeomID(), myPos.x(), -myPos.y(), 0.0);
		applyPosition();
		
		// if the new position collides with another, reset the position to the original one
		if (!scene()->collidingItems(this).isEmpty())
		{
			dGeomSetPosition(theBaseObjectPtr->getTheGeomID(), orgposx, orgposy, 0.0);
			applyPosition();
		}
	}
}

void DrawObject::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseReleaseEvent(%d)\n", event->type());
	// TODO: FIXME: for now, we're not discriminative for left or right mousebutton...
	DEBUG5("for button %d\n", event->button());

	// create and file an UndoMoveCommand
	// note that the MoveCommand also pushes the move to BaseObject
	UndoMoveCommand* myCommandPtr = new UndoMoveCommand(this, theBaseObjectPtr);
	getUndoStackPtr()->push(myCommandPtr);
	QGraphicsItem::mouseReleaseEvent(event);
}

void DrawObject::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

	DEBUG5("DrawObject::paint for %p: @(%f,%f)\n", this, myWidth, myHeight);
	
	if (theRenderer == NULL)
	{
		QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
		// Body
		myPainter->drawRect(myRect);
		myPainter->setBrush(color);
		myPainter->drawEllipse(-myWidth/2, -myHeight/2, myWidth, myHeight);
	}
	else
	{
		theRenderer->render(myPainter, myRect);
		//myPainter->drawRect(myRect);
	}

}
