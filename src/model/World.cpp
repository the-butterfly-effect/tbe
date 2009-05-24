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

World::World ( ) 
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
	dJointGroupDestroy (contactgroup1);
    
    dSpaceDestroy (theGlobalSpaceID);
    theGlobalSpaceID = 0;
    BaseObject::ForWorldOnly::setTheSpaceID(theGlobalSpaceID);
    
    dWorldDestroy (theGlobalWorldID);
    theGlobalWorldID = 0;
    BaseObject::ForWorldOnly::setTheWorldID(theGlobalWorldID);

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
	DEBUG5("addObject(%p = %s)\n", anObjectPtr, anObjectPtr->getName().toAscii().constData());
	if (theObjectPtrList.contains(anObjectPtr))
		return false;
	
	theObjectPtrList.push_back(anObjectPtr);
	anObjectPtr->reset();
	
	// if we have a graphical representation already, add object there, too
	if (theDrawWorldPtr)
	{
		// TODO
		assert(false);
	}
	return true;
}

void World::createScene(MainWindow* myMainPtr)
{
	// create a DrawWorld instance, that will immediately attach itself to 
	// the graphicsView in the main window
	assert(theDrawWorldPtr == NULL);
	theDrawWorldPtr = new DrawWorld(myMainPtr, this);
	
	// get all BaseObjects to register themselves in the DrawWorld
	BaseObjectPtrList::iterator i;
	for(i=theObjectPtrList.begin(); i!=theObjectPtrList.end(); ++i)
	{
		DEBUG5("adding item %p\n",*i);
		theDrawWorldPtr->addItem((*i)->createDrawObject());
	}
}

dReal World::getBounce(dBodyID b1)
{
	if (b1 == NULL)
		return 1.0;

	BaseObject* myObject1 = reinterpret_cast<BaseObject*>(dBodyGetData(b1));
	if (myObject1)
		return  myObject1->getTheBounciness();
	return 1.0;
}

void World::initAttributes( )
{
	theDrawWorldPtr = NULL;
	
    theGlobalWorldID = dWorldCreate ();
    BaseObject::ForWorldOnly::setTheWorldID(theGlobalWorldID);
    
    theGlobalSpaceID = dHashSpaceCreate (0);
    BaseObject::ForWorldOnly::setTheSpaceID(theGlobalSpaceID);

    contactgroup1 = dJointGroupCreate (0);

    // gravity is along negative Y axis
    dWorldSetGravity ( theGlobalWorldID,    0.0, -9.81, 0.0);
    dWorldSetCFM (theGlobalWorldID, 1e-5);

    dCreatePlane (theGlobalSpaceID, 0.0, 1.0, 0,0);
    dCreatePlane (theGlobalSpaceID, 1.0, 0.0, 0,0);
}

void World::nearCallbackReal (dGeomID o1, dGeomID o2)
{
	// TODO: the code in this member is still relatively simple
	// compared to the demo code from the opende wiki:
	// http://opende.sourceforge.net/wiki/index.php/Collision_callback_member_function
	
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    dContact contact;  
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    // friction parameter
    contact.surface.mu = dInfinity;

    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0;
    contact.surface.bounce += getBounce(b1);
    contact.surface.bounce += getBounce(b2);
    contact.surface.bounce /= 2;
  	
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.1;
    // constraint force mixing parameter
    // not filled in - use global default //  contact.surface.soft_cfm = 1e-5;  
    int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact));
    if (numc) 
    {
    	DEBUG5("nearCallback\n");
        dJointID c = dJointCreateContact (theGlobalWorldID, contactgroup1, &contact);
        dJointAttach (c,b1,b2);
    }
}

// Remember, this function is static, it has no "this" pointer,
// nor any of the other members of the World class.
void World::nearCallbackStatic (void* theDataPtr, dGeomID o1, dGeomID o2)
{
    // theDataPtr is the "this" that was passed into dSpaceCollide
    World* theWorldPtr = reinterpret_cast<World*>(theDataPtr);
    if (theWorldPtr)
    	theWorldPtr->nearCallbackReal (o1, o2);
}
	
void World::reset ( ) 
{
	DEBUG5("World::reset()\n");
}

dReal World::simStep (void)
{
    // find collisions and add contact joints
    dSpaceCollide (theGlobalSpaceID, this, nearCallbackStatic);
    
    // step the simulation
	dWorldStep (theGlobalWorldID, deltaTime);
	
    // remove all contact joints
    dJointGroupEmpty (contactgroup1);
    
    return deltaTime;
}

