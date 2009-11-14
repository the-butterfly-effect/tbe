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
class DrawObject;
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
	 * @param aDrawObjectPtr pointer to a DrawObject
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoMoveCommand (DrawObject* aDrawObjectPtr, 
			BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoMoveCommand ( );

	const Position& getOrigPos(void)
	{	return theOldPosition; }

	void setNewPosition(const Position& aNewPos)
	{	theNewPosition = aNewPos; }

	bool hasMoved(void)
	{	return theOldPosition == theNewPosition; }

	virtual void redo ();
	virtual void undo ();
	
private:
	BaseObject* theBaseObjectPtr;
	DrawObject* theDrawObjectPtr;
	Position	theOldPosition;
	Position	theNewPosition;
};


#endif // UNDOMOVECOMMAND_H
