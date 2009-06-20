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

#ifndef UNDODELETECOMMAND_H
#define UNDODELETECOMMAND_H

#include <assert.h>
#include <QString>
#include <QUndoCommand>
#include "tbe_global.h"

#include "Position.h"

// Forward Declarations
class DrawObject;
class BaseObject;

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
	 * @param aDrawObjectPtr pointer to a DrawObject
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoDeleteCommand (DrawObject* aDrawObjectPtr,
			BaseObject* aBaseObjectPtr);

	/**
	 * Destructor
	 */
	virtual ~UndoDeleteCommand ( );

	
	virtual void redo ();
	virtual void undo ();
	
private:
	BaseObject* theBaseObjectPtr;
	DrawObject* theDrawObjectPtr;
};


#endif // UNDODELETECOMMAND_H
