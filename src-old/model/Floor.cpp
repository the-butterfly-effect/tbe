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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "Floor.h"
#include "tbe_global.h"
#include "Box2D.h"

//// this class' ObjectFactory
class FloorObjectFactory : public ObjectFactory
{
public:
	FloorObjectFactory(void)
	{	announceObjectType("Floor", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Floor()); }
};
static FloorObjectFactory theFactory;

// Constructors/Destructors
//  

Floor::Floor ( ) 
{
	DEBUG5("Floor::Floor\n");

	theProps.setDefaultPropertiesString(
			Property::IMAGE_NAME_STRING + QString(":used_wood_bar/") +
			Property::BOUNCINESS_STRING + QString(":0.1/") +
			Property::MASS_STRING + QString(":0.0/") );
	
	setTheHeight(0.1);

	float myFloat;
	theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
	setTheBounciness(myFloat);

	adjustParameters();
}

Floor::~Floor ( ) { }

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  

