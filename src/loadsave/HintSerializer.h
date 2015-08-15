/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#ifndef HINTSERIALIZER_H
#define HINTSERIALIZER_H

// forward declarations:
class Hint;
class World;
class QDomElement;
class QDomNode;

/** This Serializer is used for Hints
 *  Once created, if you feed it a QDomElement pointer, it will
 *  create a QDomElement representing itself and add it to the
 *  forementioned element.
 *
 *  The second usage of this class is to create the right Hints
 *  from the XML DOM node.
 *
 *  INSTANCES OF THIS CLASS SHOULD NEVER EXIST
 */
class HintSerializer
{
public:
	/// always have a virtual destructor
	virtual ~HintSerializer() {};


	/** create a Hint from the information in the provided Dom
	 *  @param q  the QDomNode containing the Hint definition
	 *  @returns  NULL if failed or a pointer to a valid Hint if successful.
	 *            Note that that Hint is still on its own - not attached to a Level yet.
	 */
	static Hint* createObjectFromDom(const QDomNode& q);

private:
	/// constructor only called by Hint
	// (not implemented yet)
	//HintSerializer(const Hint* anObjectPtr);

	/// Neutralize copy constructor and assignment operator
	HintSerializer(HintSerializer&) = delete;
	/// Neutralize copy constructor and assignment operator
	HintSerializer& operator=(const HintSerializer&) = delete;
};

#endif // HINTSERIALIZER_H
