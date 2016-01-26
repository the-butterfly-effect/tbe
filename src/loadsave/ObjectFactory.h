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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "AbstractObjectPtr.h"
#include "Position.h"
#include <QObject>
#include <QList>

/**
 *  class ObjectFactory
 *
 *  This class will be implemented for each "final" Object type.
 *  There should be a static instance in each Object's cpp file.
 *  At system start it will announce the type to Level and do the real Object creation.
 */
class ObjectFactory : public QObject
{
    Q_OBJECT

	// there's nothing public here - nobody should call anything in this class
	// directly.
public:
	/// empty virtual destructor
    virtual ~ObjectFactory() {;}

	typedef QList<const ObjectFactory*> ObjectFactoryList;

    /// static factory method member, to be used by all classes that have childs for
    /// which there is no ObjectFactory - like DetonatorBoxHandle and SpringEnd
    /// Usage: std::shared_ptr<ChildObjectName> ChildPtr = createChildObject<ChildObjectName)(constructor arguments);
    /// but instead of the std::shared<CON>, you can also use the superclass shared ptr: AbstractObjectPtr
    /// @param _Tp  is ChildObjectName - class name of the child object
    /// @param __args  are the arguments to be forwarded to the constructor of the child object
    /// @returns a shared pointer
    template<typename _Tp, typename... _Args>
      static std::shared_ptr<_Tp>
      createChildObject(_Args&&... __args)
      {
        std::shared_ptr<_Tp> p = std::make_shared<_Tp>(std::forward<_Args>(__args)...);
        p->theThisPtr = p;
        return p;
      }


protected:
	/** create the object aName and return a pointer to it
	 * @param aName
	 * @param aPostion
	 * @param aWidth    width of the object in meter - default 1.0
	 * @param anHeight  height of the object in meter - default 1.0
     * @returns a valid shared_ptr to a newly created AbstractObject
     *          or nullptr if anything went wrong
	 */
    static AbstractObjectPtr createObject(
			const QString& aName,
			const Position aPostion,
			const qreal aWidth = 1.0,
			const qreal anHeight = 1.0);

    /// clone anObject, i.e. create another object with similar properties
    /// @note: property ID is TODO
    static AbstractObjectPtr cloneObject(const AbstractObjectPtr anOriginalPtr);

    static void announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr);

	/// setup e.g. theInternalName of the object
    AbstractObject* fixObject(AbstractObject*anObjectPtr) const;

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

    friend class LevelCreator;
};

#endif // OBJECTFACTORY_H
