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
	setTheGeomID( dCreateBox (getSpaceID(), 2*myRadius, myLength, 1.0) );
	setMassBox(myMass, 2*myRadius, myLength);
	
	setTheBounciness(1.1);
	
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
{ return new DrawObject(this, "Bowling_Pin"); }
