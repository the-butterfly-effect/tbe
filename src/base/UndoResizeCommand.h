/* The Butterfly Effect 
 * This file copyright (C) 2009,2010 Klaas van Gend
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

#ifndef UNDORESIZECOMMAND_H
#define UNDORESIZECOMMAND_H

#include <QString>
#include <QUndoCommand>
//#include <QPointF>
#include "tbe_global.h"

#include "Position.h"
#include "Anchors.h"

// Forward Declarations
class DrawObject;
class BaseObject;

/**
  * class UndoResizeCommand
  *
  * undo/redo for the moving of objects
  */

class UndoResizeCommand : public QUndoCommand
{
public:

	// Constructors/Destructors
	//  

	/** constructor
	 * 
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoResizeCommand (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoResizeCommand ( );

	
	/// @returns true if there is an actual resize
	virtual bool isChanged(void);

	/// @returns true if the Object is currently not colliding
	bool isGood(void)
	{ return isColliding == false; }

	/** Throw away the new delta and rever to the last known good delta.
	  * If setNewPosition() has never been called with a new good delta,
	  * that means the original position (i.e. object has not moved) !!!
	  *
	  * Automatically calls redo() to effectuate the changes.
	  */
	void revertToLastGood(void);

	/** updates the resizing information
	  * if this new size does not result in a collission, the "last known good" is set as well.
	  * @param anIndex indication of which Anchor is involved
	  * @param aCursorPos the actual position
	  */
	void update(Anchor::AnchorPosition anIndex, const Vector& aCursorPos);

	virtual void redo ();
	virtual void undo ();

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

#endif // UndoResizeCommand_H
