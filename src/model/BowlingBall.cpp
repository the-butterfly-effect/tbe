#include "BowlingBall.h"
#include "tbe_global.h"

// Constructors/Destructors
//  

BowlingBall::BowlingBall ( ) 
{
	DEBUG5("BowlingBall::BowlingBall\n");
	// set the bowling ball to have a 0.11 meter diameter
	// and a weight of 6.0 kg.
	setTheGeomID( dCreateSphere (theGlobalSpaceID, 0.11) );
	setMassSphere(6.0, 0.11);
}

BowlingBall::~BowlingBall ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


