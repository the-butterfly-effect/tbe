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

#ifndef WORLD_H
#define WORLD_H

#include "Box2D.h"
#include "b2Contact.h"
#include "tbe_global.h"

//#include "BaseJoint.h"
#include "Background.h"

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSet>

// Forward Definitions:
class AbstractObject;
class Goal;
class ResizingGraphicsView;
class ViewWorld;

/// struct to keep contact information (as generated by ContactListener) in
struct ContactInfo
{
	ContactInfo(const b2Fixture* a, const b2Fixture* b, float32 c, float32 d)
	  : myFixtureA(a), myFixtureB(b), myNormalImpulse(c), myTangentImpulse(d)
	{;}

	const b2Fixture* myFixtureA;
	const b2Fixture* myFixtureB;
	float32 myNormalImpulse;
	float32 myTangentImpulse;
};

/** this class can be registered to receive all contact points
 *  for each simulation step. That's a lot of calls, usually :-(
 *  also note that the data has very limited usability it is totally
 *  renewed every step.
 */
class ContactListener : public b2ContactListener
{
protected:
	/// implemented from b2ContactListener: add a new contact point,
	/// but only if one or both are sensors...
	/// (because PostSolve() won't see any sensors)
	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* aContact)
	{
		if (aContact->GetFixtureA()->IsSensor() ||
			aContact->GetFixtureB()->IsSensor())
		{
			theContactInfoList.push_back(ContactInfo(
			aContact->GetFixtureA(), aContact->GetFixtureB(),
			0, 0));
		}
	}

	/// implemented from b2ContactListener: contact point was removed
	virtual void EndContact(b2Contact*)
	{ /* no action */ }

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{ theContactInfoList.push_back(ContactInfo(
			contact->GetFixtureA(), contact->GetFixtureB(),
			impulse->normalImpulses[0], impulse->tangentImpulses[0])); }

	/// it doesn't make sense to use any more complex storage than vector
	/// the good news is that vector makes a copy upon insertion
	/// - that's what we need anyway
	typedef std::vector<ContactInfo> ContactInfoList;
	ContactInfoList theContactInfoList;

	/// erase all lists again
	void clearLists(void)
	{
		theContactInfoList.clear();
	}
};


class DestructionListener : public b2DestructionListener
{
public:
	/// implemented from b2DestructionListener
	/// this member is called whenever a Joint is (implicitly) destructed
	/// let's notify the corresponding object in our World
	virtual void SayGoodbye(b2Joint* joint);

	/// not interested...
	virtual void SayGoodbye(b2Fixture*) {};
};



/** if this interface is attached() to World,
  * the World instance will call back every registered
  * interface immediately after each time step.
  */
class SimStepCallbackInterface
{
public:
	/// empty virtual destructor
	virtual ~SimStepCallbackInterface() {;}

	// only usable by World
	friend class World;
private:
	/** if the interface is registered with World,
	  * World will call back every registered interface
	  * immediately after each time step.
	  * @param aTimeStep	time step between previous and new time
	  * @param aTotalTime	the new total time
	  */
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime) = 0;
};


/**
  * class World
  *
  * the class holding all AbstractObjects and is responsible for the simulation
  */

class World : public ContactListener, public DestructionListener, public b2ContactFilter
{
public:

	// Constructors/Destructors
	//

	/**
	 * Simple Constructor
	 */
	World (void);

	/**
	 * Empty Destructor
	 */
	virtual ~World ( );

	/// returns a pointer to the current World
	static World* getWorldPtr();

	/** Adds object to the World.
	 *  The DrawObject associated with the AbstractObject will be created and
	 *  it will be added to the ViewWorld as well.
	 *  @param anObjectPtr
	 *  @return true if success - false if object already present
	 *          (in that case we don't add again to world nor drawworld)
	 */
	bool addObject(AbstractObject* anObjectPtr);

	/** adds an instantiated Goal to the World
	 *  @param aGoalPtr pointer to an instantiated class with Goal interface
	 */
	void addGoal(Goal* aGoalPtr);

	/** creates the corresponding ViewWorld and asks it to
	 *  sets itself up as the GraphicsScene
	 *  it will add all known AbstractObjects to the Scene
	 */
	void createScene(ResizingGraphicsView* myRSGVPtr);

	void createPhysicsWorld(void);
	void deletePhysicsWorld(void);

	/** find an object with a given ID within world
	 *  @param anID  the ID to find for. Because not all objects have an ID,
	 *               an empty ID will cause a NULL return.
	 *  @returns NULL if not found or a pointer if found.
	 */
	AbstractObject* findObjectByID(const QString& anID);

	/** @returns a list with all object IDs present in the World
	  * this list can be empty
	  * and can (but shouldn't) contain multiple entries for the same ID
	  */
	QStringList getAllIDs(void) const;

	/** removes a joint from the World
	 *  NOTE: the b2Bodies the joint is linked to should both still exist!
	 *  @param aJointPtr pointer to the joint to remove
	 */
	void removeJoint(b2Joint* aJointPtr)
	{
		theB2WorldPtr->DestroyJoint(aJointPtr);
	}

	/** removes the AbstractObject pointed to from world after all simulations
	  * have run for another aDeltaTime seconds
	  * removal will be done after everything has simulated.
	  * World will take care of everything
	  * @param anObjectPtr
	  * @param aDeltaTime   time (in seconds) the object still has to live
	  */
	void removeMe(AbstractObject* anObjectPtr, qreal aDeltaTime);

	/** removes the AbstractObject pointed to by anObjectPtr from the World.
	 *  This means that the caller is now responsible for the pointer
	 *   - World doesn't destroy the AbstractObject !!!
	 *  It does not remove the DrawObject from the ViewWorld, though!
	 *  @param anObjectPtr pointer to object to be removed
	 *  @return true if success - false if object was not found
	 */
	bool removeObject(AbstractObject* anObjectPtr);

	/// keep the scene, set all objects back in original position
	void reset (void);

	/// called on death of an object (possibly several times!)
	void signalDeath(void);

	/** Take one step in the simulation loop
	 * @note    each step has fixed time, defined in theDeltaTime.
	 * @returns the time step taken in the simulation step in seconds
	 *          (most likely only a fraction of a second)
	 */
	qreal simStep (void);

	/// returns the gravity constant for this world
	virtual qreal getG(void)
	{ return -9.81; }


public:
	//////////////////////////////////////////////////////////////////////////
	// the call back interface for b2ContactFilter && assorted stuff

	/// implemented from b2ContactFilter
	/// @returns true if fixture1 and fixture2 should collide
	virtual bool ShouldCollide(
			b2Fixture* aFixture1,
			b2Fixture* aFixture2);

	/** add the set of anObject1 and anObject2 to a list of objects that
	  * cannot collide.
	  * @param anObject1	pointer to an object
	  * @param anObject2	pointer to a second object
	  */
	void addNoCollisionCombo(AbstractObject* anObject1, AbstractObject* anObject2);

private:
	typedef QMultiHash<AbstractObject*, AbstractObject*> NoCollisionList;
	NoCollisionList theNoCollisionList;

public:
	//////////////////////////////////////////////////////////////////////////
	// the call back interface for simstep

	/** register a SimStepCallbackInterface
	  * @param anInterface - interface to register for callbacks every time step
	  * @returns true if registering successful, false if not (already registered? NULL?)
	  */
	bool registerCallback(SimStepCallbackInterface* anInterface);

	/** unregister a SimStepCallbackInterface
	  * @param anInterface - interface to register for callbacks every time step
	  * @returns true if unregistering successful, false if not (not registered? NULL?)
	  */
	bool unregisterCallback(SimStepCallbackInterface* anInterface);

private:
	typedef QSet<SimStepCallbackInterface*> CallbackList;
	CallbackList theCallbackList;


	/// the actual (internal) call to add the DrawObject of a AbstractObject
	/// to the corresponding ViewWorld
	void addAbstractObjectToViewWorld(AbstractObject* aBOPtr);

public:
	// Public attribute accessor methods
	//

    /** returns the name of the Level that created the World
     */
    const QString& getName(void) const
        { return theLevelName; }

	qreal getTheWorldWidth(void)
		{ return theWorldWidth; }
	qreal getTheWorldHeight(void)
		{ return theWorldHeight; }


public:
	/// this structure contains everything to draw the background in a scene
	/// i.e. background image and/or gradient
	Background theBackground;

private:
	// Private attributes
	//

	typedef QList<AbstractObject*> AbstractObjectPtrList;

	/// the list of all objects managed by this World
	AbstractObjectPtrList theObjectPtrList;

	typedef QMap<AbstractObject*, qreal> ToRemoveList;
	ToRemoveList theToBeRemovedList;

	typedef QList<Goal*> GoalPtrList;
	GoalPtrList theGoalPtrList;

	/// pointer to the associated VIEW class of World
	ViewWorld* theViewWorldPtr;

	/// The attribute that makes World tick
	b2World* theB2WorldPtr;

	/// Do we want to let bodies sleep?
	static const bool doSleep;

	/// the time taking in each time step
	static const qreal theDeltaTime;

	/// the number of velocity iterations of Box2D per time step
	static const unsigned int theVelocityIterationcount;
	/// the number of position iterations of Box2D per time step
	static const unsigned int thePositionIterationcount;

	QString theLevelName;

	qreal	theWorldWidth;
	qreal	theWorldHeight;

	qreal	theTotalTime;

	friend class Level;

	// FIXME/TODO: get rid of these friends
	// the easiest way is to move this into GoalSerializer...
	friend class GoalEditor;
	friend class EditLevelProperties;
};

#endif // WORLD_H
