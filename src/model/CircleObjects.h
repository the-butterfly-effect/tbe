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

#ifndef CIRCLEOBJECTS_H
#define CIRCLEOBJECTS_H

#include "AbstractObject.h"

/**
  * class CircleObject
  *
  * The CircleObject class essentially models anything resembling a circle.
  * It is used with a specialised factory to build the various types of balls
  */

class CircleObject : public AbstractObject
{
public:

	// Constructors/Destructors
	//

	/// Constructor
	CircleObject (const QString& aName,
				  const QString& aTooltip,
				  const QString& anImageName,
				  qreal aRadius,
				  qreal aMass,
				  qreal aBounciness);

	/// virtual destructor
	virtual ~CircleObject ( );

	// Public attribute accessor methods
	//

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return theBallName;	}

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if its mass is larger than 0.001 kg
	virtual b2BodyType getObjectType(void) const
	{	return theB2ObjectType; }

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

protected:
	/// internally used by CircleObject and CustomBall to
	/// actually create the b2Shape and b2FixtureDef
	virtual void createBallShapeFixture(float aRadius, float aMass);

private:
	QString theBallName;

	b2BodyType theB2ObjectType;
};


class CustomBall : public CircleObject
{
public:
	/// Constructor
	CustomBall ();

	/// virtual destructor
	virtual ~CustomBall ( );

	virtual ViewObject* createViewObject(float aDefaultDepth = 2.0);

	/// we need to parse the Radius - duh
	virtual void  parseProperties(void);
};

#endif // CIRCLEOBJECTS_H
