/* The Butterfly Effect 
 * This file copyright (C) 2009-2010  Klaas van Gend
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

#ifndef UNDODELETECOMMAND_H
#define UNDODELETECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class DrawObject;
class BaseObject;
class ToolBox;

/**
  * class UndoDeleteCommand
  *
  * removes the DrawObject and BaseObject from the DrawWorld and World, but in such fashion
  * that it can redo that - if needed
  */

class UndoDeleteCommand : public QUndoCommand
{
public:

	// Constructors/Destructors
	//  

	/** constructor
	 * 
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoDeleteCommand (BaseObject* aBaseObjectPtr);

	/**
	 * Destructor
	 */
	virtual ~UndoDeleteCommand ( );

	/** called by resizinggraphicsview to confirm that the Drag/Drop succeeded
	  * this member will push itself onto the UndoStack and announce the
	  * modification to the ToolBox
	  */
	void push();

	/** (overridden from QUndoCommand)
	  * will deregister BaseObject and DrawObject
	  * and announce itself to ToolBox
	  */
	virtual void redo ();

	/** each ToolBox constructor sets a pointer to iself so UDC can
	 *  announce itself to the Toolbox - after all,
	 *  a deleted object should re-appear in the ToolBox
	 */
	static void setToolBoxPtr(ToolBox* aPtr);

	/** (overridden from QUndoCommand)
	  * will reregister BaseObject and DrawObject
	  * and announce that to ToolBox
	  */
	virtual void undo ();

protected:
	BaseObject* theBaseObjectPtr;

	/// true if this UndoDeleteCommand was pushed onto the undo stack
	bool isInUse;

	static ToolBox* getCurrentToolboxPtr(void);

private:
	/// kill possibility for copy constructor - do not implement
	UndoDeleteCommand(const UndoDeleteCommand&);

	/// kill possibility for assignment operator - do not implement
	const UndoDeleteCommand& operator=(const UndoDeleteCommand&);

};


#endif // UNDODELETECOMMAND_H
