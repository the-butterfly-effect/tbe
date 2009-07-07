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

#ifndef UNDORESIZECOMMAND_H
#define UNDORESIZECOMMAND_H

#include <assert.h>
#include <QString>
#include <QUndoCommand>
#include <QPointF>
#include "tbe_global.h"

#include "Position.h"

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
	 * @param aDrawObjectPtr pointer to a DrawObject
	 * @param aBaseObjectPtr pointer to a BaseObject
	 */
	UndoResizeCommand (DrawObject* aDrawObjectPtr,
			BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~UndoResizeCommand ( );

	
	virtual void redo ();
	virtual void undo ();

	void setDelta(qreal anAnchorPos, QPointF aDelta);

	/// @returns true if there is an actual resize
	bool isResized(void);

private:
	BaseObject* theBaseObjectPtr;
	DrawObject* theDrawObjectPtr;

	Position	theOldCenter;
	Position	theNewCenter;
	QPointF		theOldSize;
	QPointF		theNewSize;
};

#endif // UndoResizeCommand_H
