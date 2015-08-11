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

#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "PostIt.h"
#include "ViewPostIt.h"

// this class' ObjectFactory
class PostItObjectFactory : public ObjectFactory
{
public:
	PostItObjectFactory(void)
	{	announceObjectType("PostIt", this); }
    virtual AbstractObject* createObject(void) const
	{	return fixObject(new PostIt()); }
};
static PostItObjectFactory theFactory;



PostIt::PostIt( )
{
	// Post-Its are 3x3 inch (i.e. 8x8 cm)
	// but because they are too small, we will triple each side and make them 22x22 cm...
	const double mySideLength = 0.22;

	setTheWidth(mySideLength);
	setTheHeight(mySideLength);

	// Note that PostIt doesn't have a physics representation
	// it is only graphics
	theProps.setDefaultPropertiesString(
			QString("-") + Property::IMAGE_NAME_STRING + QString(":/") +
			"-" + Property::MASS_STRING + QString(":/") );
    theToolTip = QObject::tr("Someone left notes all over the place.\n"
                             "You know, those yellow 3x3 inch pieces of paper.\n"
                             "You might want to read them - it may help!");

	DEBUG5("PostIt::PostIt done\n");
}


PostIt::~PostIt( )
{
	;
}



ViewObject*  PostIt::createViewObject(float aDefaultDepth)
{
	assert(theViewObjectPtr==NULL);
    theViewObjectPtr = new ViewPostIt(getThisPtr());
	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	return theViewObjectPtr;
}
