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

#ifndef BASEOBJECTSERIALIZER_H
#define BASEOBJECTSERIALIZER_H

#include "BaseObject.h"

// forward declarations:
class QDomElement;
class QDomNode;

/** This Serializer can only be created by the BaseObject itself
 *  Once created, if you feed it a QDomElement pointer, it will
 *  create a QDomElement representing itself and add it to the
 *  forementioned element.
 *
 *  The second usage of this class is to create the right BaseObject
 *  from the XML DOM node.
 *
 *  INSTANCES OF THIS CLASS SHOULD ONLY EXIST FOR A SHORT WHILE
 *  (if at all)
 */
class BaseObjectSerializer : public ObjectFactory
{
public:
	/** create a QDomElement of the BaseObject this instance
	 *  was created by and add it to aParent
	 *  @param aParent QDomElement pointer to add the serialized data to
	 */
	void serialize(QDomElement* aParent) const;

	~BaseObjectSerializer() {};


	/** create a BaseObject from the information in the provided Dom
	 *  @param q  the QDomNode containing the object definition
	 *  @returns  NULL if failed or a pointer to a valid BaseObject if successful.
	 *            Note that that BaseObject is still on its own - not attached to a World yet.
	 */
	static BaseObject* createObjectFromDom(const QDomNode& q);

private:
	/// constructor only called by BaseObject
	BaseObjectSerializer(const BaseObject* anObjectPtr);

	friend class BaseObject;

	const BaseObject* theBaseObjectPtr;

	/// implementation of ObjectFactory - not needed in BaseObjectSerializer...
	virtual BaseObject* createObject(void) const { return NULL; }

	/// Neutralize copy constructor and assignment operator
	BaseObjectSerializer(BaseObjectSerializer&);
	/// Neutralize copy constructor and assignment operator
	BaseObjectSerializer& operator=(const BaseObjectSerializer&);
};

#endif // BASEOBJECTSERIALIZER_H
