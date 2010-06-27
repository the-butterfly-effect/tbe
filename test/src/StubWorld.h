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

#ifndef STUBWORLD_H
#define STUBWORLD_H

#include "Box2D.h"
#include "tbe_global.h"
#include "BaseObject.h"

class DrawWorld;

/**
  * STUB VERSION OF World
  *
  */

class World
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

	/** adds object to the World
	 *  if the BaseObject knows about a DrawObject,
	 *      that one will be added to the DrawWorld as well.
	 *  @param anObjectPtr
	 *  @return true if success - false if object already present
	 */
	bool addObject(BaseObject* anObjectPtr);

	/** find an object with a given ID within world
	 *  @param anID  the ID to find for. Because not all objects have an ID,
	 *               an empty ID will cause a NULL return.
	 *  @returns NULL if not found or a pointer if found.
	 */
	BaseObject* findObjectByID(const QString& anID);

	virtual qreal getG(void) { return -9.81; }

	/** removes a joint from the World
	 *  NOTE: the b2Bodies the joint is linked to should both still exist!
	 *  @param aJointPtr pointer to the joint to remove
	 */
	void removeJoint(b2Joint* aJointPtr)
	{
		if (theB2WorldPtr!=NULL)
			theB2WorldPtr->DestroyJoint(aJointPtr);
	}

	/** removes the BaseObject pointed to from world after all simulations
	  * have run for another aDeltaTime seconds
	  * removal will be done after everything has simulated.
	  * World will take care of everything
	  * @param anObjectPtr
	  * @param aDeltaTime   time (in seconds) the object still has to live
	  */
	void removeMe(BaseObject* anObjectPtr, qreal aDeltaTime);

	/** removes the BaseObject pointed to by anObjectPtr from the World.
	 *  This means that the caller is now responsible for the pointer
	 *   - World doesn't destroy the BaseObject !!!
	 *  It does not remove the DrawObject from the DrawWorld, though!
	 *  @param anObjectPtr pointer to object to be removed
	 *  @return true if success - false if object was not found
	 */
	bool removeObject(BaseObject* anObjectPtr);

	/// keep the scene, set all objects back in original position
	void reset (void);

public:
	void addBaseObjectToDrawWorld(BaseObject* aBOPtr);
	void initAttributes ( ) ;

	typedef QList<BaseObject*> BaseObjectPtrList;
	DrawWorld* theDrawWorldPtr;

	/// the list of all objects managed by this World
	BaseObjectPtrList theObjectPtrList;

	typedef QMap<BaseObject*, qreal> ToRemoveList;
	ToRemoveList theToBeRemovedList;

	b2World* theB2WorldPtr;
	b2AABB*  theAABBPtr;

	/// Do we want to let bodies sleep?
	static const bool doSleep = true;
};

#endif // STUBWORLD_H
