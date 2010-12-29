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
#include "BaseJoint.h"


const bool World::doSleep = true;
const qreal World::theDeltaTime = 0.01;
const unsigned int World::theVelocityIterationcount = 30;
const unsigned int World::thePositionIterationcount = 30;


// Constructors/Destructors
//  

World::World ( void) : theB2World( b2Vec2(0.0f, getG()), doSleep)

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
	BaseObject::ForWorldOnly::setTheB2WorldPtr(NULL);
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


void World::addNoCollisionCombo(BaseObject* anObject1, BaseObject* anObject2)
{
	// always make sure to get the lowest pointer value in #1
	// (this simplifies lookup)
	if (anObject1 > anObject2)
	{
		BaseObject* myTemp = anObject1;
		anObject1 = anObject2;
		anObject2 = myTemp;
	}

	// only insert when not combo is not present yet
	if (theNoCollisionList.contains(anObject1, anObject2)==false)
		theNoCollisionList.insert(anObject1, anObject2);
}



bool World::addObject(BaseObject* anObjectPtr)
{
	if (anObjectPtr == NULL)
		return false;
	DEBUG4("World::addObject(%p = %s)\n", anObjectPtr, ASCII(anObjectPtr->getName()));

	// if it is already present, let's not insert again
	if (theObjectPtrList.contains(anObjectPtr)==false)
		theObjectPtrList.push_back(anObjectPtr);
	anObjectPtr->theWorldPtr = this;
	anObjectPtr->reset();
	
	if (theDrawWorldPtr!=NULL)
		addBaseObjectToDrawWorld(anObjectPtr);
	return true;
}

void World::addBaseObjectToDrawWorld(BaseObject* aBOPtr)
{
	assert(theDrawWorldPtr!=NULL);
	DEBUG4("World::addBaseObjectToDrawWorld(%p)\n", aBOPtr);
	DrawObject* myDOPtr = aBOPtr->createDrawObject();
	if (myDOPtr!=NULL)
		theDrawWorldPtr->addItem(myDOPtr);
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
	theB2World.SetDebugDraw(theDrawWorldPtr);

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

QStringList World::getAllIDs(void) const
{
	QStringList myIDList;

	// iterate through all BaseObjects
	BaseObjectPtrList::const_iterator i;
	for (i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		QString myID = (*i)->getID();
		if (myID.isEmpty()==false)
			myIDList.push_back(myID);
	}
	return myIDList;
}


void World::initAttributes( )
{
	theDrawWorldPtr = NULL;
	theTotalTime = 0.0f;
	
	BaseObject::ForWorldOnly::setTheB2WorldPtr(&theB2World);
	theB2World.SetContactListener(this);
	theB2World.SetDestructionListener(this);
	theB2World.SetContactFilter(this);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(50.01f, -1.0f);
	b2Body* groundBodyPtr = theB2World.CreateBody(&groundBodyDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(50.0f, 1.0f);
	groundBodyPtr->CreateFixture(&groundShape, 0.0f);

	// Define the zero body - only used to make DrawDebug look good :-)
	b2BodyDef myZeroBodyDef;
	myZeroBodyDef.type = b2_staticBody;
	myZeroBodyDef.position.Set(-0.06f, -0.06f);
	b2Body* myZeroBodyPtr = theB2World.CreateBody(&myZeroBodyDef);
	b2PolygonShape myZeroShape;
	myZeroShape.SetAsBox(0.05f, 0.05f);
	myZeroBodyPtr->CreateFixture(&myZeroShape, 0.0f);
	BaseJoint::setGroundBodyPtr(myZeroBodyPtr);


	// Define the left wall body.
	b2BodyDef myLeftBodyDef;
	myLeftBodyDef.type = b2_staticBody;
	myLeftBodyDef.position.Set(-1.0f, 50.01f);
	b2Body* myLeftBodyPtr = theB2World.CreateBody(&myLeftBodyDef);
	b2PolygonShape myLeftShape;
	myLeftShape.SetAsBox(1.0f, 50.0f);
	myLeftBodyPtr->CreateFixture(&myLeftShape, 0.0f);
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
	DEBUG5("World::registerCallback(%p)\n", anInterface);
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
	theTotalTime = 0;

	theB2World.ClearForces();
}


bool World::ShouldCollide(
			b2Shape* shape1,
			b2Shape* shape2)
{
// TODO/FIXME BEFORE CHECKIN
#if 0
	BaseObject* myObj1 = reinterpret_cast<BaseObject*>(shape1->GetUserData());
	BaseObject* myObj2 = reinterpret_cast<BaseObject*>(shape2->GetUserData());

	if (myObj1 == NULL || myObj2 == NULL)
		return true;

	// always make sure to get the lowest pointer value in #1
	// (this simplifies lookup)
	if (myObj1 > myObj2)
	{
		BaseObject* myTemp = myObj1;
		myObj1 = myObj2;
		myObj2 = myTemp;
	}

	if (theNoCollisionList.contains(myObj1, myObj2))
		return false;
#endif
	return true;
}



qreal World::simStep (void)
{
	// clear the contact point lists
	clearLists();

	// run the simulation
	theB2World.Step(theDeltaTime,theVelocityIterationcount, thePositionIterationcount);

	// run all the callbacks for each sensor
	ContactPointList::iterator j = theAddedCPList.begin();
	for (; j!=theAddedCPList.end(); ++j)
	{
		const b2Contact* myCPtr = *j;
		const b2Fixture* myFixtureA = myCPtr->GetFixtureA();
		const b2Fixture* myFixtureB = myCPtr->GetFixtureB();

		// only call the sensor when just *one* of both shapes is a sensor
		if (myFixtureA->IsSensor() && !myFixtureB->IsSensor())
		{
			SensorInterface* myPtr = reinterpret_cast<SensorInterface*>(myFixtureA->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPtr);
		}
		if (!myFixtureA->IsSensor() && myFixtureB->IsSensor())
		{
			SensorInterface* myPtr = reinterpret_cast<SensorInterface*>(myFixtureB->GetUserData());
			if (myPtr!=NULL)
				myPtr->callBackSensor(myCPtr);
		}
	}

	// check all contactresults if either of the objects is interested
	// in hearing the impulses...
// TODO/FIXME before checkin:
#if 0
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
#endif

	// run all the callbacks per sim step
	foreach(SimStepCallbackInterface* i, theCallbackList)
		i->callbackStep(theDeltaTime, theTotalTime);

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

	theTotalTime += theDeltaTime;

	// check if all goals are met
	if (theGoalPtrList.count()==0)
		goto goals_not_met;
	foreach(Goal* l, theGoalPtrList)
	{
		if (l->checkForSuccess()==false)
			goto goals_not_met;
	}
	emit theDrawWorldPtr->on_winning();

goals_not_met:
	return theDeltaTime;
}

void World::signalDeath(void)
{
	emit theDrawWorldPtr->on_death();
}

bool World::unregisterCallback(SimStepCallbackInterface* anInterface)
{
	if (anInterface==NULL)
		return false;
	theCallbackList.remove(anInterface);
	return true;
}
