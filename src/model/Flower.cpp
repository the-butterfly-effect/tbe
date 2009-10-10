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

#include "Flower.h"
#include "tbe_global.h"
#include "Box2D.h"

//// this class' ObjectFactory
class FlowerObjectFactory : public ObjectFactory
{
public:
	FlowerObjectFactory(void)
	{	announceObjectType("Flower", this); }
	virtual BaseObject* createObject(void) const
	{	return new Flower(); }
};
static FlowerObjectFactory theFactory;


typedef QList<Flower*> FlowerPtrList;
static  FlowerPtrList theGlobalFlowerPtrs;



// Constructors/Destructors
//  

Flower::Flower ( )
{
	DEBUG5("Flower::Flower, this=%p\n", this);
	setTheBounciness(0.0);
	adjustParameters();
	theGlobalFlowerPtrs.push_back(this);
}

Flower::~Flower ( )
{
	DEBUG5("Flower::~Flower\n");
	int i = theGlobalFlowerPtrs.indexOf(this);
	theGlobalFlowerPtrs.removeAt(i);
}

//  
// Methods
//  


// Accessor methods
//  

// Other methods
//  


Flower* Flower::getFlowerPtr(void)
{
	DEBUG2("Flower::getFlowerPtr()=%p\n", theGlobalFlowerPtrs.first());
	return theGlobalFlowerPtrs.first();
}
