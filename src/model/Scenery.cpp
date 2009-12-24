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

#include "Scenery.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawObject.h"
#include "Property.h"

// this class' ObjectFactory
class SceneryObjectFactory : public ObjectFactory
{
public:
	SceneryObjectFactory(void)
	{	announceObjectType("Scenery", this); }
	virtual BaseObject* createObject(void) const
	{	return new Scenery(); }
};
static SceneryObjectFactory theFactory;



Scenery::Scenery( ) : RectObject()
{
	// Note that Scenery doesn't have a physics representation
	// it is only graphics

	deletePhysicsObject();
	DEBUG5("Scenery::Scenery done\n");
}


Scenery::~Scenery( )
{
	;
}



DrawObject*  Scenery::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	theDrawObjectPtr = new DrawObject(this, getProperty(Property::IMAGE_NAME_STRING));
	// set the ZValue below 2 (the default for DrawObjects)
	// because we want Scenery to draw behind other objects
	theDrawObjectPtr->setZValue(0.1);
	return theDrawObjectPtr;
}
