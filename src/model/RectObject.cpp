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

#include "RectObject.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawObject.h"


static const char* OBJECT_NAME_STRING  = "ObjectName";
static const char* BOUNCINESS_STRING  = "Bounciness";
static const char* RESIZABLE_STRING   = "Resizable";
static const char* MASS_STRING        = "Mass";
static const char* IMAGE_NAME_STRING   = "ImageName";
static const char* DESCRIPTION_STRING = "Description";

static const char* DEFAULT_RECTOBJECT_NAME = "RectObject";

//// this class' ObjectFactory
class RectObjectFactory : public ObjectFactory
{
public:
	RectObjectFactory(void)
	{	announceObjectType(DEFAULT_RECTOBJECT_NAME, this); }
	virtual BaseObject* createObject(void) const
	{	return new RectObject(); }
};
static RectObjectFactory theRectObjectFactory;


// Constructors/Destructors
//

RectObject::RectObject ( ) : BaseObject(), theNameString(DEFAULT_RECTOBJECT_NAME)
{
	DEBUG5("RectObject::RectObject\n");

	// because this object is very flexible and many parameters can be set through
	// the Properties, do not assume too much here...
}

RectObject::~RectObject ( ) { }

//
// Methods
//


// Accessor methods
//

void RectObject::setTheWidth ( qreal new_var )
{
	BaseObject::setTheWidth(new_var);
	DEBUG5("RectObject::setTheWidth (%f)\n", getTheWidth());
	adjustParameters();
}

void RectObject::setTheHeight ( qreal new_var )
{
	BaseObject::setTheHeight(new_var);
	DEBUG5("RectObject::setTheHeight (%f)\n", getTheHeight());
	adjustParameters();
}


// Other methods
//

void RectObject::adjustParameters(void)
{

	clearShapeList();

	if (getTheWidth()/getTheHeight() > 10.0)
	{
		// FIXME/TODO: implement
//		adjust_wide_Parameters();
	}
	else
	{
		if (getTheWidth()/getTheHeight() < 1/10.0)
		{
			// FIXME/TODO: implement
//			adjust_tall_Parameters();
		}
		else
		{
			DEBUG5("RectObject::adjustParameters wxh=%fx%f\n", getTheWidth(),getTheHeight());
			b2PolygonDef* boxDef = new b2PolygonDef();
			boxDef->SetAsBox(getTheWidth()/2.0, getTheHeight()/2.0);

			// get mass:  no mass -> no density -> no motion
			qreal myMass = getProperty(MASS_STRING).toDouble();
			boxDef->density = myMass / getTheWidth()*getTheHeight();
			theShapeList.push_back(boxDef);
		}
	}

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}

DrawObject*  RectObject::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawObject(this, getProperty(IMAGE_NAME_STRING));
	return theDrawObjectPtr;
}

void  RectObject::setProperty(const QString& aKey, const QString& aValue)
{
	// apart from actually setting the property, we also must
	// check if this is a known property that should take effect immediately

	BaseObject::setProperty(aKey,aValue);

	if (aKey == OBJECT_NAME_STRING)
		theNameString = aValue;
	if (aKey == BOUNCINESS_STRING)
		setTheBounciness(aValue.toDouble());
	if (aKey == RESIZABLE_STRING)
	{
		// FIXME: TODO:  IMPLEMENT THIS PART
		// and run adjust
		adjustParameters();
	};
	if (aKey == MASS_STRING)
		adjustParameters();
	// No need to do image name
	if (aKey == DESCRIPTION_STRING)
		theToolTipString = aValue;
}
