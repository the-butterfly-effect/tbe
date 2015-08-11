/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef BACKGROUNDSERIALIZER_H
#define BACKGROUNDSERIALIZER_H

#include "Background.h"

// forward declarations:
class QDomElement;
class QDomNode;

#include <QtCore/QList>
#include <QtCore/QString>


/** This Serializer cannot be created.
 *  it has two static members:
 *    - to create a DomNode from a Background instantiation
 *    - to populate a Background from a DomNode.
 */
class BackgroundSerializer
{
public:
	/** create a QDomElement of the Background pointed to
	 *  @param aParent QDomElement pointer to add the serialized data to
	 *  @param theBackgroundPtr   Background to serialize
	 */
	static void serialize(QDomElement* aParent, Background* aBackgroundPtr);

	/** fill up Background from the information in the provided Dom
	 *  @param q  the QDomNode containing the object definition
	 *  @param aBGPtr   OUTPUT (must already exist) filled in struct
	 *  @returns  error message if failed / empty string if successful.
	 */
	static QString createObjectFromDom(const QDomNode& q, Background* aBGPtr);

	/** returns a string representation of the float
	 *  - maximum 2 digits
	 *  - always a dot as decimal separator
	 *  @param aValue the float to convert
	 *  @returns a QString with the string representation of aValue.
	 */
	static QString floatToString(float aValue);

private:
	/// constructor only called by Background
	BackgroundSerializer(const Background* anObjectPtr);
	/// Neutralize copy constructor and assignment operator
	BackgroundSerializer(BackgroundSerializer&);
	/// Neutralize copy constructor and assignment operator
	BackgroundSerializer& operator=(const BackgroundSerializer&);
};

#endif // BACKGROUNDSERIALIZER_H
