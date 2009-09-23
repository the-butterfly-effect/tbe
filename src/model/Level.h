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
#include <QDomDocument>
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

	/// returns the Level's title
	virtual const QString& getName ( ) const
		{ return theLevelName; }
	
	World* getTheWorldPtr(void)
		{ return theWorldPtr; }

	/** open file containing a level definition, parse it, build the Level
	 * 
	 * @param aFileName file to parse and populate Level with.
	 * @return empty string is returned if loading was successful, otherwise
	 *			the return will contain the i18n'ed error message
	 */
	QString load(const QString& aFileName);
	
	/** save the Level to a file
	 *  the file name must be unique - overwriting is not allowed here
	 * 
	 * @param aFileName file to serialize the Level/World to.
	 * @return false if saving failed - error message will be set.
	 */
	bool save(const QString& aFileName);

	/// returns the path to the file that describes the current level
	static QString getPathToLevelFile(void);

protected:
	// TODO FIXME: move these two somewhere else so we no longer need the #include for QDomElement here
	void addTextElement(QDomElement aParent, const QString& anElementName, const QString& aText) const;
	void addBaseObject(QDomElement aParent, const BaseObject& anObjectRef) const;
	
private:	
	World* theWorldPtr;

	/// implementation of ObjectFactory - not needed in Level...
	virtual BaseObject* createObject(void) const { return NULL; }
	
	QString theLevelName;
	QString theLevelAuthor;
	QString theLevelLicense;
	QString	theLevelDescription;
	QString theLevelDate;

	friend class SaveLevelInfo;
};

#endif // LEVEL_H
