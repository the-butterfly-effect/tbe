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

#include "Anchors.h"
#include "ImageStore.h"
#include "DrawObject.h"
#include "BaseObject.h"
#include "UndoDeleteCommand.h"

// for DetonatorBox
#include "TriggerExplosion.h"
#include "ChoosePhoneNumber.h"

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
	if ( (myBOPtr->isResizable()&BaseObject::HORIZONTALRESIZE) || theIsLevelEditor)
		myMode = RESIZEHORI;
	theAnchorList.push_back(new ResizeAnchor(myMode, Anchor::RIGHT, this));
	theAnchorList.push_back(new ResizeAnchor(myMode, Anchor::LEFT, this));

	if ( (myBOPtr->isResizable()&BaseObject::VERTICALRESIZE) || theIsLevelEditor)
		myMode = RESIZEVERTI;
	else
		myMode = NONE;
	theAnchorList.push_back(new ResizeAnchor(myMode, Anchor::TOP, this));
	theAnchorList.push_back(new ResizeAnchor(myMode, Anchor::BOTTOM, this));

	if (myBOPtr->isRotatable() || theIsLevelEditor)
		myMode = ROTATE;
	else
		myMode = NONE;
	theAnchorList.push_back(new RotateAnchor(myMode, Anchor::TOPRIGHT, this));
	theAnchorList.push_back(new RotateAnchor(myMode, Anchor::TOPLEFT, this));
	theAnchorList.push_back(new RotateAnchor(myMode, Anchor::BOTTOMLEFT, this));
	theAnchorList.push_back(new RotateAnchor(myMode, Anchor::BOTTOMRIGHT, this));

	//// the Buttons are displayed left of topleft and then down

	// delete button
	ButtonAnchor* myBAPtr = new ButtonAnchor(this, "ActionDelete", 0);
	connect(myBAPtr, SIGNAL(clicked()), this, SLOT(delete_clicked()));
	theAnchorList.push_back(myBAPtr);

	int myButtonIndex=1;

	// object edit button (in level creator only)
	if (theIsLevelEditor)
	{
		myBAPtr = new ButtonAnchor(this, "IconModified", myButtonIndex++);
		connect(myBAPtr, SIGNAL(clicked()), this, SLOT(editObject_clicked()));
		theAnchorList.push_back(myBAPtr);
	}

	// set phone number (for DetonatorBox only)
	// FIXME - when this list is increasing,
	// we should move it to the BaseObject class at some point
	theDetBoxPtr = dynamic_cast<DetonatorBox*>(myBOPtr);
	if (theDetBoxPtr!=NULL && theIsLevelEditor==true)
	{
		myBAPtr = new ButtonAnchor(this, "IconSetPhone", myButtonIndex++);
		connect(myBAPtr, SIGNAL(clicked()), this, SLOT(setPhoneNr_clicked()));
		theAnchorList.push_back(myBAPtr);
	}

	// FIXME/TODO: add another one for post-it editing.
	// and remove the double-click there.

	if (theObjectDialogPtr != NULL)
	{
		theObjectDialogPtr->readFromObject(myBOPtr);
		theObjectDialogPtr->show();
	}

	updatePosition();
}

Anchors::~Anchors()
{
	clearEditObjectDialogPtr();
	while(!theAnchorList.isEmpty())
	{
		Anchor* myPtr = theAnchorList.first();
		delete myPtr;
		theAnchorList.pop_front();
	}
	theDrawObjectPtr->focusRemove(false);
}


void Anchors::delete_clicked()
{
	// the DELETE button
	UndoDeleteCommand* myCommandPtr = new UndoDeleteCommand(getBOPtr());
	myCommandPtr->push();
}

void Anchors::editObject_clicked()
{
	// the EDITOBJECT button
	if (theObjectDialogPtr == NULL)
	{
		theObjectDialogPtr = new EditObjectDialog(getBOPtr(), QApplication::activeWindow());
		theObjectDialogPtr->show();
	}
}

BaseObject*   Anchors::getBOPtr(void) const
{
	return theDrawObjectPtr->getBaseObjectPtr();
}

bool Anchors::isAnchor(QGraphicsItem* anItem)
{
	for (int i=0; i<theAnchorList.size();i++)
	{
		if (theAnchorList[i]==anItem)
			return true;
	}
	return false;
}

QGraphicsScene* Anchors::getScenePtr()
{
	assert(theDrawObjectPtr != NULL);
	assert(theDrawObjectPtr->scene() != NULL);
	return theDrawObjectPtr->scene();
}

void Anchors::setPhoneNr_clicked()
{
	DEBUG4("void Anchors::setPhoneNr_clicked for %p()\n", theDetBoxPtr);
	assert(theDetBoxPtr!=NULL);

	// display a combobox with those numbers
	QGraphicsView* myViewPtr = theDrawObjectPtr->scene()->views()[0];
	ChoosePhoneNumber* myDialogPtr = new ChoosePhoneNumber(theDetBoxPtr, myViewPtr);
	myDialogPtr->setAutoFillBackground(true);
	QSize myDialogSize = myDialogPtr->size();
	QSize myViewSize = myViewPtr->size();
	Position myC = theDrawObjectPtr->getBaseObjectPtr()->getOrigCenter();
	QPoint myPosInView = myViewPtr->mapFromScene(myC.x,-myC.y);
	if (myPosInView.x()+myDialogSize.width() > myViewSize.width())
		myPosInView.setX(myViewSize.width() - 1.2*myDialogSize.width());
	if (myPosInView.y()+myDialogSize.height() > myViewSize.height())
		myPosInView.setY(myViewSize.height() - 1.2*myDialogSize.height());
	myDialogPtr->move(myPosInView);
	myDialogPtr->show();

	// The dialog will set the phone numbers itself, no need for us to worry
	// it will also clean up after itself, I hope :-)
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

const int Anchor::theIconSize = 16;

Anchor::Anchor(Anchors::AnchorType aDirection, AnchorPosition anIndex, Anchors* aParent)
		: theParentPtr(aParent),
		  theDirection(aDirection),
		  theIndex(anIndex), theButtonIndex(0), theOldAngle(0)
{
	// get the QSvgRenderer for my icon
	switch (aDirection)
	{
		case Anchors::NONE:
			setSharedRenderer(ImageStore::getRenderer("ActionNone"));
			break;
		case Anchors::RESIZEHORI:
			setSharedRenderer(ImageStore::getRenderer("ActionResizeHori"));
			break;
		case Anchors::RESIZEVERTI:
			setSharedRenderer(ImageStore::getRenderer("ActionResizeVerti"));
			break;
		case Anchors::ROTATE:
			setSharedRenderer(ImageStore::getRenderer("ActionRotate"));
			break;
		case Anchors::BUTTON:
			// will be handled in the constructor
			break;
	}

	if (aDirection!=Anchors::BUTTON)
		scaleIcon();

        theParentPtr->getScenePtr()->addItem(this);
		// anchors always appear on top of everything
		// and if selectable, let's make sure we actually can :-)
		setZValue(100.0);
		if (aDirection != Anchors::NONE)
			setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

void Anchor::scaleIcon()
{
	// calculate how to scale my icon to look like it should
	if (theParentPtr->getScenePtr() == NULL)
		return;
	QGraphicsView* myViewPtr = theParentPtr->getScenePtr()->views()[0];
	assert (myViewPtr != NULL);
	QRectF mySquare= myViewPtr->mapToScene(QRect(0,0,theIconSize,theIconSize)).boundingRect();
	DEBUG5("32 pix = %f hori / %f verti\n", mySquare.width(), mySquare.height());
	// so we now know that our image should be reduced to width&height
        setTransform(
                    QTransform::fromScale(mySquare.width()/boundingRect().width(),
                                          mySquare.height()/boundingRect().height()));
        theOffset=mySquare.width()/2;
}


int Anchor::getDX()
{
	switch (theIndex)
	{
		case RIGHT:
		case TOPRIGHT:
		case BOTTOMRIGHT:
			return 1;

		case TOP:
		case BOTTOM:
			return 0;

		case BUTTONBAR:
		case TOPLEFT:
		case LEFT:
		case BOTTOMLEFT:
			return -1;
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

		case TOPRIGHT:
		case TOP:
		case TOPLEFT:
		case BUTTONBAR:
			return 1;

		case BOTTOMRIGHT:
		case BOTTOM:
		case BOTTOMLEFT:
			return -1;
	}
	return 0;
}


void Anchor::updatePosition(Position myC, qreal myW, qreal myH)
{
	// calculate the position of the center of the anchor
	qreal myDX = (0.5*myW+1.5*theOffset) * static_cast<qreal>(getDX()) - theOffset;
	qreal myDY = (0.5*myH+1.5*theOffset) * static_cast<qreal>(getDY()) + theOffset;
	if (theIndex==Anchor::BUTTONBAR)
	{
		myDX = myDX -2.5*theOffset;
		myDY = myDY -2.5*theOffset*theButtonIndex;
	}

	Position myD = myC + Vector(myDX,myDY);

	// rotate the icon, note that rotate is incremental
	rotate((-myD.angle-theOldAngle)*180/3.14);

	// to make sure	that our buttons are inside the screen
	if (theIndex==Anchor::BUTTONBAR && myD.x < 0.0)
	{
		myDX=-myDX;
		myD = myC + Vector(myDX,myDY);
	}

	setPos(myD.x, -myD.y);
	theOldAngle=-myD.angle;
}


// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################

ResizeAnchor::ResizeAnchor(Anchors::AnchorType aDirection,
						   AnchorPosition anIndex, Anchors* aParent)
		: Anchor(aDirection, anIndex, aParent), theUndoResizePtr(NULL)
{
    DEBUG5("ResizeAnchor::ResizeAnchor(%d, %d, %p)\n", aDirection, anIndex, aParent);
}

void ResizeAnchor::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
	DEBUG5("ResizeAnchor::mouseMoveEvent(%d)\n", event->type());

	if(theUndoResizePtr==NULL)
		return;

	QPointF aCursorPos = event->scenePos();
	Position myOldCenter = theUndoResizePtr->getOldPosition();
	Position myNewCenter;
	Vector myOldSize = theUndoResizePtr->getOldSize();
	Vector myNewSize;

	// Essentially, we're going to project the current cursor position
	// across the axis through the object center and the anchor
	// the length of the projection is used to calculate the new
	// width of the object.
	// The goal is to keep the anchor at the other end of the object at the
	// same position.

	Vector myVectorToCenter = Vector(aCursorPos) - myOldCenter.toVector();
	float myAngle = myVectorToCenter.toAngle();
	float myAxisAngle = theIndex*PI/4 + myOldCenter.angle;
	float myLengthAcrossAxis = myVectorToCenter.length()*cos(myAngle-myAxisAngle);

	// This works - even for larger angles!
	if (theIndex==RIGHT || theIndex==LEFT)
	{
		float myOldWidth = myOldSize.dx/2.0;
		if (myLengthAcrossAxis+myOldWidth < 0.1)
			return;
		myNewSize = Vector(myLengthAcrossAxis+myOldWidth, myOldSize.dy);
		if (theIndex==Anchor::RIGHT)
			myNewCenter = myOldCenter + Vector(0.5*(myLengthAcrossAxis-myOldWidth),0);
		if (theIndex==Anchor::LEFT)
			myNewCenter = myOldCenter + Vector(-0.5*(myLengthAcrossAxis-myOldWidth),0);
	}
	if (theIndex==TOP || theIndex==BOTTOM)
	{
		float myOldHeight= myOldSize.dy/2.0;
		if (myLengthAcrossAxis+myOldHeight < 0.1)
			return;
		myNewSize = Vector(myOldSize.dx, myLengthAcrossAxis+myOldHeight);
		if (theIndex==Anchor::TOP)
			myNewCenter = myOldCenter + Vector(0, 0.5*(myLengthAcrossAxis-myOldHeight));
		if (theIndex==Anchor::BOTTOM)
			myNewCenter = myOldCenter + Vector(0, -0.5*(myLengthAcrossAxis-myOldHeight));
	}

	theUndoResizePtr->update(myNewCenter, myNewSize);
//	theParentPtr->updatePosition();
}

void ResizeAnchor::mousePressEvent ( QGraphicsSceneMouseEvent*)
{
	DEBUG5("ResizeAnchor::mousePressEvent\n");

	// only active this if there are real icons to be had
	if (theDirection==Anchors::NONE)
		return;

	// reset cursor to hori/verti/rotate
	// and setup the corresponding undo class
	assert(theUndoResizePtr==NULL);

	// note that if an object has rotated between 45 and 135 degrees,
	// we'd better swap the cursors or we're showing the wrong thingie...
	float myRealAngle = theOldAngle;
	if (theIndex==TOP || theIndex==BOTTOM)
		myRealAngle += PI/2;
	myRealAngle = fabsf(fmodf(myRealAngle,PI));
	if (myRealAngle>PI/4 && myRealAngle<3*PI/4)
		setCursor(Qt::SizeVerCursor);
	else
		setCursor(Qt::SizeHorCursor);
	
	theUndoResizePtr = UndoObjectChange::createUndoObject(theParentPtr->getBOPtr());
}

void ResizeAnchor::mouseReleaseEvent ( QGraphicsSceneMouseEvent*)
{
	DEBUG5("ResizeAnchor::mouseReleaseEvent\n");

	if (theUndoResizePtr==NULL)
		return;

	if (theUndoResizePtr->pushYourself()==true)
	{
		DEBUG5("PUSHED UNDO\n");
	}
	else
	{
		DEBUG5("CLEARED UNDO\n");
		theUndoResizePtr->undo();
		delete theUndoResizePtr;
	}
	theUndoResizePtr = NULL;

}

// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################

RotateAnchor::RotateAnchor(Anchors::AnchorType aDirection, AnchorPosition anIndex, Anchors* aParent)
		: Anchor(aDirection, anIndex, aParent), theUndoRotatePtr(NULL)
{
    DEBUG5("RotateAnchor::RotateAnchor(%d, %d, %p)\n", aDirection, anIndex, aParent);
}

float RotateAnchor::getCurrentAngle(Vector aHotspot) const
{
	return (aHotspot - theParentPtr->getBOPtr()->getOrigCenter().toVector()).toAngle();
}


void RotateAnchor::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
	DEBUG5("RotateAnchor::mouseMoveEvent(%d)\n", event->type());

	if(theUndoRotatePtr==NULL)
		return;
	theUndoRotatePtr->update(getCurrentAngle(event->scenePos())-theHotspotAngle);
//	theParentPtr->updatePosition();
}

void RotateAnchor::mousePressEvent ( QGraphicsSceneMouseEvent* event)
{
        DEBUG5("RotateAnchor::mousePressEvent %d, %d\n", theDirection, theIndex);

	// only active this if there are real icons to be had
	if (theDirection==Anchors::NONE)
		return;

	// reset cursor to hori/verti/rotate
	// and setup the corresponding undo class
	assert(theUndoRotatePtr==NULL);

	// store this position - calculate differential angles later
	theUndoRotatePtr = UndoObjectChange::createUndoObject(
			theParentPtr->getBOPtr());
	theHotspotAngle = getCurrentAngle(event->scenePos());
}

void RotateAnchor::mouseReleaseEvent ( QGraphicsSceneMouseEvent*)
{
	DEBUG5("RotateAnchor::mouseReleaseEvent\n");

	if (theUndoRotatePtr==NULL)
		return;

	if (theUndoRotatePtr->pushYourself()==true)
	{
		DEBUG5("PUSHED UNDO\n");
	}
	else
	{
		DEBUG5("CLEARED UNDO\n");
		theUndoRotatePtr->undo();
		delete theUndoRotatePtr;
	}
	theUndoRotatePtr = NULL;

}


// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################

ButtonAnchor::ButtonAnchor(Anchors* aParent, const QString & anIconName,
						   int aButtonIndex)
		: Anchor(Anchors::BUTTON, Anchor::BUTTONBAR, aParent)

{
	theButtonIndex = aButtonIndex;
	setSharedRenderer(ImageStore::getRenderer(anIconName));
	scaleIcon();
}

void ButtonAnchor::mousePressEvent ( QGraphicsSceneMouseEvent* /* event */)
{
	DEBUG5("ButtonAnchor::mousePressEvent\n");
	emit clicked();
}
