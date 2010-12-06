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
	
	theOldProperties = theBaseObjectPtr->theProps;
	theNewProperties = theOldProperties;

	theOldID = theBaseObjectPtr->theID;
	theNewID = theBaseObjectPtr->theID;
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
UndoObjectChange::createUndoObject (UndoType anUndoType,
									BaseObject* aBOPtr,
									const Vector & aHotspot )
{
	DEBUG1("UndoObjectChange::createUndoObject(%d,%p,%s)\n",
		   anUndoType, aBOPtr, ASCII(aHotspot.toString()));

	UndoObjectChange* myPtr = new UndoObjectChange(aBOPtr);

	return myPtr;
}


bool UndoObjectChange::pushYourself(void)
{
	QString myUndoString;

	// let's figure out what type of undo we are...

	// did we change ID?
	if (theNewID != theOldID)
	{
		myUndoString = QObject::tr("ID change on %1").arg(theBaseObjectPtr->getName());
		goto letspush;
	}

	// did we change properties?
	if (theNewProperties != theOldProperties)
	{
		myUndoString = QObject::tr("Property change on %1").arg(theBaseObjectPtr->getName());
		goto letspush;
	}

	// did we change size?
	if ((theNewSize == theOldSize) == false)
	{
		myUndoString = QObject::tr("Resize %1").arg(theBaseObjectPtr->getName());
		goto letspush;
	}

	// did we rotate?
	if (theNewCenter.angle != theOldCenter.angle)
	{
		myUndoString = QObject::tr("Rotate %1").arg(theBaseObjectPtr->getName());
		goto letspush;
	}

	// did we move?
	if ((theNewCenter == theOldCenter) == false)
	{
		myUndoString = QObject::tr("Move %1").arg(theBaseObjectPtr->getName());
		goto letspush;
	}

letspush:
	setText(myUndoString);
	DEBUG1("UndoObjectChange::push '%s'\n", ASCII(myUndoString));
	theBaseObjectPtr->theDrawObjectPtr->pushUndo(this);

	return true;
}

void UndoObjectChange::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.dx);
	theBaseObjectPtr->setTheHeight(theNewSize.dy);

	// TODO/FIXME: This was copy/pasted from the original UndoRotateCommand
	// but do we still need this code???
		Position myNewPosition = theNewCenter;
		// It is unfair: QT doesn't redraw if just the angle changed
		// So we need to force a position change...
		double myRandom = 0.0001 * qrand() / (double)RAND_MAX;
		myNewPosition.x += myRandom;

	theBaseObjectPtr->setTempCenter(myNewPosition);
	theBaseObjectPtr->setOrigCenter(myNewPosition);

	theBaseObjectPtr->theProps = theNewProperties;
	theBaseObjectPtr->theID    = theNewID;

	theBaseObjectPtr->reset();


	requestSceneRefresh();

//	if (Anchors::getEditObjectDialogPtr())
//		Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}


/// invalidate both the AABB of the old and new positions (including
/// Anchors) in DrawWorld
void UndoObjectChange::requestSceneRefresh(void)
{
	// very much still TODO

//	theBaseObjectPtr->theDrawObjectPtr->focusRemove();
	theBaseObjectPtr->theDrawObjectPtr->applyPosition();
}


void UndoObjectChange::update (qreal aNewAngle)
{
	theNewCenter.angle = aNewAngle;
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

	theBaseObjectPtr->setTempCenter(theOldCenter);
	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->theProps = theOldProperties;
	theBaseObjectPtr->theID    = theOldID;

	theBaseObjectPtr->reset();

	requestSceneRefresh();

//	if (Anchors::getEditObjectDialogPtr())
//		Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}
