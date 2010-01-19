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

#ifndef POSITION_H
#define POSITION_H

#include "tbe_global.h"
#include <QPointF>

class b2Vec2;
class Position;

class Vector
{
public:
	// constructors
	Vector (qreal aDX=0.0, qreal aDY=0.0);
	Vector (const QPointF& aPoint);
	Vector (const b2Vec2& aVec);

	qreal length(void);

	b2Vec2   toB2Vec2(void);
	Position toPosition(void);
	QString  toString(void);

	qreal dx;
	qreal dy;
};

/**
  * class Position
  * 
  * This class abstracts the Position - x,y,angle
  */
class Position
{
public:
	
	// Constructors/Destructors
	
	/**
	 * Constructor - set x (m), y(m), angle(radial)
	 */
	Position (qreal anX=0.0, qreal aY=0.0, qreal anAngle=0.0);

	Position (const QPointF& aPoint, qreal anAngle = 0.0);

	Position (const b2Vec2& aVec, qreal anAngle = 0.0);

	// Public attributes
	
	/// x-coordinate: x=0 is lower left corner, positive is right. unit: meter
	qreal x;
	
	/// y-coordinate: y=0 is lower left corner, positive is up. unit: meter
	qreal y;
	
	/// angle coordinate: angle=0 is positive x direction, turning counterclockwise. unit: 2Pi for a full turn
	qreal angle;

	const static qreal minimalMove = 0.005;

	b2Vec2  toB2Vec2(void);
	QString toString(void);
	Vector  toVector(void)
	{ return Vector(x,y); }

	qreal length(void);
};


/** add a Vector to a Position
 *  - that implies that the vector is multiplied with the angle
 *    i.e. in the local object coordinates
 */
Position operator+(const Position& p1, const Vector& v1);

/** FIXME: THIS ONE HAS TO GO
 *  add a Position to a Position
 *  - BUG: x+x, y+y, angle+angle is not right
 */
Position operator+(const Position& p1, const Position& p2);

/// add Position+QPointF
Position operator+(const Position& p1, const QPointF& p2);

/// adds a Vector to a Vector
Vector operator+(const Vector& v1, const Vector& v2);

/// subtracts two Positions (including angle!)
/// ofcourse, the end result probably is a vector, but ok...
Position operator-(const Position& p1, const Position& p2);

/// constant multiplies vector only - not the angle
Position operator*(const qreal c1, const Position& p1);

/// constant multiplies vector
Vector operator*(const qreal c1, const Vector& p1);

/// compare two positions
bool operator==(const Position& p1, const Position& p2);

/// compare two positions
bool operator==(const Vector& p1, const Vector& v2);

#endif // POSITION_H
