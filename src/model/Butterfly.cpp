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

#include "Butterfly.h"
//#include "DrawButterfly.h"
#include "DrawObject.h"
#include "tbe_global.h"
#include "Box2D.h"

//// this class' ObjectFactory
class ButterflyObjectFactory : public ObjectFactory
{
public:
	ButterflyObjectFactory(void)
	{	announceObjectType("Butterfly", this); }
	virtual BaseObject* createObject(void) const
	{	return new Butterfly(); }
};
static ButterflyObjectFactory theButterflyObjectFactory;



Butterfly::Butterfly()
		: RectObject()
{
	setProperty(IMAGE_NAME_STRING, "Butterfly");
	setProperty(DESCRIPTION_STRING, QObject::tr("lalala FIXME"));

	// the SVG is 223x339 "pixels"
	setTheWidth(0.15);
	setTheHeight(0.15*339./223.);

	setTheBounciness(0.3);
}

Butterfly::~Butterfly()
{
	;
}


void Butterfly::callBackSensor(b2ContactPoint*)
{
	hasContact = true;
}

void Butterfly::callbackStep (qreal, qreal)
{
	DEBUG6("Butterfly receives callback\n");
}


DrawObject*  Butterfly::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	adjustParameters();
	theDrawObjectPtr = new DrawObject(this, getProperty(IMAGE_NAME_STRING));
//	theDrawObjectPtr = new DrawButterfly(this, getProperty(IMAGE_NAME_STRING));
	return theDrawObjectPtr;
}


void Butterfly::reset(void)
{
	theWorldPtr->registerCallback(this);
	BaseObject::reset();
	hasContact = false;
}
