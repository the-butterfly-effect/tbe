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

#include "BaseObjectSerializer.h"
#include <QDomElement>


// these are all declared in Level.cpp
extern const char* theObjectString;
extern const char* theWidthAttributeString;
extern const char* theHeightAttributeString;
extern const char* theXAttributeString;
extern const char* theYAttributeString;
extern const char* theAngleAttributeString;
extern const char* theTypeAttributeString;


BaseObjectSerializer::BaseObjectSerializer(const BaseObject* anObjectPtr)
		: theBaseObjectPtr(anObjectPtr)
{
}


void
BaseObjectSerializer::serialize(QDomElement* aParent) const
{
	QDomElement myNode = aParent->ownerDocument().createElement(theObjectString);
	myNode.setAttribute(theTypeAttributeString, theBaseObjectPtr->getName());
	myNode.setAttribute(theXAttributeString, theBaseObjectPtr->getOrigCenter().x);
	myNode.setAttribute(theYAttributeString, theBaseObjectPtr->getOrigCenter().y);
	if (theBaseObjectPtr->isRotatable())
		myNode.setAttribute(theAngleAttributeString, theBaseObjectPtr->getOrigCenter().angle);
	if (theBaseObjectPtr->isResizable() & BaseObject::HORIZONTALRESIZE)
		myNode.setAttribute(theWidthAttributeString, theBaseObjectPtr->getTheWidth());
	if (theBaseObjectPtr->isResizable() & BaseObject::VERTICALRESIZE)
		myNode.setAttribute(theHeightAttributeString, theBaseObjectPtr->getTheHeight());
	aParent->appendChild(myNode);
}
