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

#include "UndoRotateCommand.h"
#include "BaseObject.h"
#include "DrawObject.h"

#include <cassert>

// Constructors/Destructors
//  

UndoRotateCommand::UndoRotateCommand (
		BaseObject* aBaseObjectPtr,
		const Vector& aHotSpot)
		  : UndoRCommand(),
			theBaseObjectPtr(aBaseObjectPtr)
{
	assert(aBaseObjectPtr);

	setText(QObject::tr("Rotate %1").arg(theBaseObjectPtr->getName()));
	DEBUG5("UndoRotateCommand() for %p - %s\n", this, ASCII(text()));

	theOldCenter = theBaseObjectPtr->getOrigCenter();
	theLastGoodAngle = theOldCenter.angle;

	theHotSpotAngle = (aHotSpot - theOldCenter.toVector()).toAngle();
}

UndoRotateCommand::~UndoRotateCommand ( )
{
	DEBUG5("~UndoRotateCommand() for %p - %s\n", this, ASCII(text()));
	// there's nothing to do here...
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

bool UndoRotateCommand::isChanged(void)
{
	if (fabsf(theNewAngle - theOldCenter.angle) < PI/180)
		return false;
	return true;
}

void UndoRotateCommand::redo ()
{
	Position myNewPosition = theOldCenter;
	myNewPosition.angle = theNewAngle;

	// It is unfair: QT doesn't redraw if just the angle changed
	// So we need to force a position change...
	double myRandom = 0.0001 * qrand() / (double)RAND_MAX;
	myNewPosition.x += myRandom;

	theBaseObjectPtr->setOrigCenter(myNewPosition);
	theBaseObjectPtr->reset();
	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);
	theBaseObjectPtr->theDrawObjectPtr->advance(0);
}


void UndoRotateCommand::revertToLastGood(void)
{
	theNewAngle = theLastGoodAngle;
	redo();
}


void UndoRotateCommand::update(Anchor::AnchorPosition, const Vector& aNewPos)
{
	Vector theDeltaVector = aNewPos - theOldCenter.toVector();
	theNewAngle = (theDeltaVector.toAngle() - theHotSpotAngle) + theOldCenter.angle;

	redo();

	// collision detection - only do this *after* the redraw :-)
	isColliding = theBaseObjectPtr->theDrawObjectPtr->checkForCollision();
	if (isColliding == false)
		theLastGoodAngle = theNewAngle;
}



void UndoRotateCommand::undo ()
{
	theBaseObjectPtr->setOrigCenter(theOldCenter);
	theBaseObjectPtr->reset();
	theBaseObjectPtr->notifyJoints(JointInterface::POSUPDATE);
	theBaseObjectPtr->theDrawObjectPtr->focusRemove();
	theBaseObjectPtr->theDrawObjectPtr->advance(0);
}
