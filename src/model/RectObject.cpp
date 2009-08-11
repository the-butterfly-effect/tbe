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

	// also: keep in mind that child objects may set some things automatically
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

	if (getTheWidth()/getTheHeight() > ASPECT_RATIO)
	{
		adjustWideParametersPart();
	}
	else
	{
		if (getTheWidth()/getTheHeight() < 1/ASPECT_RATIO)
		{
			adjustTallParametersPart();
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

void RectObject::adjustTallParametersPart(void)
{
	// we know that height/width > ASPECT_RATIO
	// so use multiple shapes to fix that
	int myNrOfElements = ceil(getTheHeight()/(ASPECT_RATIO*getTheWidth()));
	// also, calculate the density correctly - each shape has same density
	qreal myDensity = getProperty(MASS_STRING).toDouble() / (getTheWidth()*getTheHeight());

	qreal myBaseElemHeight = ASPECT_RATIO*getTheWidth();
	qreal myDoneHeight = 0.0;
	for (int i=0; i<myNrOfElements; i++)
	{
		b2PolygonDef* boxDef = new b2PolygonDef();

		// make sure *not* to have a small last part - i.e. the last two parts
		// average their height.
		float myElemHeight=myBaseElemHeight;
		if (getTheHeight()-myDoneHeight < 2.0*myBaseElemHeight)
		{
			if (getTheHeight()-myDoneHeight > 1.0*myBaseElemHeight)
			{
				myElemHeight = (getTheHeight()-myDoneHeight)/2.0;
			}
			else
			{
				myElemHeight = getTheHeight()-myDoneHeight;
			}
		}
		DEBUG5("elem % 2d: %fx%f\n", i, getTheWidth(), myElemHeight);
		boxDef->SetAsBox  	( getTheWidth()/2.0, myElemHeight/2.0,
							  b2Vec2(0.0, -getTheHeight()/2.0+myDoneHeight+0.5*myElemHeight), 0);
		boxDef->density = myDensity;
		theShapeList.push_back(boxDef);
		myDoneHeight += myElemHeight;
	}
}

void RectObject::adjustWideParametersPart(void)
{
	// we know that width/height > ASPECT_RATIO
	// so use multiple shapes to fix that
	int myNrOfElements = ceil(getTheWidth()/(ASPECT_RATIO*getTheHeight()));
	// also, calculate the density correctly - each shape has same density
	qreal myDensity = getProperty(MASS_STRING).toDouble() / (getTheWidth()*getTheHeight());

	qreal myBaseElemWidth = ASPECT_RATIO*getTheHeight();
	qreal myDoneWidth = 0.0;
	for (int i=0; i<myNrOfElements; i++)
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
//		DEBUG5("elem % 2d: %fx%f\n", i, myElemWidth, getTheHeight());
		boxDef->SetAsBox  	( myElemWidth/2.0, getTheHeight()/2.0,
							  b2Vec2(-getTheWidth()/2.0+myDoneWidth+0.5*myElemWidth, 0.0), 0);
		boxDef->density = myDensity;
		theShapeList.push_back(boxDef);
		myDoneWidth += myElemWidth;
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
