/* The Butterfly Effect
 * This file copyright (C) 2009,2011  Klaas van Gend
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

#ifndef ABSTRACTOBJECTSERIALIZER_H
#define ABSTRACTOBJECTSERIALIZER_H

#include "AbstractObject.h"
#include "ObjectFactory.h"

// forward declarations:
class QDomElement;
class QDomNode;

/** This Serializer can only be created by the AbstractObject itself
 *  Once created, if you feed it a QDomElement pointer, it will
 *  create a QDomElement representing itself and add it to the
 *  forementioned element.
 *
 *  The second usage of this class is to create the right AbstractObject
 *  from the XML DOM node.
 *
 *  INSTANCES OF THIS CLASS SHOULD ONLY EXIST FOR A SHORT WHILE
 *  (if at all)
 */
class AbstractObjectSerializer : public ObjectFactory
{
public:
	/** create a QDomElement of the AbstractObject this instance
	 *  was created by and add it to aParent
	 *  @param aParent QDomElement pointer to add the serialized data to
	 */
	void serialize(QDomElement* aParent) const;

	virtual ~AbstractObjectSerializer() {};


	/** create a AbstractObject from the information in the provided Dom
	 *  @param q  the QDomNode containing the object definition
	 *  @param isMovable  set to true if object is not to be moved by the user
	 *  @param isXYMandatory  set to true if X="" and Y="" are mandatory for the object
	 *  @returns  NULL if failed or a pointer to a valid AbstractObject if successful.
	 *            Note that that AbstractObject is still on its own - not attached to a World yet.
	 */
    static AbstractObjectPtr createObjectFromDom(const QDomNode& q,
											   bool isMovable,
											   bool isXYMandatory);

	/** returns a string representation of the float
	 *  - maximum 3 digits
	 *  - always a dot as decimal separator
	 *  @param aValue the float to convert
	 *  @returns a QString with the string representation of aValue.
	 */
	QString floatToString(float aValue) const;

private:
	/// constructor only called by AbstractObject
    AbstractObjectSerializer(const AbstractObjectPtr anObjectPtr);

	friend class AbstractObject;

    const AbstractObjectPtr theAbstractObjectPtr;

	/// implementation of ObjectFactory - not needed in AbstractObjectSerializer...
    virtual AbstractObject* createObject(void) const { return NULL; }

	/// Neutralize copy constructor and assignment operator
	AbstractObjectSerializer(AbstractObjectSerializer&);
	/// Neutralize copy constructor and assignment operator
	AbstractObjectSerializer& operator=(const AbstractObjectSerializer&);
};

#endif // ABSTRACTOBJECTSERIALIZER_H
