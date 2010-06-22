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

#include "tbe_global.h"
#include "Link.h"

#include "Box2D.h"
#include <cassert>

//// this class' ObjectFactory
class LinkObjectFactory : public ObjectFactory
{
public:
	LinkObjectFactory(void)
	{	announceObjectType("Link", this); }
	virtual BaseObject* createObject(void) const
	{	return new Link(); }
};
static LinkObjectFactory theLinkFactory;

Link::Link()
{
	theFirstPtr = NULL;
	theSecondPtr = NULL;
	theFirstLocalPosPtr = NULL;
	theSecondLocalPosPtr = NULL;
}

void Link::createPhysicsObject(void)
{
	if (theWorldPtr==NULL)
		return;

	// *** parse connection1
	// NOTE: if we used the constructor with baseobject, this will still work
	// because propertyToObjectPtr only modifies theFirstPtr if successful
	propertyToObjectPlusVectorPtr(theWorldPtr, Property::OBJECT1_STRING,
								  &theFirstPtr, &theFirstLocalPosPtr);
	assert(theFirstPtr!=NULL);
	assert(theFirstLocalPosPtr!=NULL);
	if (theFirstPtr==NULL)
	{
		DEBUG4("Link: No valid first object found...\n");
		return;
	}
	b2Body* myFirstB2BodyPtr = theFirstPtr->theB2BodyPtr;
	assert (myFirstB2BodyPtr);
	theFirstPtr->addJoint(this);

	// *** parse connection2
	propertyToObjectPlusVectorPtr(theWorldPtr, Property::OBJECT2_STRING,
								  &theSecondPtr, &theSecondLocalPosPtr);
	assert(theSecondPtr!=NULL);
	assert(theSecondLocalPosPtr!=NULL);
	if (theSecondPtr==NULL)
	{
		DEBUG4("Link: No valid second object found...\n");
		return;
	}
	b2Body* mySecondB2BodyPtr = theSecondPtr->theB2BodyPtr;
	assert (mySecondB2BodyPtr);
	theSecondPtr->addJoint(this);

	// *** initialise Box2D's distance joint:
	// note: Initialize() uses a global coordinate...
	b2DistanceJointDef myJointDef;

	// TODO: this one...
	myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr,
						  (theFirstPtr->getOrigCenter()+*theFirstLocalPosPtr).toB2Vec2(),
						  (theSecondPtr->getOrigCenter()+*theSecondLocalPosPtr).toB2Vec2());
	myJointDef.userData = this;
	theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);

}

Position Link::getTempCenter() const
{
	Vector myV1 = (theFirstPtr->getTempCenter()+*theFirstLocalPosPtr).toVector();
	Vector myV2 = (theSecondPtr->getTempCenter()+*theSecondLocalPosPtr).toVector();
	Vector myMiddle = myV1 + myV2;
	myMiddle = 0.5 * myMiddle;

	// TODO: calculate the angle!
	Vector myDiff = myV1 - myV2;
	if (myDiff.dx == 0)
		myDiff.dx = 0.01;
	double myAngle = atan(abs(myDiff.dy / myDiff.dx));

	if (myDiff.dx > 0)
	{
		if (myDiff.dy > 0)
			myAngle = myAngle;
		else
			myAngle = 2*PI - myAngle;
	}
	else
	{
		if (myDiff.dy > 0)
			myAngle = PI - myAngle;
		else
			myAngle = PI + myAngle;
	}

	myAngle -= PI/2;
	return Position(myMiddle.toB2Vec2(), myAngle);
}


void Link::parseProperties(void)
{
}

void Link::updateOrigCenter(void)
{
}
