/* The Butterfly Effect 
 * This file copyright (C) 2009, 2010  Klaas van Gend
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
	// obvious constructors
	Vector (void);
	Vector (qreal aDX, qreal aDY);
	Vector (const QPointF& aPoint);
	Vector (const b2Vec2& aVec);

	/// constructs a vector of length 1 in direction anAngle
	Vector (qreal anAngle);


	qreal length(void);

	b2Vec2   toB2Vec2(void) const;
	Position toPosition(void) const;
	QString  toString(void) const;
	qreal	 toAngle(void) const;

	/// @returns this vector, rotated along anAngle
	Vector rotate(qreal anAngle) const;

	/** Converts a string in format "(0.0,0.0)" into this Vector
	  * If the conversion is not successful, the Vector is not modified.
	  * @param aString the string to convert
	  * @returns true if conversion successful.
	  */
	bool fromString(QString aString);

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

	const static qreal minimalMove;

	b2Vec2  toB2Vec2(void) const;
	QString toString(void) const;
	Vector  toVector(void) const
	{ return Vector(x,y); }

	qreal length(void);
};


/** add a Vector to a Position
 *  - that implies that the vector is multiplied with the angle
 *    i.e. in the local object coordinates
 */
Position operator+(const Position& p1, const Vector& v1);

/// add Position+QPointF
Position operator+(const Position& p1, const QPointF& p2);

/// adds a Vector to a Vector
Vector operator+(const Vector& v1, const Vector& v2);

/// subtracts two Vectors
Vector operator-(const Vector& p1, const Vector& p2);

/// constant multiplies vector
Vector operator*(const qreal c1, const Vector& p1);

/// scale - Vector multiplies Vector into a Vector
Vector operator*(const Vector& v1, const Vector& v2);

/// compare two positions
bool operator==(const Position& p1, const Position& p2);

/// compare two vectors
bool operator==(const Vector& p1, const Vector& v2);

#endif // POSITION_H
