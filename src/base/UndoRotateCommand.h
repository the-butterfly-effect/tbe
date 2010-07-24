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

#ifndef UNDOROTATECOMMAND_H
#define UNDOROTATECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <cmath>
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class BaseObject;


/**
  * class UndoRotateCommand
  *
  * undo/redo for the rotation of objects by the user
  */

class UndoRotateCommand : public QUndoCommand
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
	UndoRotateCommand (BaseObject* aBaseObjectPtr, const Vector& aHotSpot);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoRotateCommand ( );

	const Position& getOrigPos(void) const
	{	return theOldCenter; }

	/// @returns true if there is an actual rotation
	virtual bool isChanged(void);

	/// @returns true if the Object is currently not colliding
	bool isGood(void)
	{ return isColliding == false; }

	/** Throw away the new position and rever to the last known good position.
	  * If setNewPosition() has never been called with a new good position,
	  * that means the original position (i.e. object has not moved) !!!
	  *
	  * Automatically calls redo() to effectuate the changes.
	  */
	void revertToLastGood(void);

	/** sets the new rotation. if this rotation is known good,
	  * the "last known good rotation" is set as well.
	  * @param aNewPos	new position of the mouse pointer
	  */
	void setNewRotation(const Vector& aNewPos);

	virtual void redo ();
	virtual void undo ();
	
private:
	BaseObject* theBaseObjectPtr;
	Position	theOldCenter;
	float		theHotSpotAngle;
	float		theLastGoodAngle;
	float		theNewAngle;

	bool isColliding;
};


#endif // UNDOROTATECOMMAND_H
