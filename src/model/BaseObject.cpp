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

#include "BaseObject.h"
#include "DrawObject.h"

// Static variables
static dWorldID theStaticWorldID = NULL;
static dSpaceID theStaticSpaceID = NULL;



// Constructors/Destructors
//  

BaseObject::BaseObject ( ) 
{
	initAttributes();
}

BaseObject::~BaseObject ( ) 
{
	DEBUG5("~BaseObject() for %p\n", this);
	
	// destroy the Geometry
	dGeomSetData(theGeomID, NULL);
	dGeomDestroy(theGeomID);
	theGeomID = NULL;
	
	// destroy the Body
	dBodyDestroy(theBodyID);
	theBodyID = NULL;
	
	// mass will be destroyed in MovingObject
	
	// any joints will be destroyed in the implementation class
}

//  
// Methods
//  


// Accessor methods
//  

dSpaceID BaseObject::getSpaceID(void)
{
	assert (theStaticSpaceID);
	return theStaticSpaceID;
}

dWorldID BaseObject::getWorldID(void)
{
	assert (theStaticWorldID);
	return theStaticWorldID;
}

void BaseObject::ForWorldOnly::setTheSpaceID(dSpaceID anID)
{
	theStaticSpaceID = anID;
}

void BaseObject::ForWorldOnly::setTheWorldID(dWorldID anID)
{
	theStaticWorldID = anID;
}


// Other methods
//  

DrawObject*  BaseObject::createDrawObject(void)
{ return new DrawObject(this); }


void BaseObject::initAttributes ( ) 
{
	DEBUG5("BaseObject::initAttributes\n");
	theBodyID = dBodyCreate (getWorldID());
	dBodySetData(theBodyID, this);
	
	theGeomID = 0;

	theWidth = 1.0;
	theHeight = 1.0;
	theBounciness = 1.0;
	// don't need to initialise theCenter - it has a default constructor
	
	theDrawObjectPtr = NULL;
}

void BaseObject::reset ( ) 
{
	DEBUG5("BaseObject::reset() body pos for '%s' to (%f,%f)@%f\n", 
			getName().toAscii().constData(), theCenter.x, theCenter.y, theCenter.angle);
	dBodySetPosition(theBodyID, theCenter.x, theCenter.y, 0.0);
	dMatrix3 R;
	dRFromAxisAndAngle (R, 0.0, 0.0, 1.0, theCenter.angle);
	dBodySetRotation(theBodyID, R);
}
