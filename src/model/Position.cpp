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

#include "Position.h"
#include "Box2D.h"

#include <cmath>

// Constructors/Destructors

Position::Position (qreal anX, qreal aY, qreal anAngle)
	: x(anX), y(aY), angle(anAngle)
{
	// nothing to do here, sorry...
	;
}

Position::Position (const QPointF& aPoint)
		: x(aPoint.x()), y(-aPoint.y()), angle(0)
{
	// again: nothing to do here.
}


Position::Position (const b2Vec2& aVec)
		: x(aVec.x), y(aVec.y), angle(0)
{
	// yawn, again: nothing to do here.
}


qreal Position::length(void)
{
	return sqrt(x*x+y*y);
}


b2Vec2 Position::toB2Vec2(void)
{
	return b2Vec2(x,y);
}


Position operator+(const Position& p1, const Position& p2)
{
	return Position(p1.x+p2.x, p1.y+p2.y, p1.angle+p2.angle);
}

Position operator+(const Position& p1, const QPointF& p2)
{
	return Position(p1.x+p2.x(), p1.y+p2.y(), p1.angle);
}

Position operator-(const Position& p1, const Position& p2)
{
	return Position(p1.x-p2.x, p1.y-p2.y, p1.angle-p2.angle);
}

Position operator*(const qreal p1, const Position& p2)
{
	return Position(p1*p2.x, p1*p2.y, p2.angle);
}

bool operator==(const Position& p1, const Position& p2)
{
	if (fabs(p1.x - p2.x) < Position::minimalMove)
		return false;
	if (fabs(p1.y - p2.y) < Position::minimalMove)
		return false;
	if (p1.angle != p2.angle)
		return false;
	return true;
}
