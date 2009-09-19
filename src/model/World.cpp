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

#include "World.h"
#include "BaseObject.h"
#include "DrawWorld.h"
#include "DrawObject.h"

// Constructors/Destructors
//  

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
	if (theAABBPtr)
	{
		delete theAABBPtr;
		theAABBPtr = NULL;
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
	DEBUG5("addObject(%p = %s)\n", anObjectPtr, anObjectPtr->getName().toAscii().constData());
	if (theObjectPtrList.contains(anObjectPtr))
		return false;
	
	theObjectPtrList.push_back(anObjectPtr);
	anObjectPtr->theWorldPtr = this;
	anObjectPtr->reset();
	
	if (theDrawWorldPtr)
	{
		theDrawWorldPtr->addItem(anObjectPtr->createDrawObject());
	}

	return true;
}

void World::createScene(MainWindow* myMainPtr)
{
	// create a DrawWorld instance, that will immediately attach itself to 
	// the graphicsView in the main window
	assert(theDrawWorldPtr == NULL);
	theDrawWorldPtr = new DrawWorld(myMainPtr, this);
	
#ifdef DRAWDEBUG
	// if DRAWDEBUG is defined, DrawWorld also inherits from the b2DrawDebug class
	// and will cause drawing of all shapes - good for debugging new objects
	// but we have to register the debug thingie first.
	theB2WorldPtr->SetDebugDraw(theDrawWorldPtr);
#endif

	// get all BaseObjects to register themselves in the DrawWorld
	BaseObjectPtrList::iterator i;
	for(i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		DEBUG5("adding item %p\n",*i);
		theDrawWorldPtr->addItem((*i)->createDrawObject());
	}
}

void World::initAttributes( )
{
	theDrawWorldPtr = NULL;
	
	theAABBPtr = new b2AABB();
	theAABBPtr->lowerBound.Set(-5.0f, -50.0f);
	theAABBPtr->upperBound.Set(100.0f, 100.0f);

	// Define the gravity vector.
	b2Vec2 myGravity(0.0f, -9.81f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	theB2WorldPtr = new b2World(*theAABBPtr, myGravity, doSleep);
	BaseObject::ForWorldOnly::setTheB2WorldPtr(theB2WorldPtr);
	theB2WorldPtr->SetContactListener(this);
	
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(50.01f, -1.0f);
	b2Body* groundBody = theB2WorldPtr->CreateBody(&groundBodyDef);
	b2PolygonDef groundShapeDef;
	groundShapeDef.SetAsBox(50.0f, 1.0f);
	groundShapeDef.restitution=0.0f;
	groundBody->CreateShape(&groundShapeDef);

	// Define the left wall body.
	b2BodyDef leftBodyDef;
	leftBodyDef.position.Set(-1.0f, 50.01f);
	b2Body* leftBody = theB2WorldPtr->CreateBody(&leftBodyDef);
	b2PolygonDef leftShapeDef;
	leftShapeDef.SetAsBox(1.0f, 50.0f);
	leftShapeDef.restitution=0.0f;
	leftBody->CreateShape(&leftShapeDef);
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
	DEBUG5("removeObject(%p = %s)\n", anObjectPtr, anObjectPtr->getName().toAscii().constData());
	int myPos = theObjectPtrList.indexOf(anObjectPtr);

	if (myPos == -1)
		return false;
	theObjectPtrList.removeAt(myPos);
	return true;
}

bool World::registerCallback(SimStepCallbackInterface* anInterface)
{
	if (anInterface==NULL)
		return false;
	theCallbackList.insert(anInterface);
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

qreal World::simStep (void)
{
	// clear the contact point lists
	clearLists();

	// run the simulation
	theB2WorldPtr->Step(theDeltaTime,theIterationcount);

	// run all the callbacks per sim step
	CallbackList::iterator i;
	for (i=theCallbackList.begin(); i != theCallbackList.end(); ++i)
	{
		// FIXME: total time not available???
		(*i)->callbackStep(theDeltaTime, 0.0);
	}

	// run all the callbacks for each sensor
	ContactPointList::iterator j = theAddedCPList.begin();
	for (; j!=theAddedCPList.end(); ++j)
	{
		b2ContactPoint* myCPPtr = &(*j);
		b2Shape* myShape1 = myCPPtr->shape1;
		b2Shape* myShape2 = myCPPtr->shape2;

		// only call the sensor when only one of both contacts is a sensor
		if (myShape1->IsSensor() && !myShape2->IsSensor())
		{
			BaseObject* myPtr = reinterpret_cast<BaseObject*>(myShape1->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPPtr);
		}
		if (!myShape1->IsSensor() && myShape2->IsSensor())
		{
			BaseObject* myPtr = reinterpret_cast<BaseObject*>(myShape2->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPPtr);
		}
	}


	ToRemoveList::iterator k;
	for (k=theToBeRemovedList.begin(); k!=theToBeRemovedList.end(); )
	{
		k.value() -= theDeltaTime;
		if (k.value() <= 0.0)
		{
			removeObject(k.key());
			delete k.key();
			k = theToBeRemovedList.erase(k);
		}
		else
		{
			++k;
		}
	}

	return theDeltaTime;
}

bool World::unregisterCallback(SimStepCallbackInterface* anInterface)
{
	if (anInterface==NULL)
		return false;
	theCallbackList.erase(anInterface);
	return true;
}
