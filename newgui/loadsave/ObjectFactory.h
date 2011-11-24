#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

//////////////////////////////////////////////////////////////////////////////

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
	static BaseObject* createObject(
			const QString& aName,
			const Position aPostion,
			const qreal aWidth = 1.0,
			const qreal anHeight = 1.0);

	static void announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr);

	/// setup e.g. theInternalName of the object
	BaseObject* fixObject(BaseObject*) const;

	QString getFactoryName(void) const
	{ return theFactoryName; }

	/** create an ObjectFactoryList with pointers to all known ObjectFactories.
	 *  it is up to the caller (usually ToolBoxItemListModel) to delete the list
	 *  - do not delete the contents of the list.
	 */
	static ObjectFactoryList* getAllFactories(void);
	friend class ToolBox;

	/** pure virtual function that creates an object of the type the factory is for
	 *  must be implemented for each real factory.
	 * @return pointer to a newly instantiated object
	 */
	virtual BaseObject* createObject(void) const = 0;
	friend class TBItem;
private:
	QString theFactoryName;
};

#endif // OBJECTFACTORY_H
