#include "ObjectFactory.h"

//////////////////////////////////////////////////////////////////////////////

/** small, only locally used class
  */
class FactoryList
{
public:
	void insert(const QString& aName, ObjectFactory* theFactoryPtr)
		{ theMap[aName]=theFactoryPtr; }
	const ObjectFactory* getFactoryPtr(const QString& aName) const
		{ return theMap.value(aName); }

	ObjectFactory::ObjectFactoryList* getAllFactories(void)
	{
		ObjectFactory::ObjectFactoryList* myList = new ObjectFactory::ObjectFactoryList();
		TheMap::iterator myI = theMap.begin();
		while (myI != theMap.end())
		{
			myList->push_back(*myI);
			++myI;
		}
		return myList;
	}

private:
	typedef QMap<QString, ObjectFactory*> TheMap;
	TheMap theMap;
};
static FactoryList* theFactoryListPtr=NULL;


void
ObjectFactory::announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr)
{
	DEBUG4("ObjectFactory::announceObjectType(\"%s\", %p)\n",
			ASCII(anObjectTypeName), aThisPtr);
	if (theFactoryListPtr==NULL)
		theFactoryListPtr = new FactoryList();
	theFactoryListPtr->insert(anObjectTypeName,aThisPtr);
	aThisPtr->theFactoryName = anObjectTypeName;
}

AbstractObject*
ObjectFactory::createObject(
		const QString& aName,
		const Position aPosition,
		const qreal aWidth,
		const qreal anHeight)
{
	const ObjectFactory* myFactoryPtr = theFactoryListPtr->getFactoryPtr(aName);
	DEBUG5("ObjectFactory::createObject(\"%s\") Factory=%p\n", ASCII(aName), myFactoryPtr);
	if (myFactoryPtr == NULL)
	{
		DEBUG1("There is no factory for Object type %s\n", ASCII(aName));
		return NULL;
	}
	AbstractObject* myObjectPtr = myFactoryPtr->createObject();
	DEBUG5("  object created = %p, i18n name = '%s'\n", myObjectPtr, ASCII(myObjectPtr->getName()));
	assert (aName.contains(" ")==false);
	myObjectPtr->theInternalName = aName;
	myObjectPtr->theCenter=aPosition;
	if (aWidth!=1.0)
		myObjectPtr->theWidth=aWidth;
	if (anHeight!=1.0)
		myObjectPtr->theHeight=anHeight;
	return myObjectPtr;
}


AbstractObject* ObjectFactory::fixObject(AbstractObject* anObject) const
{
	anObject->theInternalName = theFactoryName;
	return anObject;
}

ObjectFactory::ObjectFactoryList* ObjectFactory::getAllFactories(void)
{
	if (theFactoryListPtr==NULL)
		return NULL;
	return theFactoryListPtr->getAllFactories();
}
