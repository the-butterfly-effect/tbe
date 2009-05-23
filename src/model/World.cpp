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


// FIXME: hack hack
float delta = 0.01;

dWorldID theGlobalWorldID;
dSpaceID theGlobalSpaceID;
dJointGroupID contactgroup1;


// Constructors/Destructors
//  

World::World ( ) 
{
	initAttributes();
}

World::~World ( ) 
{
	DEBUG5("World::~World\n");
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

void World::nearCallback (void *data, dGeomID o1, dGeomID o2)
{
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

void World::reset ( ) 
{
	DEBUG5("World::reset()\n");
}

dReal World::simStep (void)
{
    // find collisions and add contact joints
    dSpaceCollide (theGlobalSpaceID, 0, &World::nearCallback);
    // step the simulation
	
    dWorldStep (theGlobalWorldID, delta);  
    // remove all contact joints
    dJointGroupEmpty (contactgroup1);
    
    return delta;
}

