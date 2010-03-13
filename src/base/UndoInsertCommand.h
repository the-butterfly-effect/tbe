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

#ifndef UNDOINSERTCOMMAND_H
#define UNDOINSERTCOMMAND_H

#include "UndoDeleteCommand.h"

/**
  * class UndoInsertCommand
  *
  * the undo handler for inserting an object from the toolbox
  */

class UndoInsertCommand : public UndoDeleteCommand
{
public:

	// Constructors/Destructors
	//  

	/** constructor
	 * 
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoInsertCommand (BaseObject* aBaseObjectPtr);

	/**
	 * Destructor
	 */
	virtual ~UndoInsertCommand ( );

	/** (overridden from QUndoCommand)
	  * will register/reregister the BaseObject
	  */
	virtual void redo ();

	/** (overridden from QUndoCommand)
	  * will unregister BaseObject and "return it" to ToolBox
	  */
	virtual void undo ();

	/** D&D will always require a move after the insert.
	  * i.e. during the dragMoveEvent(), we will have to update the
	  * position of the not-yet-inserted object
	  * @param aNewPos the new position
	  */
	void setNewPosition(const Position& aNewPos);
};

#endif // UNDOINSERTCOMMAND_H
