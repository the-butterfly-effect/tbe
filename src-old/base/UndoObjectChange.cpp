/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "UndoObjectChange.h"
#include "DrawObject.h"
#include "Anchors.h"

#include <QGraphicsScene>
#include <cassert>

// Constructors/Destructors
//  

UndoObjectChange::UndoObjectChange (BaseObject* aBaseObjectPtr)
		  : QUndoCommand(),
			theBaseObjectPtr(aBaseObjectPtr)
{
	assert(aBaseObjectPtr);
	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theNewCenter = theOldCenter;
	theLastGoodCenter = theOldCenter;
	
	theOldSize = Vector(theBaseObjectPtr->getTheWidth(),theBaseObjectPtr->getTheHeight());
	theLastGoodSize = theOldSize;
	theNewSize = theOldSize;
	
	theOldProperties = theBaseObjectPtr->theProps;
	theNewProperties = theOldProperties;

	theOldID = theBaseObjectPtr->theID;
	theNewID = theBaseObjectPtr->theID;

	isNowColliding = false;
}

UndoObjectChange::~UndoObjectChange ( )
{
}

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

UndoObjectChange*
UndoObjectChange::createUndoObject (BaseObject* aBOPtr)
{
	UndoObjectChange* myPtr = new UndoObjectChange(aBOPtr);
	DEBUG5("UndoObjectChange::createUndoObject(BO=%p)=%p\n", aBOPtr, myPtr);
	return myPtr;
}

bool UndoObjectChange::isChanged (void) const
{
	return getChangeString().isEmpty() == false;
}


QString UndoObjectChange::getChangeString(void) const
{
	// did we change ID?
	if (theNewID != theOldID)
		return QObject::tr("ID change on %1").arg(theBaseObjectPtr->getName());

	// did we change properties?
	if (theNewProperties != theOldProperties)
		return QObject::tr("Property change on %1").arg(theBaseObjectPtr->getName());

	// did we change size?
	if ((theNewSize == theOldSize) == false)
		return QObject::tr("Resize %1").arg(theBaseObjectPtr->getName());

	// did we rotate?
	if (theNewCenter.angle != theOldCenter.angle)
		return QObject::tr("Rotate %1").arg(theBaseObjectPtr->getName());

	// did we move?
	if ((theNewCenter == theOldCenter) == false)
		return QObject::tr("Move %1").arg(theBaseObjectPtr->getName());

	return "";
}

bool UndoObjectChange::pushYourself(void)
{
	// are we currently good?
	if (isNowColliding == true)
		revertToLastGood();
	if (isNowColliding == true)
		return false;

	// let's figure out what type of undo we are...
	QString myUndoString = getChangeString();

	// if that changestring is empty, we're unchanged...
	if (myUndoString.isEmpty())
	{
		DEBUG3("No change found - not pushing UndoObjectChange\n");
		return false;
	}

	setText(myUndoString);
	DEBUG3("UndoObjectChange::push '%s'\n", ASCII(myUndoString));
	theBaseObjectPtr->theDrawObjectPtr->pushUndo(this);
	return true;
}

void UndoObjectChange::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.dx);
	theBaseObjectPtr->setTheHeight(theNewSize.dy);

	// It is unfair: QT doesn't redraw if just the angle changed
	// So we need to force a position change...
	Position myNewPosition = theNewCenter;
	double myRandom = 0.0001 * qrand() / (double)RAND_MAX;
	myNewPosition.x += myRandom;
	theBaseObjectPtr->setOrigCenter(myNewPosition);

	theBaseObjectPtr->theProps = theNewProperties;
	theBaseObjectPtr->theID    = theNewID;

	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);

	requestSceneRefresh();

//	if (Anchors::getEditObjectDialogPtr())
//		Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}


void UndoObjectChange::revertToLastGood(void)
{
	update(theLastGoodCenter, theLastGoodSize);
}

void inline updateMinMax(Position& aMin, Position& aMax, const Position& aNew)
{
	if (aNew.x < aMin.x)    aMin.x = aNew.x;
	if (aNew.x > aMax.x)    aMax.x = aNew.x;
	if (aNew.y < aMin.y)    aMin.y = aNew.y;
	if (aNew.y > aMax.y)    aMax.y = aNew.y;
}

/// invalidate both the AABB of the old and new positions in DrawWorld
void UndoObjectChange::requestSceneRefresh(void)
{
	Position myMin = theOldCenter + -1*theOldSize;
	Position myMax = myMin;

	// calculate the AABB
	updateMinMax(myMin, myMax, theOldCenter + theOldSize);
	updateMinMax(myMin, myMax, theNewCenter + theNewSize);
	updateMinMax(myMin, myMax, theNewCenter + -1*theNewSize);

	theBaseObjectPtr->theDrawObjectPtr->applyPosition();
	if (theBaseObjectPtr->theDrawObjectPtr->theAnchorsPtr)
		theBaseObjectPtr->theDrawObjectPtr->theAnchorsPtr->updatePosition();
	theBaseObjectPtr->theDrawObjectPtr->prepareGeometryChange();

	theBaseObjectPtr->theDrawObjectPtr->scene()->
			update(QRectF(myMin.toVector().toQPointF(), myMax.toVector().toQPointF()));
}


void UndoObjectChange::update (qreal aNewAngle)
{
	theNewCenter.angle = theOldCenter.angle+aNewAngle;
	update(theNewCenter, theNewSize);
}

void UndoObjectChange::update(const Vector& aNewPos)
{
	theNewCenter.x = aNewPos.dx;
	theNewCenter.y = aNewPos.dy;
	update(theNewCenter, theNewSize);
}

void UndoObjectChange::update (const Position& aNewPos, const Vector& aNewSize)
{
	theNewCenter = aNewPos;
	theNewSize   = aNewSize;

	redo();

	// collision detection - only do this *after* the redraw (part of redo) :-)
	isNowColliding = theBaseObjectPtr->theDrawObjectPtr->checkForCollision();
	if (isNowColliding == false)
	{
		theLastGoodCenter= theNewCenter;
		theLastGoodSize  = theNewSize;
	}
	requestSceneRefresh();
}

void UndoObjectChange::update(const QString& aKey, const QString& aValue)
{
	theNewProperties.setProperty(aKey, aValue);
}

void UndoObjectChange::update(const QString& anID)
{
	theNewID = anID;
}

void UndoObjectChange::undo ()
{
	theBaseObjectPtr->setTheWidth(theOldSize.dx);
	theBaseObjectPtr->setTheHeight(theOldSize.dy);

	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->theProps = theOldProperties;
	theBaseObjectPtr->theID    = theOldID;

	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);

	requestSceneRefresh();

//	if (Anchors::getEditObjectDialogPtr())
//		Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}
