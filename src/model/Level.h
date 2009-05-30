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

#ifndef LEVEL_H
#define LEVEL_H

#include <assert.h>
#include <QString>
#include <QList>
#include "tbe_global.h"
#include "BaseObject.h"

// Forward Declarations:
class MainWindow;
class World;

/**
  * class Level
  *
  * the level reads the objects from a file and creates the world containing
  * the various objects
  * 
  * FIXME: for now, Level is just a single, hardcoded level.
  * File parsing will only come in Milestone 2.
  */

class Level : public ObjectFactory
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	Level ( );

	/**
	 * Empty Destructor
	 */
	virtual ~Level ( );

public:
	// Public accessor methods
	//

	/// returns the Level's title
	virtual const QString getName ( ) const
		{ return "Bowling for Butterflies"; }
	
	World* getTheWorldPtr(void)
		{ return theWorldPtr; }


private:	
	World* theWorldPtr;

	/// implementation of ObjectFactory - not needed in Level...
	virtual BaseObject* createObject(void) const { return NULL; }
};

#endif // LEVEL_H
