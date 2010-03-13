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
#include "Goal.h"
#include "DrawWorld.h"
#include "DrawObject.h"
#include "PivotPoint.h"

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

void World::addGoal(Goal* aGoalPtr)
{
	if (aGoalPtr == NULL)
		return;
	theGoalPtrList.push_back(aGoalPtr);
}

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
		{
			theDrawWorldPtr->addItem(myDOPtr);
			myDOPtr->setupCache();
		}
	}
}

void World::createScene(MainWindow* myMainPtr)
{
	// create a DrawWorld instance, that will immediately attach itself to 
	// the graphicsView in the main window
	assert(theDrawWorldPtr == NULL);
	theDrawWorldPtr = new DrawWorld(myMainPtr, this);
	
	// if theDrawDebug is true, Box2D will ask DrawWorld to draw
	// all shapes - useful for debugging new objects
	// but we have to register the debug thingie first.
	if (theDrawDebug==true)
		theB2WorldPtr->SetDebugDraw(theDrawWorldPtr);

	// get all BaseObjects to register themselves in the DrawWorld
	BaseObjectPtrList::iterator i;
	for(i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		DEBUG5("adding item %p\n",*i);
		addBaseObjectToDrawWorld(*i);
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
	
	theAABBPtr = new b2AABB();
	theAABBPtr->lowerBound.Set(-5.0f, -50.0f);
	theAABBPtr->upperBound.Set(100.0f, 100.0f);

	// Define the gravity vector.
	b2Vec2 myGravity(0.0f, -9.81f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	theB2WorldPtr = new b2World(*theAABBPtr, myGravity, doSleep);
	BaseObject::ForWorldOnly::setTheB2WorldPtr(theB2WorldPtr);
	theB2WorldPtr->SetContactListener(this);
	theB2WorldPtr->SetDestructionListener(this);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(50.01f, -1.0f);
	b2Body* groundBody = theB2WorldPtr->CreateBody(&groundBodyDef);
	b2PolygonDef groundShapeDef;
	groundShapeDef.SetAsBox(50.0f, 1.0f);
	groundShapeDef.restitution=0.0f;
	groundBody->CreateShape(&groundShapeDef);

	// Define the zero body - only used to make DrawDebug look good :-)
	b2BodyDef zeroBodyDef;
	zeroBodyDef.position.Set(-0.06f, -0.06f);
	b2Body* zeroBody = theB2WorldPtr->CreateBody(&zeroBodyDef);
	b2PolygonDef zeroShapeDef;
	zeroShapeDef.SetAsBox(0.05f, 0.05f);
	zeroShapeDef.restitution=0.0f;
	zeroBody->CreateShape(&zeroShapeDef);
	PivotPoint::setGroundBodyPtr(zeroBody);


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
	DEBUG5("removeObject(%p = %s)\n", anObjectPtr, ASCII(anObjectPtr->getName()));
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

	// run all the callbacks for each sensor
	ContactPointList::iterator j = theAddedCPList.begin();
	for (; j!=theAddedCPList.end(); ++j)
	{
		b2ContactPoint* myCPPtr = &(*j);
		b2Shape* myShape1 = myCPPtr->shape1;
		b2Shape* myShape2 = myCPPtr->shape2;

		// only call the sensor when just *one* of both shapes is a sensor
		if (myShape1->IsSensor() && !myShape2->IsSensor())
		{
			SensorInterface* myPtr = reinterpret_cast<SensorInterface*>(myShape1->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPPtr);
		}
		if (!myShape1->IsSensor() && myShape2->IsSensor())
		{
			SensorInterface* myPtr = reinterpret_cast<SensorInterface*>(myShape2->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPPtr);
		}
	}

	// check all contactresults if either of the objects is interested
	// in hearing the impulses...
	for (ContactResultList::iterator k = theContactResultList.begin();
			 k!=theContactResultList.end(); ++k)
	{
		b2ContactResult* myResPtr = &(*k);
		BaseObject* myBO1Ptr = reinterpret_cast<BaseObject*>(myResPtr->shape1->GetUserData());
		BaseObject* myBO2Ptr = reinterpret_cast<BaseObject*>(myResPtr->shape2->GetUserData());

		if (myBO1Ptr!=NULL)
		{
			if (myBO1Ptr->isInterestedInNormalImpulse())
				myBO1Ptr->reportNormalImpulseLength( myResPtr->normalImpulse );
			if (myBO1Ptr->isInterestedInTangentImpulse())
				myBO1Ptr->reportTangentImpulse( myResPtr->tangentImpulse );
		}

		if (myBO2Ptr!=NULL)
		{
			if (myBO2Ptr->isInterestedInNormalImpulse())
				myBO2Ptr->reportNormalImpulseLength( myResPtr->normalImpulse );
			if (myBO2Ptr->isInterestedInTangentImpulse())
				myBO2Ptr->reportTangentImpulse( myResPtr->tangentImpulse );
		}
	}


	// run all the callbacks per sim step
	CallbackList::iterator i;
	for (i=theCallbackList.begin(); i != theCallbackList.end(); ++i)
	{
		// FIXME: total time not available???
		(*i)->callbackStep(theDeltaTime, 0.0);
	}

	// remove all scheduled BaseObjects from the World
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

	// check if all goals are met
	GoalPtrList::iterator l;
	if (theGoalPtrList.count()==0)
		goto goals_not_met;
	for (l=theGoalPtrList.begin(); l!=theGoalPtrList.end(); ++l)
	{
		if ((*l)->checkForSuccess()==false)
			goto goals_not_met;
	}
	emit theDrawWorldPtr->on_winning();

goals_not_met:
	return theDeltaTime;
}

bool World::unregisterCallback(SimStepCallbackInterface* anInterface)
{
	if (anInterface==NULL)
		return false;
	theCallbackList.erase(anInterface);
	return true;
}
