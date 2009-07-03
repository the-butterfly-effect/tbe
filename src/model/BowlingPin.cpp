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

#include "BowlingPin.h"
#include "DrawObject.h"
#include "tbe_global.h"
#include "Box2D.h"

// this class' ObjectFactory
class BowlingPinObjectFactory : public ObjectFactory
{
public:
	BowlingPinObjectFactory(void)
	{	announceObjectType("Bowling Pin", this); }
	virtual BaseObject* createObject(void) const
	{	return new BowlingPin(); }
};
static BowlingPinObjectFactory theFactory;

// Constructors/Destructors
//  

BowlingPin::BowlingPin ( ) 
{
	DEBUG5("BowlingPin::BowlingPin\n");
	// set the bowling pin to be a *box* - not a cylinder
	const qreal myRadius = 0.06; // m
	const qreal myLength = 0.38; // m
	const qreal myMass   = 1.5;  // kg
	
	b2PolygonDef* boxDef = new b2PolygonDef();

	// remember, SetAsBox takes half width and half height
	// and will center around the center - exactly what we want :-)
	boxDef->SetAsBox  	( myRadius, myLength/2.0);

	boxDef->density = myMass/(2*myRadius*myLength);

	// delete any shapes on the body
	// and create a new shape from the above polygon def
	theShapeList.push_back(boxDef);

//	// top (10)
//	b2PolygonDef* partDef = new b2PolygonDef();
//	// TODO: make density across all polygons
//	partDef->density = myMass/(2*myRadius*myLength);
//	partDef->vertexCount = 3;
//	partDef->vertices[0].Set(-00.318, 03.493);
//	partDef->vertices[1].Set(00.318, 03.493);
//	partDef->vertices[2].Set(0, 03.81);
//	theShapeList.push_back(partDef);
//
//	// right-top (9)
//	partDef = new b2PolygonDef();
//	partDef->vertexCount = 3;
//	partDef->vertices[0].Set(2.29, 34.93);
//	partDef->vertices[1].Set(2.29, 25.4);
//	partDef->vertices[2].Set(3.18, 25.4);
////	theShapeList.push_back(partDef);
//
//	// mid-top (8)
//	partDef = new b2PolygonDef();
//	partDef->vertexCount = 4;
//	partDef->vertices[0].Set( 2.29,	33);
//	partDef->vertices[1].Set( 2.29,	34.93);
//	partDef->vertices[2].Set(-2.29, 34.93);
//	partDef->vertices[3].Set(-2.29, 33);
////	theShapeList.push_back(partDef);
//
//
//	// mid-mid (4)
//	partDef = new b2PolygonDef();
//	// TODO: make density across all polygons
//	partDef->density = myMass/(2*myRadius*myLength);
//	partDef->vertexCount = 4;
//	partDef->vertices[0].Set( 0.229, 1.143);
//	partDef->vertices[1].Set( 0.229, 2.54);
//	partDef->vertices[2].Set(-0.229, 2.54);
//	partDef->vertices[3].Set(-0.229, 1.143);
//	theShapeList.push_back(partDef);

	setTheBounciness(0.85);
	
	setTheWidth(2.0*myRadius);
	setTheHeight(myLength);
}


BowlingPin::~BowlingPin ( )
{
}

//  
// Methods
//  

DrawObject*  BowlingPin::createDrawObject(void)
{
	theDrawObjectPtr = new DrawObject(this, "Bowling_Pin");
	return theDrawObjectPtr;
}
