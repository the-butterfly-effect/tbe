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

#ifndef DRAWNOTEIT_H
#define DRAWNOTEIT_H

#include "DrawObject.h"
#include "NoteIt.h"

// forward declarations
class BaseObject;


/** class DrawNoteIt
  *
  * This class draws the NoteIt class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class DrawNoteIt : public DrawObject
{
public:
	
	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	DrawNoteIt (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawNoteIt ( );


protected:
    // Protected attribute accessor methods
	//  

private:
	virtual void initAttributes ( ) ;
};

#endif // DRAWNOTEIT_H
