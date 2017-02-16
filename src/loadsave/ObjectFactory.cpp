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

#include "AbstractObject.h"
#include "ObjectFactory.h"

#include <cassert>

/** small, only locally used class
  */
class FactoryList
{
public:
    void insert(const QString &aName, ObjectFactory *theFactoryPtr)
    {
        theMap[aName] = theFactoryPtr;
    }
    const ObjectFactory *getFactoryPtr(const QString &aName) const
    {
        return theMap.value(aName);
    }

    ObjectFactory::ObjectFactoryList *getAllFactories(void)
    {
        ObjectFactory::ObjectFactoryList *myList = new ObjectFactory::ObjectFactoryList();
        TheMap::iterator myI = theMap.begin();
        while (myI != theMap.end()) {
            myList->push_back(*myI);
            ++myI;
        }
        return myList;
    }

private:
    typedef QMap<QString, ObjectFactory *> TheMap;
    TheMap theMap;
};
static FactoryList *theFactoryListPtr = nullptr;


void
ObjectFactory::announceObjectType(const QString &anObjectTypeName, ObjectFactory *aThisPtr)
{
    DEBUG5("ObjectFactory::announceObjectType(\"%s\", %p)",
           ASCII(anObjectTypeName), aThisPtr);
    if (theFactoryListPtr == nullptr)
        theFactoryListPtr = new FactoryList();
    theFactoryListPtr->insert(anObjectTypeName, aThisPtr);
    aThisPtr->theFactoryName = anObjectTypeName;
}

AbstractObjectPtr
ObjectFactory::createObject(
    const QString &aName,
    const Position aPosition,
    const qreal aWidth,
    const qreal anHeight)
{
    const ObjectFactory *myFactoryPtr = theFactoryListPtr->getFactoryPtr(aName);
    DEBUG5("ObjectFactory::createObject(\"%s\") Factory=%p", ASCII(aName), myFactoryPtr);
    if (myFactoryPtr == nullptr) {
        DEBUG1("There is no factory for Object type '%s'", ASCII(aName));
        return nullptr;
    }
    AbstractObject *myObjectPtr = myFactoryPtr->createObject();
    assert (myObjectPtr != nullptr);
    AbstractObjectPtr mySharedOPtr = AbstractObjectPtr(myObjectPtr);
    myObjectPtr->theThisPtr = mySharedOPtr;
    DEBUG5("  object created = %p, i18n name = '%s'", myObjectPtr, ASCII(myObjectPtr->getName()));
    assert (aName.contains(" ") == false);
    myObjectPtr->theInternalName = aName;
    myObjectPtr->theCenter = aPosition;
    if (aWidth != 1.0)
        myObjectPtr->theWidth = aWidth;
    if (anHeight != 1.0)
        myObjectPtr->theHeight = anHeight;
    // finally, get rid of the actual pointer and return the shared_ptr
    assert (nullptr != mySharedOPtr);
    return mySharedOPtr;
}

AbstractObjectPtr
ObjectFactory::cloneObject(const AbstractObjectPtr anOriginalPtr)
{
    // *** use our factory to create a similar object
    AbstractObjectPtr myClonePtr = createObject(
                                       anOriginalPtr->getInternalName(),
                                       anOriginalPtr->theCenter,
                                       anOriginalPtr->theWidth,
                                       anOriginalPtr->theHeight);
    // *** copy all Properties that make sense
    myClonePtr->theProps = anOriginalPtr->theProps;

    // *** copy everything else
    myClonePtr->theToolTip = anOriginalPtr->theToolTip;
    myClonePtr->hasCustomToolTip = anOriginalPtr->hasCustomToolTip;

    // *** we're done
    return myClonePtr;
}


AbstractObject *ObjectFactory::fixObject(AbstractObject *anObjectPtr) const
{
    anObjectPtr->theInternalName = theFactoryName;
    return anObjectPtr;
}

ObjectFactory::ObjectFactoryList *ObjectFactory::getAllFactories(void)
{
    if (theFactoryListPtr == nullptr)
        return nullptr;
    return theFactoryListPtr->getAllFactories();
}
