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

#include <QString>
#include <QList>
#include "Box2D.h"
#include "tbe_global.h"

// Forward Definitions:
class BaseObject;
class MainWindow;
class DrawWorld;

/**
  * class World
  *
  * the class holding all BaseObjects and is responsible for the simulation 
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
    

	/** removes the BaseObject pointed to by anObjectPtr from the World.
	 *  This means that the caller is now responsible for the pointer
	 *   - World doesn't destroy the BaseObject !!!
	 *  It does not remove the DrawObject from the DrawWorld, though!
	 *  @param anObjectPtr pointer to object to be removed
	 *  @return true if success - false if object was not found
	 */
	bool removeObject(BaseObject* anObjectPtr);


	/** creates the corresponding DrawWorld and asks it to
	 *  sets itself up as the GraphicsScene
	 *  it will add all known BaseObjects to the Scene
	 */
    void createScene(MainWindow* myMainPtr);
    

	/// keep the scene, set all objects back in original position
	void reset (void);
	

	/** take one step in the simulation loop
	 *  Note that due to a limitation in OpenDE, each step is fixed time 
	 * 
	 * @return the time step taken in the simulation step in seconds 
	 *         (most likely only a fraction of a second)
	 */
	qreal simStep (void);

	
public:
    /** returns the name of the Level that created the World
     */
    const QString& getName(void) const
		{ return theLevelName; } 

    void setLevelName(const QString& aName)
		{ theLevelName = aName; }
    
    // Public attribute accessor methods

	void setTheWorldSize(qreal aWidth, qreal aHeight)
		{ theWorldWidth=aWidth; theWorldHeight=aHeight; }
	qreal getTheWorldWidth(void)
		{ return theWorldWidth; }
	qreal getTheWorldHeight(void)
		{ return theWorldHeight; }

private:
	void initAttributes ( ) ;
	
private:	
	// Private attributes
	//

	typedef QList<BaseObject*> BaseObjectPtrList;
	
	/// the list of all objects managed by this World 
	BaseObjectPtrList theObjectPtrList;
	
	/// pointer to the associated VIEW class of World
	DrawWorld* theDrawWorldPtr;

	// The two attributes that make World tick:
	// 
	
	b2World* theB2WorldPtr;
	b2AABB*  theAABBPtr;
	
	/// Do we want to let bodies sleep?
	static const bool doSleep = true;
	
	/// the time taking in each time step
	static const qreal theDeltaTime = 0.01;
	
	/// the number of iterations of Box2D per time step
	static const unsigned int theIterationcount = 10;
	
	QString theLevelName;

	qreal	theWorldWidth;
	qreal	theWorldHeight;
};

#endif // WORLD_H
