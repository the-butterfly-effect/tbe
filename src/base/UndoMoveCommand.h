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

#ifndef UNDOMOVECOMMAND_H
#define UNDOMOVECOMMAND_H

#include <assert.h>
#include <QString>
#include <QUndoCommand>
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class BaseObject;


/**
  * class UndoMoveCommand
  *
  * undo/redo for the moving of objects by the user
  */

class UndoMoveCommand : public QUndoCommand
{
public:

	// Constructors/Destructors
	//  

	/** constructor
	 * this constructor will automatically retrieve the "old" Original
	 * position from aBaseObjectPtr
	 * 
	 * @param aBaseObjectPtr pointer to a BaseObject
	 * @param aHotSpot mouse position offset from the center of the object
	 */
	UndoMoveCommand (BaseObject* aBaseObjectPtr, const Vector& aHotSpot);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoMoveCommand ( );

	const Position& getOrigPos(void)
	{	return theOldPosition; }

	/** Throw away the new position and rever to the last known good position.
	  * If setNewPosition() has never been called with a new good position,
	  * that means the original position (i.e. object has not moved) !!!
	  *
	  * Automatically calls redo() to effectuate the changes.
	  */
	void revertToLastGood(void)
	{	theNewPosition = theLastGoodPosition; redo();}

	/** sets the new position. if this position is known good, the "last known good position" is set as well.
	  * @param aNewPos	new position
	  * @param isGood   true if new position is known good (i.e. is not colliding)
	  */
	void setNewPosition(const Position& aNewPos, bool isGood)
	{
		theNewPosition = aNewPos;
		theNewPosition.x -= theHotSpot.dx;
		theNewPosition.y -= theHotSpot.dy;
		if (isGood)
			theLastGoodPosition = aNewPos;
	}

	bool hasMoved(void)
	{	return theOldPosition == theNewPosition; }

	virtual void redo ();
	virtual void undo ();
	
private:
	BaseObject* theBaseObjectPtr;
	Position	theLastGoodPosition;
	Position	theNewPosition;
	Position	theOldPosition;
	Vector		theHotSpot;
};


#endif // UNDOMOVECOMMAND_H
