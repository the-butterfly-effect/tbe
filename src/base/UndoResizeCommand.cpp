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

#include "UndoResizeCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"
#include "EditObjectDialog.h"

#include <cassert>
#include <cmath>

UndoRCommand::UndoRCommand()
		: QUndoCommand()
{}

UndoRCommand::~UndoRCommand()
{}

// Constructors/Destructors
//  

UndoResizeCommand::UndoResizeCommand (
		BaseObject* aBaseObjectPtr)
		  : UndoRCommand(),
			theBaseObjectPtr(aBaseObjectPtr)
{
	assert(aBaseObjectPtr);
	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theLastGoodCenter = theOldCenter;
	theNewCenter = theBaseObjectPtr->getTempCenter();
	theOldSize = QPointF(theBaseObjectPtr->getTheWidth(),theBaseObjectPtr->getTheHeight());
	theLastGoodSize = theOldSize;
	setText(QObject::tr("Resize %1").arg(theBaseObjectPtr->getName()));
}

UndoResizeCommand::~UndoResizeCommand ( )
{
	DEBUG5("~UndoResizeCommand() for %p - %s\n", this, ASCII(text()));
	// there's nothing to do here...
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

bool UndoResizeCommand::isChanged(void)
{
	if (theNewSize.isNull())
		return false;

	QPointF theDelta = theOldSize-theNewSize;
	if (fabs(theDelta.x()) < Position::minimalMove && fabs(theDelta.y()) < Position::minimalMove)
		return false;
	return true;
}


void UndoResizeCommand::redo ()
{
	theBaseObjectPtr->setTheWidth(theNewSize.x());
	theBaseObjectPtr->setTheHeight(theNewSize.y());

	theBaseObjectPtr->setTempCenter(theNewCenter);
	theBaseObjectPtr->setOrigCenter(theNewCenter);

	theBaseObjectPtr->reset();
	theBaseObjectPtr->theDrawObjectPtr->applyPosition();
	Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}


void UndoResizeCommand::revertToLastGood(void)
{
	theNewSize = theLastGoodSize;
	theNewCenter = theLastGoodCenter;
	theBaseObjectPtr->theDrawObjectPtr->removeCollisionCross();
	redo();
}


void UndoResizeCommand::undo ()
{
	theBaseObjectPtr->setTheWidth(theOldSize.x());
	theBaseObjectPtr->setTheHeight(theOldSize.y());

	theBaseObjectPtr->setTempCenter(theOldCenter);
	theBaseObjectPtr->setOrigCenter(theOldCenter);

	theBaseObjectPtr->reset();
	theBaseObjectPtr->theDrawObjectPtr->focusRemove();
	theBaseObjectPtr->theDrawObjectPtr->applyPosition();
	Anchors::getEditObjectDialogPtr()->readFromObject(theBaseObjectPtr);
}


void UndoResizeCommand::update(
		Anchor::AnchorPosition anIndex,
		const Vector& aCursorPos)
{
	// Essentially, we're going to project the current cursor position
	// across the axis through the object center and the anchor
	// the length of the projection is used to calculate the new
	// width of the object.
	// The goal is to keep the anchor at the other end of the object at the
	// same position.

	Vector myVectorToCenter = Vector(aCursorPos) - theOldCenter.toVector();
	float myAngle = myVectorToCenter.toAngle();
	float myAxisAngle = anIndex*PI/4 + theOldCenter.angle;
	float myLengthAcrossAxis = myVectorToCenter.length()*cos(myAngle-myAxisAngle);

	// This works - even for larger angles!
	if (anIndex==Anchor::RIGHT || anIndex==Anchor::LEFT)
	{
		float myOldWidth = theOldSize.x()/2.0;
		if (myLengthAcrossAxis+myOldWidth < 0.1)
			return;
		theNewSize = QPointF(myLengthAcrossAxis+myOldWidth, theOldSize.y());
		if (anIndex==Anchor::RIGHT)
			theNewCenter = theOldCenter + Vector(0.5*(myLengthAcrossAxis-myOldWidth),0);
		if (anIndex==Anchor::LEFT)
			theNewCenter = theOldCenter + Vector(-0.5*(myLengthAcrossAxis-myOldWidth),0);
	}
	if (anIndex==Anchor::TOP || anIndex==Anchor::BOTTOM)
	{
		float myOldHeight= theOldSize.y()/2.0;
		if (myLengthAcrossAxis+myOldHeight < 0.1)
			return;
		theNewSize = QPointF(theOldSize.x(), myLengthAcrossAxis+myOldHeight);
		if (anIndex==Anchor::TOP)
			theNewCenter = theOldCenter + Vector(0, 0.5*(myLengthAcrossAxis-myOldHeight));
		if (anIndex==Anchor::BOTTOM)
			theNewCenter = theOldCenter + Vector(0, -0.5*(myLengthAcrossAxis-myOldHeight));
	}

	redo();

	// collision detection - only do this *after* the redraw :-)
	isColliding = theBaseObjectPtr->theDrawObjectPtr->checkForCollision();
	if (isColliding == false)
	{
		theLastGoodSize = theNewSize;
		theLastGoodCenter = theNewCenter;
	}
}
