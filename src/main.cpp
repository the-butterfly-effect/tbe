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


#include "MainWindow.h"

#include "ode/ode.h"

#include <QtGui>

#include "BowlingBall.h"
#include "BowlingPin.h"


// the verbosity for all logging - by default defined at 5 (most logging)
int theVerbosity = 5;


// FIXME: hack hack hack
// externed in BaseObject.h
dWorldID theGlobalWorldID;
dSpaceID theGlobalSpaceID;

static dJointGroupID contactgroup1;


float delta = 0.01;

dReal getBounce(dBodyID b1)
{
	if (b1 == NULL)
		return 1.0;

	BaseObject* myObject1 = reinterpret_cast<BaseObject*>(dBodyGetData(b1));
	if (myObject1)
		return  myObject1->getTheBounciness();
	return 1.0;
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
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


// simulation loop
static void simLoop (void)
{
    // find collisions and add contact joints
    dSpaceCollide (theGlobalSpaceID, 0, &nearCallback);
    // step the simulation
	
    dWorldStep (theGlobalWorldID, delta);  
    // remove all contact joints
    dJointGroupEmpty (contactgroup1);
}


int main(int argc, char **argv)
{
	// init Qt (graphics toolkit) - www.qtsoftware.com
	QApplication app(argc, argv);
	// init OpenDE (physics library) - opende.sf.net
    dInitODE ();

    MainWindow myMain;
    myMain.show();
    
    // FIXME: hack hack hack
    // should move to World class at some point?
    theGlobalWorldID = dWorldCreate ();
    theGlobalSpaceID = dHashSpaceCreate (0);
    // gravity is along negative Y axis
    dWorldSetGravity ( theGlobalWorldID,    0.0, -9.81, 0.0);
    dWorldSetCFM (theGlobalWorldID, 1e-5);
    
    BowlingBall myBall;
    myBall.setTheCenter( Position(0.5, 2, 0) );
    myBall.reset();
    
    BowlingPin myPin;
    myPin.setTheCenter( Position(1, 0.20, 0) );
    myPin.reset();
    
    
    ///////////////////////////////////
    dCreatePlane (theGlobalSpaceID, 0.0, 1.0, 0,0);
    dCreatePlane (theGlobalSpaceID, 1.0, 0.0, 0,0);
    contactgroup1 = dJointGroupCreate (0);
    // run simulation
    int i;
    for(i=0; i<200;i++)
    {
    	static float mytime=0.0;
    	
		simLoop();
		mytime += delta;

		// redraw sphere at new location
	    const dReal *pos1 = dGeomGetPosition (myBall.getTheGeomID());
	    const dReal *pos2 = dGeomGetPosition (myPin.getTheGeomID());
		
		printf("%f \t %f\t%f\t%f \t %f\t%f\t%f\n", mytime, 
				pos1[0], pos1[1], pos1[2],
				pos2[0], pos2[1], pos2[2]);
    }
    
    
    // clean up
    dJointGroupDestroy (contactgroup1);
    dSpaceDestroy (theGlobalSpaceID);
    dWorldDestroy (theGlobalWorldID);
    dCloseODE();
    return 0;
    ///////////////////////////////////
//    return app.exec();
}
