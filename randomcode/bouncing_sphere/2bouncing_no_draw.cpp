#include <assert.h>
#include "ode/ode.h"


// dynamics and collision objects
static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup1;

float delta = 0.04;


class Ball
{
public:
	dBodyID body;	
	dGeomID geom;	
	dMass m;

	dReal theBounce;
	
	Ball(dReal radius, dReal aBounce,     dReal x, dReal y, dReal z)
		: theBounce(aBounce)
	{
		body = dBodyCreate (world);
		geom = dCreateSphere (space, radius);
		dMassSetSphere (&m, 1, radius);
		dBodySetMass (body, &m);
		dGeomSetBody (geom, body);
		// set initial position
		dBodySetPosition (body, x, y, z);
		
		dBodySetData(body, this);
	}
};



Ball* ball1;
Ball* ball2;

dReal getBounce(dBodyID b1)
{
	if (b1 == NULL)
		return 1.0;

	Ball* myObject1 = reinterpret_cast<Ball*>(dBodyGetData(b1));
	if (myObject1)
		return  myObject1->theBounce;
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
    contact.surface.soft_cfm = delta*delta/10;  
    int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact));
    if (numc) 
    {
        dJointID c = dJointCreateContact (world,contactgroup1,&contact);
        dJointAttach (c,b1,b2);
    }
}


// simulation loop
static void simLoop (void)
{
    const dReal *pos1;
    const dReal *pos2;
    const dReal *R;
	
	static float mytime=0.0;
	
    // find collisions and add contact joints
    dSpaceCollide (space,0,&nearCallback);
    // step the simulation
	
    dWorldStep (world, delta);  
	mytime += delta;
    // remove all contact joints
    dJointGroupEmpty (contactgroup1);
    
    // redraw sphere at new location
    pos1 = dGeomGetPosition (ball1->geom);
    pos2 = dGeomGetPosition (ball2->geom);
    R = dGeomGetRotation (ball1->geom);
	
	printf("%f \t %f,%f,%f \t %f, %f, %f\n", mytime, 
			pos1[0], pos1[1], pos1[2], 
			pos2[0], pos2[1], pos2[2]);
}


int main (int argc, char **argv)
{
    dInitODE ();
    // create world
    world = dWorldCreate ();
    space = dHashSpaceCreate (0);
    dWorldSetGravity (world,0,0,-0.2);
    dWorldSetCFM (world,1e-5);
    dCreatePlane (space,0,0,1,0);
    contactgroup1 = dJointGroupCreate (0);

    //              radius bouncyness   x,y,z
    ball1 =new Ball(0.5,   0.8,        0,0,3);
    ball2 =new Ball(0.5,   0.8,        0.1,0,5);
    
    // run simulation
    int i;
    for(i=0; i<400;i++)
    {
		simLoop();
    }
    
    
    // clean up
    dJointGroupDestroy (contactgroup1);
    dSpaceDestroy (space);
    dWorldDestroy (world);
    dCloseODE();
    return 0;
}

