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

#include "Position.h"
#include "Property.h"
#include <QStringList>

#include "tbe_global.h"

// these are the identifiers of the properties in the XML file
// do not translate - ever!
const char* Property::OBJECT_NAME_STRING = "ObjectName";
const char* Property::BOUNCINESS_STRING  = "Bounciness";
const char* Property::RESIZABLE_STRING   = "Resizable";
const char* Property::ROTATABLE_STRING   = "Rotatable";
const char* Property::MASS_STRING        = "Mass";
const char* Property::FRICTION_STRING    = "Friction";
const char* Property::IMAGE_NAME_STRING  = "ImageName";
const char* Property::DESCRIPTION_STRING = "Description";

const char* Property::THRUST_STRING      = "Thrust";

const char* Property::NONE_STRING        = "none";
const char* Property::HORIZONTAL_STRING  = "horizontal";
const char* Property::VERTICAL_STRING    = "vertical";
const char* Property::TOTALRESIZE_STRING = "totalresize";


const char* Property::S_LESSTHAN = "lessthan";
const char* Property::S_MORETHAN = "morethan";

const char* Property::S_XCHANGED = "xchanged";
const char* Property::S_XBELOW   = "xbelow";
const char* Property::S_XOVER    = "xover";
const char* Property::S_YCHANGED = "ychanged";
const char* Property::S_YBELOW   = "ybelow";
const char* Property::S_YOVER    = "yover";
const char* Property::S_ACHANGED = "anglechanged";
const char* Property::S_ANYTHING = "anythingchanged";

const char* Property::OBJECT_STRING  = "object";
const char* Property::OBJECT1_STRING = "object1";
const char* Property::OBJECT2_STRING = "object2";


bool PropertyList::propertyToFloat(const QString& aPropertyName,
										 float* aFloat)
{
	QString myValue = getProperty(aPropertyName);
	if (myValue.isEmpty())
		return false;

	bool isOK = false;
	float myFloat = myValue.toFloat(&isOK);
	if (isOK == false)
		return false;

	*aFloat = myFloat;
	return true;
}


bool PropertyList::propertyToPosition(const QString& aPropertyName, Position* aPosition)
{
	QString myValue = getProperty(aPropertyName);
	if (myValue.isEmpty())
		return false;

	bool isOK = false;

	// we have deltaX before, and deltaY after the comma
	QStringList myList = myValue.split(",");
	if (myList.count()!=2)
	{
		DEBUG2("propertyToPosition '%s' does not have a comma?\n", ASCII(myValue));
		return false;
	}
	float dx =  myList.first().toFloat(&isOK);
	if (isOK == false)
		return false;
	float dy =  myList.last().toFloat(&isOK);
	if (isOK == false)
		return false;

	aPosition->x = dx;
	aPosition->y = dy;
	return true;
}
