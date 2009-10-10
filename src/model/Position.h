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

	Position (const QPointF& aPoint);

	Position (const b2Vec2& aVec);
	
	// Public attributes
	
	/// x-coordinate: x=0 is lower left corner, positive is right. unit: meter
	qreal x;
	
	/// y-coordinate: y=0 is lower left corner, positive is up. unit: meter
	qreal y;
	
	/// angle coordinate: angle=0 is positive x direction, turning counterclockwise. unit: 2Pi for a full turn
	qreal angle;

	const static qreal minimalMove = 0.005;

	b2Vec2 toB2Vec2(void);

	qreal length(void);
};


/// add two Positions together (including angle!)
Position operator+(const Position& p1, const Position& p2);

/// add Position+QPointF
Position operator+(const Position& p1, const QPointF& p2);

/// subtracts two Positions (including angle!)
/// ofcourse, the end result probably is a vector, but ok...
Position operator-(const Position& p1, const Position& p2);

/// constant multiplies vector only - not the angle
Position operator*(const qreal p1, const Position& p2);


#endif // POSITION_H
