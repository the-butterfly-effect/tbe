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

#ifndef ABSTRACTBALL_H
#define ABSTRACTBALL_H

#include "BaseObject.h"

/**
  * class AbstractBall
  * 
  * The AbstractBall class essentially models anything resembling a circle.
  * It is used with a specialised factory to build the various types of balls
  */

class AbstractBall : public BaseObject
{
public:

	// Constructors/Destructors
	//  

	/// Constructor
	AbstractBall (const QString& aName,
				  const QString& aTooltip,
				  const QString& anImageName,
				  qreal aRadius,
				  qreal aMass,
				  qreal aBounciness);

	/// virtual destructor
	virtual ~AbstractBall ( );

	// Public attribute accessor methods
	//  

	virtual DrawObject* createDrawObject();

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return theBallName;	}

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if its mass is larger than 0.001 kg
	virtual b2BodyType getObjectType(void) const
	{	return theB2ObjectType; }

	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return theBallTooltip; }

	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

protected:
	/// internally used by AbstractBall and CustomBall to
	/// actually create the b2Shape and b2FixtureDef
	virtual void createBallShapeFixture(float aRadius, float aMass);

private:
	QString theBallName;
	QString theBallTooltip;
	QString theBallImage;

	b2BodyType theB2ObjectType;
};


class CustomBall : public AbstractBall
{
public:
	/// Constructor
	CustomBall ();

	/// virtual destructor
	virtual ~CustomBall ( );

	virtual DrawObject* createDrawObject();

	/// we need to parse the Radius - duh
	virtual void  parseProperties(void);
};

#endif // ABSTRACTBALL_H
