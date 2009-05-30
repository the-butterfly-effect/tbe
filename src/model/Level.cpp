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


// Constructors/Destructors
//  

Level::Level ( ) 
{
    theWorldPtr = new World();
    
    BaseObject* myBOPtr;
    
    myBOPtr = ObjectFactory::createObject("Ramp", Position(0.75, 0.5, 0), 1.5);
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Ball", Position(0.3, 2.0, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3.23, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);

    myBOPtr = ObjectFactory::createObject("Bowling Pin", Position(3.46, 0.19, 0));
    theWorldPtr->addObject(myBOPtr);
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
