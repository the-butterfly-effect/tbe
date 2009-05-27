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

#include "Level.h"
#include "World.h"
#include "BaseObject.h"
#include "BowlingBall.h"
#include "BowlingPin.h"
#include "Ramp.h"


// Constructors/Destructors
//  

Level::Level ( ) 
{
    theWorldPtr = new World();
    
    // local scope is enough - World will take over ownership
    Ramp* myRampPtr = new Ramp();
    myRampPtr->setTheCenter( Position(0.75, 0.75, 0) );
    myRampPtr->setTheWidth(1.5);
    theWorldPtr->addObject(myRampPtr);

    // local scope is enough - World will take over ownership
    BowlingBall* myBallPtr = new BowlingBall();
    myBallPtr->setTheCenter( Position(2.85, 1.0, 0) );
    theWorldPtr->addObject(myBallPtr);

    // local scope is enough - World will take over ownership
    BowlingBall* myBallPtr2 = new BowlingBall();
    myBallPtr2->setTheCenter( Position(3.15, 1.0, 0) );
    theWorldPtr->addObject(myBallPtr2);

    // local scope is enough - World will take over ownership
    BowlingBall* myBallPtr3 = new BowlingBall();
    myBallPtr3->setTheCenter( Position(0.3, 2.0, 0) );
    theWorldPtr->addObject(myBallPtr3);

    // local scope is enough - World will take over ownership
    BowlingPin* myPinPtr = new BowlingPin();
    myPinPtr->setTheCenter( Position(3, 0.20, 0) );
    theWorldPtr->addObject(myPinPtr);

}

Level::~Level ( ) 
{
	DEBUG5("World::~World - clear the ObjectPtrList \n");
    delete theWorldPtr;
    theWorldPtr = NULL;
}

//  
// Methods
//  
