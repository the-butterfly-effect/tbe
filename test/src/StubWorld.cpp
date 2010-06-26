/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "StubWorld.h"

World::World ( void)
{
	initAttributes();
}

World::~World ( )
{
	DEBUG5("World::~World - clear the ObjectPtrList \n");

	while(theObjectPtrList.isEmpty()==false)
	{
		BaseObject* myPtr = theObjectPtrList.first();
		delete myPtr;
		theObjectPtrList.pop_front();
	}

	DEBUG5("World::~World - destroy the rest \n");
	if (theB2WorldPtr)
	{
		delete theB2WorldPtr;
		theB2WorldPtr = NULL;
		BaseObject::ForWorldOnly::setTheB2WorldPtr(theB2WorldPtr);

	}
}

//
// Methods
//


// Accessor methods
//

// Other methods
//

bool World::addObject(BaseObject* anObjectPtr)
{
	if (anObjectPtr == NULL)
		return false;
	DEBUG5("addObject(%p = %s)\n", anObjectPtr, ASCII(anObjectPtr->getName()));
	if (theObjectPtrList.contains(anObjectPtr))
		return false;

	theObjectPtrList.push_back(anObjectPtr);
	anObjectPtr->theWorldPtr = this;
	anObjectPtr->reset();

	addBaseObjectToDrawWorld(anObjectPtr);
	return true;
}

void World::addBaseObjectToDrawWorld(BaseObject* aBOPtr)
{
	if (theDrawWorldPtr)
	{
		DrawObject* myDOPtr = aBOPtr->createDrawObject();
		if (myDOPtr!=NULL)
			theDrawWorldPtr->addItem(myDOPtr);
	}
}


BaseObject* World::findObjectByID(const QString& anID)
{
	if (anID.isEmpty())
		return NULL;

	// iterate through all BaseObjects
	BaseObjectPtrList::iterator i;
	for(i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		if ((*i)->getID() == anID)
			return (*i);
	}

	return NULL;
}


void World::initAttributes( )
{
	theDrawWorldPtr = NULL;
}

void World::removeMe(BaseObject* anObjectPtr, qreal aDeltaTime)
{
	// add to the list of "todo"
	// note that this list is actually a map
	//   - we won't allow double insertions of the same object
	if (theToBeRemovedList.contains(anObjectPtr))
		return;
	theToBeRemovedList.insert(anObjectPtr, aDeltaTime);
}

bool World::removeObject(BaseObject* anObjectPtr)
{
	if (anObjectPtr == NULL)
		return false;
	DEBUG5("removeObject(%p = %s)\n", anObjectPtr, ASCII(anObjectPtr->getName()));
	int myPos = theObjectPtrList.indexOf(anObjectPtr);

	if (myPos == -1)
		return false;
	theObjectPtrList.removeAt(myPos);
	return true;
}

void World::reset ( )
{
	DEBUG5("World::reset()\n");

	BaseObjectPtrList::iterator i=theObjectPtrList.begin();
	while (i!= theObjectPtrList.end())
	{
		if ((*i)->isTemp())
		{
			delete (*i);
			i = theObjectPtrList.erase(i);
		}
		else
		{
			(*i)->reset();
			++i;
		}
	}

	// emptying the theObjectPtrList automatically also
	// took care of everything in the theToBeRemovedList
	theToBeRemovedList.clear();
}
