#include "BowlingBall.h"
#include "tbe_global.h"

// Constructors/Destructors
//  

BowlingBall::BowlingBall ( ) 
{
	DEBUG5("BowlingBall::BowlingBall\n");
	// set the bowling ball to have a 0.11 meter diameter
	const dReal myRadius = 0.11;
	// and a weight of 6.0 kg.
	setTheGeomID( dCreateSphere (getSpaceID(), myRadius) );
	setMassSphere(6.0, myRadius);
	setTheBounciness(0.2);
	
	setTheWidth(2.0*myRadius);
	setTheHeight(2.0*myRadius);

}

BowlingBall::~BowlingBall ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


