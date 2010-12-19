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
#include "Spring.h"

//// this class' ObjectFactory
class SpringObjectFactory : public ObjectFactory
{
public:
	SpringObjectFactory(void)
	{	announceObjectType("Spring", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Spring()); }
};
static SpringObjectFactory theSpringObjectFactory;

// note that both ends are not colliding with each other:
// that makes the minimum start size
//
// the default spring is lying flat:
// +--------------+--------------+   --
// |              |              |   ^
// |      M       |      M       |   |  theMinSpringHeight and/or theHeight
// |              |              |   v
// +--------------+--------------+   --
//        |<------------>|<----->|
//         theMinDistance  theHalfEndSize
//
// |<--------------------------->|
//         theWidth
//
// the default (spring20) image size also is 20x10cm

const float theSpringMass = 0.3;
const float theHalfEndSize = 0.05;
const float theMinDistance = 2*theHalfEndSize;
const float theMinSpringWidth  = 4*theHalfEndSize;
const float theMinSpringHeight = 2*theHalfEndSize;

Spring::Spring()
		: PolyObject(QObject::tr("Spring"),
			 "TODO/FIXME",
			 "spring20",
			 "",	/* no shapes */
			 0.20, 0.10, theSpringMass, 0.1),
		the1stEnd(NULL), the2ndEnd(NULL)
{
}

Spring::~Spring()
{
	;
}


DrawObject*  Spring::createDrawObject(void)
{
	return BaseObject::createDrawObject();
}

void Spring::createPhysicsObject(Position /*aPosition*/)
{
	DEBUG1("Spring::createPhysicsObject()\n");
//	PolyObject::createPhysicsObject();
	// because we don't have any, and the SpringEnds are not
	// connected to world, let us pass the info through
	if (the1stEnd==NULL || the2ndEnd==NULL)
		createEnds();
	the1stEnd->createPhysicsObject();
	the2ndEnd->createPhysicsObject();
}

void Spring::createEnds(void)
{
	if (the1stEnd==NULL)
		the1stEnd = new SpringEnd();
	if (the2ndEnd==NULL)
		the2ndEnd = new SpringEnd();
}


void Spring::deletePhysicsObject(void)
{
	// because we don't have any, and the SpringEnds are not
	// connected to world, let us pass the info through
	PolyObject::deletePhysicsObject();
	if (the1stEnd!=NULL)
		the1stEnd->deletePhysicsObject();
	if (the2ndEnd!=NULL)
		the2ndEnd->deletePhysicsObject();
}

Position Spring::getTempCenter() const
{
	if (the1stEnd==NULL || the2ndEnd==NULL )
		return getOrigCenter();

	Vector myV1 = the1stEnd->getTempCenter().toVector();
	Vector myV2 = the2ndEnd->getTempCenter().toVector();
	Vector myMiddle = myV1 + myV2;
	myMiddle = 0.5 * myMiddle;

	Vector myDiff = myV1 - myV2;
	double myAngle = myDiff.toAngle();

	return Position(myMiddle.toB2Vec2(), myAngle);
}

qreal Spring::getTheWidth ( ) const
{
	if (the1stEnd==NULL || the2ndEnd==NULL )
		return BaseObject::getTheWidth();

	Vector myV1 = the1stEnd->getTempCenter().toVector();
	Vector myV2 = the2ndEnd->getTempCenter().toVector();

	Vector myLength = myV1-myV2;
	return myLength.length()+2*theHalfEndSize;
}


void Spring::reset(void)
{
	PolyObject::reset();

	// let's create our ends if they don't exist yet
	// and move them to their places
	createEnds();
	float myEndDistance = getTheWidth()-2*theHalfEndSize;
	the1stEnd->setOrigCenter(getOrigCenter() + -0.5*Vector(myEndDistance,0));
	the1stEnd->setTempCenter(getOrigCenter() + -0.5*Vector(myEndDistance,0));
	the1stEnd->setTheHeight(getTheHeight());
	the1stEnd->reset();
	the2ndEnd->setOrigCenter(getOrigCenter() +  0.5*Vector(myEndDistance,0));
	the2ndEnd->setTempCenter(getOrigCenter() +  0.5*Vector(myEndDistance,0));
	the2ndEnd->setTheHeight(getTheHeight());
	the2ndEnd->reset();
}


// ###########################################################################
// ###########################################################################
// ###########################################################################

SpringEnd::SpringEnd()
		:	RectObject( QObject::tr("SpringEnd"),
				"", // no tooltip
				"", // no image
				2*theHalfEndSize, 0.1, 0.5*theSpringMass, 0.1)
{
}

SpringEnd::~SpringEnd()
{
	;
}
