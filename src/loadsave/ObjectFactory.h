/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "Position.h"
#include <QList>

// forward declarations
class AbstractObject;

/**
 *  class ObjectFactory
 *
 *  This class will be implemented for each "final" Object type.
 *  There should be a static instance in each Object's cpp file.
 *  At system start it will announce the type to Level and do the real Object creation.
 */
class ObjectFactory
{
	// there's nothing public here - nobody should call anything in this class
	// directly.
public:
	/// empty virtual destructor
	virtual ~ObjectFactory() {;}

	typedef QList<const ObjectFactory*> ObjectFactoryList;

protected:
	/** create the object aName and return a pointer to it
	 * @param aName
	 * @param aPostion
	 * @param aWidth    width of the object in meter - default 1.0
	 * @param anHeight  height of the object in meter - default 1.0
	 * @return a valid pointer to a newly created object or NULL if not found
	 */
	static AbstractObject* createObject(
			const QString& aName,
			const Position aPostion,
			const qreal aWidth = 1.0,
			const qreal anHeight = 1.0);

	static void announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr);

	/// setup e.g. theInternalName of the object
	AbstractObject* fixObject(AbstractObject*) const;

	QString getFactoryName(void) const
	{ return theFactoryName; }

	/** create an ObjectFactoryList with pointers to all known ObjectFactories.
	 *  it is up to the caller (usually ToolBoxItemListModel) to delete the list
	 *  - do not delete the contents of the list.
	 */
	static ObjectFactoryList* getAllFactories(void);

	/** pure virtual function that creates an object of the type the factory is for
	 *  must be implemented for each real factory.
	 * @return pointer to a newly instantiated object
	 */
	virtual AbstractObject* createObject(void) const = 0;

private:
	QString theFactoryName;

    friend class MainWindow;
};

#endif // OBJECTFACTORY_H
