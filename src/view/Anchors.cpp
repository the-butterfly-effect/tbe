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

#include "Anchors.h"
#include "ImageStore.h"
#include "DrawObject.h"
#include "BaseObject.h"
#include "UndoResizeCommand.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <cassert>
#include <cmath>


EditObjectDialog* Anchors::theObjectDialogPtr = NULL;

//////////////////////////////////////////////////////////////////////////////
// first the implementation for class Anchors, below for class Anchor
// (note the missing 's')


Anchors::Anchors(DrawObject* anObjectPtr)
		: theDrawObjectPtr(anObjectPtr)
{
	DEBUG5("Anchors::Anchors for %p\n", anObjectPtr);

	// only create anchors when it is a player-inserted object
	BaseObject* myBOPtr = theDrawObjectPtr->getBaseObjectPtr();
	if (myBOPtr->isMovable()==false)
		return;

	// the below code creates the 8 anchors around the object
	AnchorType myMode = NONE;
	if (myBOPtr->isResizable()&BaseObject::HORIZONTALRESIZE || theIsLevelEditor)
		myMode = RESIZE;
	theAnchorList.push_back(new Anchor(myMode, Anchor::RIGHT, this));
	theAnchorList.push_back(new Anchor(myMode, Anchor::LEFT, this));

	if (myBOPtr->isResizable()&BaseObject::VERTICALRESIZE || theIsLevelEditor)
		myMode = RESIZE;
	else
		myMode = NONE;
	theAnchorList.push_back(new Anchor(myMode, Anchor::TOP, this));
	theAnchorList.push_back(new Anchor(myMode, Anchor::BOTTOM, this));

	if (myBOPtr->isRotatable() || theIsLevelEditor)
		myMode = ROTATE;
	else
		myMode = NONE;
	theAnchorList.push_back(new Anchor(myMode, Anchor::TOPRIGHT, this));
	theAnchorList.push_back(new Anchor(myMode, Anchor::TOPLEFT, this));
	theAnchorList.push_back(new Anchor(myMode, Anchor::BOTTOMLEFT, this));
	theAnchorList.push_back(new Anchor(myMode, Anchor::BOTTOMRIGHT, this));

	if (theIsLevelEditor)
	{
		if (theObjectDialogPtr == NULL)
		{
			theObjectDialogPtr = new EditObjectDialog(myBOPtr, QApplication::activeWindow());
			theObjectDialogPtr->show();
		}
		else
			theObjectDialogPtr->readFromObject(myBOPtr);
	}
	updatePosition();
}

Anchors::~Anchors()
{
	while(!theAnchorList.isEmpty())
	{
		Anchor* myPtr = theAnchorList.first();
		delete myPtr;
		theAnchorList.pop_front();
	}
	theDrawObjectPtr->focusRemove(false);
}

UndoResizeCommand* Anchors::createUndoResize(void)
{
	return new UndoResizeCommand(theDrawObjectPtr->getBaseObjectPtr());
}


QGraphicsScene* Anchors::getScenePtr()
{
	return theDrawObjectPtr->scene();
}

void Anchors::updatePosition()
{
	Position myC = theDrawObjectPtr->getBaseObjectPtr()->getOrigCenter();
	myC.angle = fmodf(myC.angle, PI*2.0);
	qreal myW = theDrawObjectPtr->getBaseObjectPtr()->getTheWidth();
	qreal myH = theDrawObjectPtr->getBaseObjectPtr()->getTheHeight();

	for (int i=0; i<theAnchorList.count();i++)
	{
		theAnchorList[i]->updatePosition(myC, myW, myH);
	}
}

//////////////////////////////////////////////////////////////////////////////

const int Anchor::theIconSize;

Anchor::Anchor(Anchors::AnchorType aDirection, AnchorPosition anIndex, Anchors* aParent)
		: theParentPtr(aParent),
		  theDirection(aDirection),
		  theIndex(anIndex),
		  theOldAngle(0), theUndoPtr(NULL)
{
	// get the QSvgRenderer for my icon
	switch (aDirection)
	{
		case Anchors::NONE:
			setSharedRenderer(ImageStore::getRenderer("ActionNone"));
			break;
		case Anchors::RESIZE:
			setSharedRenderer(ImageStore::getRenderer("ActionResizeHori"));
			break;
		case Anchors::ROTATE:
			setSharedRenderer(ImageStore::getRenderer("ActionRotate"));
			break;
	}

	if (aDirection!=Anchors::NONE)
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);


	// calculate how to scale my icon to look like it should
	QRectF mySquare=theParentPtr->getScenePtr()->views()[0]->mapToScene(QRect(0,0,theIconSize,theIconSize)).boundingRect();
	DEBUG5("32 pix = %f hori / %f verti\n", mySquare.width(), mySquare.height());
	// so we now know that our image should be reduced to width&height
	scale(mySquare.width()/boundingRect().width(),
		  mySquare.height()/boundingRect().height());

	rotate(-theIndex*45.0);

	theParentPtr->getScenePtr()->addItem(this);
	setZValue(10.0);

	theOffset=mySquare.width()/2;

	if (aDirection != Anchors::NONE)
		setEnabled(true);

}


int Anchor::getDX()
{
	switch (theIndex)
	{
		case RIGHT:
		case TOPRIGHT:
		case BOTTOMRIGHT:
			return 1;
			break;

		case TOP:
		case BOTTOM:
			return 0;
			break;

		case TOPLEFT:
		case LEFT:
		case BOTTOMLEFT:
			return -1;
			break;
	}
	return 0;
}

int Anchor::getDY()
{
	switch (theIndex)
	{
		case RIGHT:
		case LEFT:
			return 0;
			break;

		case TOPRIGHT:
		case TOP:
		case TOPLEFT:
			return 1;
			break;

		case BOTTOMRIGHT:
		case BOTTOM:
		case BOTTOMLEFT:
			return -1;
			break;
	}
	return 0;
}



void Anchor::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG3("Anchor::mouseMoveEvent(%d)\n", event->type());

	// calculate which direction we're moving
	Vector myDelta = event->scenePos() - thePrevMousePos;
//	if (theIndex==0 || theIndex==4)
//	{
//		myDelta.setY(0);
//		theUndoPtr->setDelta(getDX(theIndex), myDelta);
//	}
//	if (theIndex==2 || theIndex==6)
//	{
//		myDelta.setX(0);
//		theUndoPtr->setDelta(getDY(theIndex), myDelta);
//	}

//	if (theIndex==0 || theIndex==2 || theIndex==4 || theIndex==6)
//	{
//		theUndoPtr->setDelta2(theIndex, myDelta);
//	}


	theUndoPtr->setDelta3(theIndex, event->scenePos());

	theParentPtr->updatePosition();
}

void Anchor::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG3("Anchor::mousePressEvent\n");

	// record cursor position here
	thePrevMousePos=event->scenePos ();

	// reset cursor to hori/verti/rotate
	// TODO/FIXME: this code is obviously ignoring rotation...
	if (theIndex==RIGHT || theIndex==LEFT)
		setCursor(Qt::SizeHorCursor);
	if (theIndex==TOP || theIndex==BOTTOM)
		setCursor(Qt::SizeVerCursor);
	// TODO: add cursor ROTATE shape

	assert(theUndoPtr==NULL);
	theUndoPtr = theParentPtr->createUndoResize();
}

void Anchor::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG3("Anchor::mouseReleaseEvent\n");

	// are we currently in a collision?
	// in that case, go back to last known good
	if (theUndoPtr->isGood()==false)
	{
		DEBUG4("Reverting to last known non-colliding position\n");
		theUndoPtr->revertToLastGood();
	}

	// there was actual resizing???
	if (theUndoPtr->isResized())
	{
		DEBUG5("PUSHED UNDO RESIZE\n");
		theParentPtr->pushUndo(theUndoPtr);
		theUndoPtr = NULL;
	}
	else
	{
		DEBUG5("CLEARED UNDO RESIZE\n");
		if (theUndoPtr)
		{
			theUndoPtr->undo();
			delete theUndoPtr;
		}
		theUndoPtr = NULL;
	}
	thePrevMousePos=event->scenePos ();
}

void Anchor::updatePosition(Position myC, qreal myW, qreal myH)
{
	// calculate the position of the center of the anchor
	qreal myDX = (0.5*myW+0.3*theOffset) * static_cast<qreal>(getDX());
	qreal myDY = (0.5*myH+0.3*theOffset) * static_cast<qreal>(getDY());
	myC = myC + Vector(myDX,myDY);

	// anchor center correction
	qreal myTempAngle = myC.angle;
	myC.angle = theIndex*PI/4.;
	myC = myC + Vector(theOffset,theOffset);
	myC.angle = myTempAngle;

	// the final rotate/move
	rotate((-myC.angle-theOldAngle)*180/3.14);
	setPos(myC.x, -myC.y);
	theOldAngle=-myC.angle;
}
