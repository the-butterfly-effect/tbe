/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2012  Klaas van Gend
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

#include "Position.h"
#include "Box2D.h"

#include <cmath>
#include <QtCore/QStringList>

qreal THESCALE = 100;

const qreal Position::minimalMove = b2_linearSlop;
const qreal Position::minimalRot  = b2_angularSlop;

// Constructors/Destructors

Position::Position (qreal anX, qreal aY, qreal anAngle)
	: x(anX), y(aY), angle(anAngle)
{	; // nothing to do here, sorry...
}

Position::Position (const QPointF& aPoint, qreal anAngle)
		: x(aPoint.x()/THESCALE), y(-aPoint.y()/THESCALE), angle(anAngle)
{	; // nothing to do here, sorry...
}

Position::Position (const Vector& aPoint, qreal anAngle)
		: x(aPoint.dx), y(aPoint.dy), angle(anAngle)
{	; // nothing to do here, sorry...
}

Position::Position (const b2Vec2& aVec, qreal anAngle)
		: x(aVec.x), y(aVec.y), angle(anAngle)
{	; // nothing to do here, sorry...
}


qreal Position::length(void)
{ assert(isValid()); return sqrt(x*x+y*y); }

b2Vec2 Position::toB2Vec2(void) const
{ assert(isValid()); return b2Vec2(x,y); }

QString Position::toString(void) const
{	return QString("(%1,%2)@%3").arg(x).arg(y).arg(angle); }

Vector::Vector (void)
		: dx(0), dy(0)
{	; // nothing to do here, sorry...
}

Vector::Vector (qreal anAngle)
{
	dx = cos(anAngle);
	dy = sin(anAngle);
}

Vector::Vector (const Position& aPosition)
		: dx(aPosition.x), dy(aPosition.y)
{
	// and another empty body here
}

Vector::Vector (qreal aDX, qreal aDY)
	: dx(aDX), dy(aDY)
{	; // nothing to do here, sorry...
}

Vector::Vector (const QPointF& aPoint)
		: dx(aPoint.x()/THESCALE), dy(-aPoint.y()/THESCALE)
{	; // nothing to do here, sorry...
}


Vector::Vector (const b2Vec2& aVec)
		: dx(aVec.x), dy(aVec.y)
{	; // nothing to do here, sorry...
}

bool Vector::fromString(QString aString)
{
	// don't even bother converting an empty string
	if(aString.isEmpty())
		return false;

	bool isOK = false;
	qreal myDX, myDY;
	QStringList myList;
	QString myValue;

	aString = aString.trimmed();
	if (aString.left(1)!="(")
		goto done;
	aString = aString.remove(0,1);
	if (aString.right(1)!=")")
		goto done;
	aString.chop(1);

	// we have deltaX before and deltaY after the comma
	myList = aString.split(",");
	if (myList.count()!=2)
		goto done;

	myDX =  myList.first().toFloat(&isOK);
	if (isOK == false)
		goto done;

	myDY =  myList.last().toFloat(&isOK);
	if (isOK == false)
		goto done;

	dx = myDX;
	dy = myDY;
	isOK = true;

done:
	if (isOK==false)
		DEBUG4("Vector.fromString(%s) failed!", ASCII(aString));
	return isOK;
}

qreal Vector::length(void) const
{	return sqrt(dx*dx+dy*dy); }

Vector Vector::rotate(qreal anAngle) const
{
	float myCos = cos(anAngle);
	float mySin = sin(anAngle);
	return  Vector(myCos*dx - mySin*dy, mySin*dx + myCos*dy);
}

qreal Vector::toAngle(void) const
{
	// prevent division by zero
	// even though C++ already knows about NaN and such
	if (fabs(dx) < Position::minimalMove)
	{
		if (dy > 0.0)
			return PI/2.0;
		else
			return 3.0*PI/2.0;
	}

	qreal myAngle = atanf(fabs(dy / dx));

	if (dx > 0.0)
	{
		if (dy < 0.0)
			myAngle = 2*PI - myAngle;
	}
	else
	{
		if (dy > 0.0)
			myAngle = PI - myAngle;
		else
			myAngle = PI + myAngle;
	}
	return myAngle;
}

b2Vec2 Vector::toB2Vec2(void) const
{	return b2Vec2(dx,dy); }

Position Vector::toPosition(void) const
{	return Position(dx,dy, 0); }

QPointF Vector::toQPointF(void) const
{	return QPointF(THESCALE*dx,-THESCALE*dy); }

QString Vector::toString(void) const
{	return QString("(%1,%2)").arg(dx).arg(dy); }

Vector Vector::toUnitVector(void) const
{
	float myRecipLength = 1/length();
	return Vector(myRecipLength*dx, myRecipLength*dy);
}




bool areQRealsTheSame(qreal anA, qreal aB)
{
	return fabs(anA - aB) < Position::minimalMove;
}




Position operator+(const Position& p1, const Vector& v1)
{
    assert(p1.isValid());
    float myCos = cos(p1.angle);
    float mySin = sin(p1.angle);
    return Position(p1.x +v1.dx*myCos - v1.dy*mySin, p1.y +v1.dx*mySin +v1.dy*myCos, p1.angle);
}

Position operator-(const Position& p1, const Vector& v1)
{
    assert(p1.isValid());
    float myCos = cos(p1.angle);
    float mySin = sin(p1.angle);
    return Position(p1.x -v1.dx*myCos + v1.dy*mySin, p1.y -v1.dx*mySin -v1.dy*myCos, p1.angle);
}

Position operator+(const Position& p1, const QPointF& p2)
{
    assert(p1.isValid());
    return Position(p1.x+p2.x()/THESCALE, p1.y+p2.y()/THESCALE, p1.angle);
}

Position operator-(const Position& p1, const QPointF& p2)
{
    assert(p1.isValid());
    return Position(p1.x-p2.x()/THESCALE, p1.y-p2.y()/THESCALE, p1.angle);
}

Vector operator+(const Vector& v1, const Vector& v2)
{
    return Vector(v1.dx+v2.dx, v1.dy+v2.dy);
}

Vector operator-(const Vector& v1, const Vector& v2)
{
    return Vector(v1.dx-v2.dx, v1.dy-v2.dy);
}


Vector operator*(const qreal c1, const Vector& v1)
{
    return Vector(c1*v1.dx, c1*v1.dy);
}

Vector operator*(const Vector& v1, const Vector& v2)
{
    return Vector(v2.dx*v1.dx, v2.dy*v1.dy);
}

bool operator==(const Position& p1, const Position& p2)
{
    if (!areQRealsTheSame(p1.x, p2.x))
        return false;
    if (!areQRealsTheSame(p1.y, p2.y))
        return false;
    if (fabs(p1.angle-p2.angle) > Position::minimalRot)
        return false;
    return true;
}

bool operator!=(const Position& p1, const Position& p2)
{
    return !(p1==p2);
}

bool operator==(const Vector& v1, const Vector& v2)
{
    if (!areQRealsTheSame(v1.dx, v2.dx))
        return false;
    if (!areQRealsTheSame(v1.dy, v2.dy))
        return false;
    return true;
}

bool operator!=(const Vector& v1, const Vector& v2)
{
    return !(v1==v2);
}

#ifdef QT_DEBUG
QDebug operator<<(QDebug dbg, const Position& p)
{
    dbg.nospace() << p.toString();
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Vector& v)
{
    dbg.nospace() << v.toString();
    return dbg.space();
}
#endif
