/* The Butterfly Effect
 * This file copyright (C) 2009, 2011,2014  Klaas van Gend
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
#include "AbstractObject.h"
#include "Goal.h"
#include "resizinggraphicsview.h"
#include "ViewObject.h"
#include "AbstractJoint.h"
#include "Box2D.h"
#include "ViewWorld.h"

const bool World::doSleep = true;
const qreal World::theDeltaTime = 0.01;
const unsigned int World::theVelocityIterationcount = 30;
const unsigned int World::thePositionIterationcount = 30;

static World* theStaticWorldPtr = NULL;

// Constructors/Destructors
//

void DestructionListener::SayGoodbye(b2Joint* joint)
{
	// we *know* that all b2Joints will have UserData but e.g. DetonatorHandle
	// does not - but RTTI can fix that.
    AbstractJoint* myIF = dynamic_cast<AbstractJoint*>(joint->GetUserData());
	if (myIF != NULL)
		myIF->jointWasDeleted();
}


World::World ( void) : theB2WorldPtr(NULL)
{
	theViewWorldPtr = NULL;
	theTotalTime = 0.0f;
	theStaticWorldPtr = this;
}

World::~World ( )
{
    DEBUG5("World::~World");
	AbstractObject::setTheB2WorldPtr(NULL);
	theStaticWorldPtr = NULL;
}

//
// Methods
//


// Accessor methods
//

World* World::getWorldPtr()
{
	return theStaticWorldPtr;
}


// Other methods
//

void World::addGoal(Goal* aGoalPtr)
{
	if (aGoalPtr == NULL)
		return;
	theGoalPtrList.push_back(aGoalPtr);
}


void World::addNoCollisionCombo(AbstractObjectPtr anObjectPtr1, AbstractObjectPtr anObjectPtr2)
{
	// always make sure to get the lowest pointer value in #1
	// (this simplifies lookup)
    if (anObjectPtr1.get() > anObjectPtr2.get())
        std::swap(anObjectPtr1, anObjectPtr2);

	// only insert when not combo is not present yet
    if (!findNoCollisionCombo(anObjectPtr1.get(), anObjectPtr2.get()))
        theNoCollisionList.insert(NoCollisionList::value_type(anObjectPtr1, anObjectPtr2));
}


bool World::addObject(AbstractObjectPtr anObjectPtr)
{
    if (anObjectPtr == nullptr)
        return false;
    DEBUG4("World::addObject(%p = %s)", anObjectPtr.get(), ASCII(anObjectPtr->getName()));

    // if it is already present, let's not insert again
    if (theObjectPtrList.contains(anObjectPtr)==false)
    {
        theObjectPtrList.push_back(anObjectPtr);
    }
    anObjectPtr->theWorldPtr = this;
    anObjectPtr->parseProperties();
    anObjectPtr->registerChildObjects();

    if (theViewWorldPtr!=NULL)
        addAbstractObjectToViewWorld(anObjectPtr);
    return true;
}


void World::addAbstractObjectToViewWorld(AbstractObjectPtr anAOPtr)
{
    assert(theViewWorldPtr!=NULL);
    DEBUG4("World::addAbstractObjectToViewWorld(%p)", anAOPtr.get());
    ViewObject* myVOPtr = anAOPtr->createViewObject();
    if (myVOPtr!=NULL)
    {
        theViewWorldPtr->addItem(myVOPtr);
        anAOPtr->updateViewObject(false);
    }
}


void World::createPhysicsWorld(void)
{
	DEBUG3("World::createPhysicsWorld()");
	if (theB2WorldPtr!=NULL)
		return;

	theB2WorldPtr = new	b2World( b2Vec2(0.0f, getG()), doSleep);
	AbstractObject::setTheB2WorldPtr(theB2WorldPtr);
	theB2WorldPtr->SetContactListener(this);
	theB2WorldPtr->SetDestructionListener(this);
	theB2WorldPtr->SetContactFilter(this);

	// if theDrawDebug is true, we can ask Box2D to ask ViewWorld to draw
	// all shapes - useful for debugging new objects. But we have to register
	// the debug thingie first.
	if (theDrawDebug)
	{
		theB2WorldPtr->SetDebugDraw(theViewWorldPtr);
		const uint32 myDebugFlags = b2DebugDraw::e_shapeBit |
									b2DebugDraw::e_jointBit |
									b2DebugDraw::e_centerOfMassBit;
		theViewWorldPtr->AppendFlags(myDebugFlags);
	}

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(50.01f, -1.0f);
	b2Body* groundBodyPtr = theB2WorldPtr->CreateBody(&groundBodyDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(50.0f, 1.0f);
	groundBodyPtr->CreateFixture(&groundShape, 0.0f);

	// Define the zero body - only used to make DrawDebug look good :-)
	b2BodyDef myZeroBodyDef;
	myZeroBodyDef.type = b2_staticBody;
	myZeroBodyDef.position.Set(-0.06f, -0.06f);
	b2Body* myZeroBodyPtr = theB2WorldPtr->CreateBody(&myZeroBodyDef);
	b2PolygonShape myZeroShape;
	myZeroShape.SetAsBox(0.05f, 0.05f);
	myZeroBodyPtr->CreateFixture(&myZeroShape, 0.0f);
	AbstractJoint::setGroundBodyPtr(myZeroBodyPtr);


	// Define the left wall body.
	b2BodyDef myLeftBodyDef;
	myLeftBodyDef.type = b2_staticBody;
	myLeftBodyDef.position.Set(-1.0f, 50.01f);
	b2Body* myLeftBodyPtr = theB2WorldPtr->CreateBody(&myLeftBodyDef);
	b2PolygonShape myLeftShape;
	myLeftShape.SetAsBox(1.0f, 50.0f);
	myLeftBodyPtr->CreateFixture(&myLeftShape, 0.0f);

    foreach(AbstractObjectPtr i, theObjectPtrList)
	{
		i->createPhysicsObject();
	}
}

void World::createScene(ResizingGraphicsView *myRSGVPtr)
{
	// create a ViewWorld instance, that will immediately attach itself to
	// the graphicsView in the main window
	assert(theViewWorldPtr == NULL);
	theViewWorldPtr = new ViewWorld(myRSGVPtr, this);

	// get all AbstractObjects to register themselves in the ViewWorld
	AbstractObjectPtrList::iterator i;
	for(i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
        DEBUG5("adding item %p\n", (*i).get());
		addAbstractObjectToViewWorld(*i);
	}
}

void World::deletePhysicsWorld()
{
	DEBUG3ENTRY;

	// update our object lists and notify all objects
	// that we're going to delete the physics parts...
	// (note that no actual physics objects are removed
	//   - we'll leave that to the delete b2WorldPtr below)
	AbstractObjectPtrList::iterator i=theObjectPtrList.begin();
	while (i!= theObjectPtrList.end())
	{
		if ((*i)->isTemp())
		{
            i = theObjectPtrList.erase(i);
		}
		else
		{
			(*i)->deletePhysicsObject();
			++i;
		}
	}

	// emptying the theObjectPtrList automatically also
	// took care of everything in the theToBeRemovedList
    theToBeRemovedList.clear();
    theTotalTime = 0;

	// and delete the b2World itself...
	delete theB2WorldPtr;
	theB2WorldPtr = NULL;
	AbstractObject::setTheB2WorldPtr(theB2WorldPtr);

	// redraw all objects in their static position
	updateViewWorld(false);
}


bool World::findNoCollisionCombo(AbstractObject *anObjectPtr1, AbstractObject *anObjectPtr2)
{
    // always make sure to get the lowest pointer value in #1
    // (this simplifies lookup)
    if (anObjectPtr1 > anObjectPtr2)
        std::swap(anObjectPtr1, anObjectPtr2);

    for (auto i : theNoCollisionList)
    {
        if (i.first.get()==anObjectPtr1 && i.second.get()==anObjectPtr2)
            return true;
    }
    return false;
}


AbstractObjectPtr World::findObjectByID(const QString& anID)
{
	if (anID.isEmpty())
        return nullptr;

	// iterate through all AbstractObjects
	AbstractObjectPtrList::iterator i;
    for(auto i : theObjectPtrList)
	{
        if (i->getID() == anID)
            return i;
	}

    return nullptr;
}

QStringList World::getAllIDs(void) const
{
	QStringList myIDList;

	// iterate through all AbstractObjects
	AbstractObjectPtrList::const_iterator i;
	for (i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		QString myID = (*i)->getID();
		if (myID.isEmpty()==false)
			myIDList.push_back(myID);
	}
	return myIDList;
}



void World::removeMe(AbstractObjectPtr anObjectPtr, qreal aDeltaTime)
{
	// add to the list of "todo"
	// note that this list is actually a map
	//   - we won't allow double insertions of the same object
    if (theToBeRemovedList.contains(anObjectPtr))
        return;
    theToBeRemovedList.insert(anObjectPtr, aDeltaTime);
}

bool World::removeObject(AbstractObjectPtr anObjectPtr)
{
	if (anObjectPtr == NULL)
		return false;
    DEBUG5("removeObject(%p = %s)", anObjectPtr.get(), ASCII(anObjectPtr->getName()));
	int myPos = theObjectPtrList.indexOf(anObjectPtr);

	if (myPos == -1)
        return false;
	theObjectPtrList.removeAt(myPos);
	return true;
}

bool World::registerCallback(SimStepCallbackInterface* anInterface)
{
	DEBUG5("World::registerCallback(%p)", anInterface);
	if (anInterface==NULL)
		return false;
	theCallbackList.insert(anInterface);
	return true;
}



bool World::ShouldCollide(
			b2Fixture* aFixture1,
			b2Fixture* aFixture2)
{
    AbstractObject* myObj1 = aFixture1->GetUserData();
    AbstractObject* myObj2 = aFixture2->GetUserData();

	if (myObj1 == NULL || myObj2 == NULL)
		return true;

	// always make sure to get the lowest pointer value in #1
	// (this simplifies lookup)
	if (myObj1 > myObj2)
	{
        AbstractObject* myTemp = myObj1;
		myObj1 = myObj2;
		myObj2 = myTemp;
	}

    if (findNoCollisionCombo(myObj1, myObj2))
		return false;
	return true;
}



qreal World::simStep (void)
{
	// clear the contact point lists
	clearLists();

	// run the simulation
	theB2WorldPtr->Step(theDeltaTime,theVelocityIterationcount, thePositionIterationcount);

	// check all contactresults if either of the objects is interested
	// in hearing the impulses...
	foreach(ContactInfo k, theContactInfoList)
	{
        AbstractObject* myBO1Ptr = k.myFixtureA->GetUserData();
        AbstractObject* myBO2Ptr = k.myFixtureB->GetUserData();

		if (myBO1Ptr!=NULL)
		{
			if (k.myFixtureA->IsSensor())
				myBO1Ptr->callBackSensor(k);
			if (myBO1Ptr->isInterestedInNormalImpulse())
				myBO1Ptr->reportNormalImpulseLength( k.myNormalImpulse );
			if (myBO1Ptr->isInterestedInTangentImpulse())
				myBO1Ptr->reportTangentImpulse( k.myTangentImpulse );
		}

		if (myBO2Ptr!=NULL)
		{
			if (k.myFixtureB->IsSensor())
				myBO2Ptr->callBackSensor(k);
			if (myBO2Ptr->isInterestedInNormalImpulse())
				myBO2Ptr->reportNormalImpulseLength( k.myNormalImpulse );
			if (myBO2Ptr->isInterestedInTangentImpulse())
				myBO2Ptr->reportTangentImpulse( k.myTangentImpulse );
		}
	}

	// run all the callbacks per sim step
	foreach(SimStepCallbackInterface* i, theCallbackList)
		i->callbackStep(theDeltaTime, theTotalTime);

	// remove all scheduled AbstractObjects from the World
	ToRemoveList::iterator k;
    for (k=theToBeRemovedList.begin(); k!=theToBeRemovedList.end(); )
	{
		k.value() -= theDeltaTime;
		if (k.value() <= 0.0)
		{
			removeObject(k.key());
			k = theToBeRemovedList.erase(k);
		}
		else
		{
			++k;
		}
	}

	theTotalTime += theDeltaTime;

	// check if all goals are met
	bool areGoalsMet = true;
	foreach(Goal* l, theGoalPtrList)
	{
		if (l->isFail==true)
		{
			// check if fail is met
			// one fail is enough to abort
			if (l->checkForSuccess()==true)
				emit signalDeath();
		}
		else
		{
			// check if goal is met
			// if one goal is not met, there's no win
			if (l->checkForSuccess()==false)
				areGoalsMet = false;
		}
	}
	if (areGoalsMet == true)
		emit signalWon();

	// make Box2D draw debug images if requested
	if (theDrawDebug)
		theB2WorldPtr->DrawDebugData();

	return theDeltaTime;
}

bool World::unregisterCallback(SimStepCallbackInterface* anInterface)
{
	if (anInterface==NULL)
		return false;
	theCallbackList.remove(anInterface);
	return true;
}


void World::updateViewWorld(bool isSimRunning)
{
    foreach(AbstractObjectPtr i, theObjectPtrList)
	{
		i->updateViewObject(isSimRunning);
	}
}
