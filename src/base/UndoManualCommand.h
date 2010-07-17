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

#ifndef UNDOMANUALCOMMAND_H
#define UNDOMANUALCOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QPointF>
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class DrawObject;
class BaseObject;


/**
  * class UndoManualCommand
  *
  * undo/redo for the moving/resizing of objects through the EditObjectDialog
  */

class UndoManualCommand : public QUndoCommand
{
public:

	// Constructors/Destructors
	//  

	/** constructor
	 * 
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoManualCommand (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoManualCommand ( );

	
	virtual void redo ();
	virtual void undo ();

	/** Adjust
	  */
	void setNewVal(Position aPos);
	void setNewVal(qreal anWidth, qreal aHeight);

	/// @returns true if the Object is currently not colliding
	bool isGood(void)
	{ return isColliding == false; }

	/// @returns true if there is an actual resize
	bool isResized(void);

	void pushYourself(void);

	/** Throw away the new delta and rever to the last known good delta.
	  * If setNewPosition() has never been called with a new good delta,
	  * that means the original position (i.e. object has not moved) !!!
	  *
	  * Automatically calls redo() to effectuate the changes.
	  */
	void revertToLastGood(void);

private:
	BaseObject* theBaseObjectPtr;

	Position	theOldCenter;
	Position	theLastGoodCenter;
	Position	theNewCenter;
	QPointF		theOldSize;
	QPointF		theLastGoodSize;
	QPointF		theNewSize;

	bool isColliding;
};

#endif // UndoMANUALCommand_H
