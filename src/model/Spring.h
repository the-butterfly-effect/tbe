/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#ifndef SPRING_H
#define SPRING_H

#include "PolyObject.h"
#include "RectObject.h"
#include "World.h"

// forward declarations
class SpringEnd;
class TranslationGuide;

/** this class implements a Spring: two ends that when pushed together,
  * will push back with a force linear to the amount of 'compression'.
  *
  * The two ends are modelled separately, with a prismatic joint between them.
  * This is the main class, that contains two ends. As such, this class
  * doesn't have a Box2D object associated to it.
  *
  * Note that we also have to trick DrawObject because we want a single image
  * to span both ends and resize when the object resizes.
  */
class Spring : public PolyObject
{
public:
	Spring();

	virtual ~Spring();

	/// resets the object into the start position/situation
	/// overridden from RectObject
	virtual void reset(void);

	DrawObject* createDrawObject(void);


	/// overridden from RectObject to account for daughter objects
	virtual void createPhysicsObject(Position aPosition);

	/// overridden from RectObject to account for daughter objects
	virtual void deletePhysicsObject(void);

	virtual Position getTempCenter ( ) const;

	/// overridden from BaseObject to account for daughter objects
	qreal getTheWidth ( ) const;

	/// overridden from RectObject to account for daughter objects
	virtual bool isPhysicsObjectCreated(void) const
		{ return false; }

private:
	void createEnds(void);

	SpringEnd* the1stEnd;
	SpringEnd* the2ndEnd;
	TranslationGuide* theJoint;
};

/// SpringEnd is used by the Spring class
class SpringEnd : public RectObject
{
public:
	SpringEnd();

	virtual ~SpringEnd();

	/// overridden from the base - this class doesn't have a drawobject
//	virtual DrawObject* createDrawObject() {return NULL;};
};


#endif // SPRING_H
