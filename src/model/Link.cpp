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
#include "DrawObject.h"

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
	DEBUG5("Link::Link() start\n");
	theFirstPtr = NULL;
	theSecondPtr = NULL;
	theFirstLocalPosPtr = NULL;
	theSecondLocalPosPtr = NULL;

	theProps.setDefaultPropertiesString(
			Property::OBJECT1_STRING + QString(":/") +
			Property::OBJECT2_STRING + QString(":/") +
			Property::OVERLAP_STRING + QString(":10/") +
			"-" + Property::MASS_STRING + ":/" );
	DEBUG5("Link::Link() end\n");
}

DrawObject*  Link::createDrawObject(void)
{
	// this temporary draw object is needed to figure out
	// the dimensions of the image we are drawing,
	// so we can keep the aspect ratio correct.
	DrawObject* myTempDrawObject = BaseObject::createDrawObject();
	float myAspectRatio = myTempDrawObject->getUnscaledAspectRatio();
	delete myTempDrawObject;
	theDrawObjectPtr = NULL;

	// this happens if e.g. we are in level editor mode
	if (theFirstPtr==NULL || theFirstLocalPosPtr==NULL ||
		theSecondPtr==NULL || theSecondLocalPosPtr==NULL)
	{
		if (myAspectRatio > 1.0)
		{
			setTheWidth(myAspectRatio);
			setTheHeight(1.0);
		}
		else
		{
			setTheWidth(1.0);
			setTheHeight(myAspectRatio);
		}
		return BaseObject::createDrawObject();
	}


	// we set the longest value to be the width and the shortest to be the
	// height, i.e. FOR NOW, we assume all images to be horizontal...
	Vector myV1 = (theFirstPtr->getOrigCenter()+*theFirstLocalPosPtr).toVector();
	Vector myV2 = (theSecondPtr->getOrigCenter()+*theSecondLocalPosPtr).toVector();
	Vector myDiff = myV2 - myV1;

	// guess we need something like "polar notation"...
	qreal myAngle = myDiff.toAngle();
	qreal myLength= myDiff.length();

	float myOverlap = 10;
	theProps.property2Float(Property::OVERLAP_STRING, &myOverlap);
	float myAdjustedWidth = myLength*(1.0+myOverlap/100.0);
	float myAdjustedHeight= myAdjustedWidth/myAspectRatio;

	setTheWidth(myAdjustedWidth);
	setTheHeight(myAdjustedHeight);

	return BaseObject::createDrawObject();
}


void Link::createPhysicsObject(void)
{
	if (theWorldPtr==NULL)
		return;

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

	myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr,
						  (theFirstPtr->getOrigCenter()+*theFirstLocalPosPtr).toB2Vec2(),
						  (theSecondPtr->getOrigCenter()+*theSecondLocalPosPtr).toB2Vec2());
	myJointDef.userData = this;
	theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);
}

Position Link::getTempCenter() const
{
	if (theFirstPtr==NULL || theFirstLocalPosPtr==NULL ||
		theSecondPtr==NULL || theSecondLocalPosPtr==NULL)
		return getOrigCenter();

	Vector myV1 = (theFirstPtr->getTempCenter()+*theFirstLocalPosPtr).toVector();
	Vector myV2 = (theSecondPtr->getTempCenter()+*theSecondLocalPosPtr).toVector();
	Vector myMiddle = myV1 + myV2;
	myMiddle = 0.5 * myMiddle;

	Vector myDiff = myV1 - myV2;
	double myAngle = myDiff.toAngle();

	return Position(myMiddle.toB2Vec2(), myAngle);
}


void Link::parseProperties(void)
{
	BaseObject::parseProperties();
	if (theWorldPtr==NULL)
		return;

	// *** parse object1 & object2
	// NOTE: if we used the constructor with baseobject, (i.e. properties
	// aren't read yet) this will still work because propertyToObjectPtr
	// only modifies theFirstPtr/theSecondPtr if successful
	theProps.property2ObjectPlusVectorPtr(theWorldPtr, Property::OBJECT1_STRING,
								  &theFirstPtr, &theFirstLocalPosPtr);

	theProps.property2ObjectPlusVectorPtr(theWorldPtr, Property::OBJECT2_STRING,
								  &theSecondPtr, &theSecondLocalPosPtr);

	createPhysicsObject();
}

void Link::updateOrigCenter(void)
{
}
