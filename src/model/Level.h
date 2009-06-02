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
#include <QObject>
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
  * Level inherits QObject to make i18n of error messages easier 
  */

class Level :  public QObject, public ObjectFactory
{
	Q_OBJECT

	// Constructors/Destructors
	//  
public:
	
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

	/** returns an already internationalized error message if load() or
	 *  save() failed.
	 *  
	 *  if load() or save() succeeded or if other members were run,
	 *  the output is undefined (probably an empty string, though)
	 *  
	 * @return a string with the error.
	 */ 
	const QString& getErrorMessage(void)
		{ return theErrorMessage; }

	/// returns the Level's title
	virtual const QString getName ( ) const
		{ return "Bowling for Butterflies"; }
	
	World* getTheWorldPtr(void)
		{ return theWorldPtr; }

	/** open file containing a level definition, parse it, build the Level
	 * 
	 * @param aFileName file to parse and populate Level with.
	 * @return false if loading failed - error message will be set.
	 */
	bool load(const QString& aFileName);
	
	/** save the Level to a file
	 *  the file name must be unique - overwriting is not allowed here
	 * 
	 * @param aFileName file to serialize the Level/World to.
	 * @return false if saving failed - error message will be set.
	 */
	//bool save(const QString& aFileName);
	
private:	
	World* theWorldPtr;

	/// implementation of ObjectFactory - not needed in Level...
	virtual BaseObject* createObject(void) const { return NULL; }
	
	QString theLevelName;
	QString theLevelAuthor;
	QString theLevelLicense;
	
	/** Contains an error message if load() or save() failed.
	 *  The message was i18n'ed when it was set - no need to run tr() again.
	 */ 
	QString theErrorMessage;
};

#endif // LEVEL_H
