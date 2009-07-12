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
	{	return new Floor(); }
};
static FloorObjectFactory theFactory;

// Constructors/Destructors
//  

Floor::Floor ( ) 
{
	DEBUG5("Floor::Floor\n");
	
	setTheHeight(theSlabThickness);
	setTheBounciness(0.1);
	adjustParameters();
}

Floor::~Floor ( ) { }

//  
// Methods
//  


// Accessor methods
//  

void Floor::setTheWidth ( qreal new_var )
{
	// adjusting the width also implies that the slap changes rotation and size
	if (new_var <= 0.1)
		return;
	BaseObject::setTheWidth(new_var);
	adjustParameters();
}

// Other methods
//  

void Floor::adjustParameters(void)
{
	clearShapeList();

	// note that the angle property is totally ignored here
	// Floors cannot rotate.
	qreal myHW = getTheWidth()/2.0;

	const float theAspect = 10.0;

	// because width/height <= 12.0
	// we better make floor from multiple parts if we need wider floors
	float myNrOfElements = getTheWidth()/(theAspect*theSlabThickness);

	DEBUG5("******* mynumber: %f, w=%f\n", myNrOfElements, myHW);

	qreal myBaseElemWidth = theAspect*theSlabThickness;
	qreal myDoneWidth = 0.0;
	for (float i=0.0; i<myNrOfElements; i+=1.0)
	{
		b2PolygonDef* boxDef = new b2PolygonDef();

		// make sure *not* to have a small last part - i.e. the last two parts
		// average their width.
		float myElemWidth=myBaseElemWidth;
		if (getTheWidth()-myDoneWidth < 2.0*myBaseElemWidth)
		{
			if (getTheWidth()-myDoneWidth > 1.0*myBaseElemWidth)
			{
				myElemWidth = (getTheWidth()-myDoneWidth)/2.0;
			}
			else
			{
				myElemWidth = getTheWidth()-myDoneWidth;
			}
		}
		DEBUG5("elem %f: %fx%f\n", i, myElemWidth, theSlabThickness);
		boxDef->SetAsBox  	( myElemWidth/2.0, theSlabThickness/2.0,
							  b2Vec2(-myHW+myDoneWidth+0.5*myElemWidth, 0.0), 0);
		// floor is immovable -> no mass -> no density
		boxDef->density = 0.0;
		theShapeList.push_back(boxDef);
		myDoneWidth += myElemWidth;
	}

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}
