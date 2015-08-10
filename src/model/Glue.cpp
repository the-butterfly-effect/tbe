/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

//#include "Box2D.h"
#include "Glue.h"
#include "ObjectFactory.h"
#include "tbe_global.h"
#include "ViewObject.h"

#include <cassert>

//// this class' ObjectFactory
class GlueObjectFactory : public ObjectFactory
{
public:
	GlueObjectFactory(void)
	{	announceObjectType("Glue", this); }
    virtual AbstractObject* createObject(void) const
	{	return fixObject(new Glue()); }
};
static GlueObjectFactory theGlueFactory;

Glue::Glue() : AbstractJoint()
{
	DEBUG5("Glue::Glue() start\n");
	theFirstPtr = NULL;
	theSecondPtr = NULL;
	theFirstLocalPosPtr = NULL;
	theSecondLocalPosPtr = NULL;
	theLinkPtr = NULL;

    theToolTip = QObject::tr("Glue links two objects immovably together.");
    theProps.setDefaultPropertiesString(
			Property::OBJECT1_STRING + QString(":/") +
			Property::OBJECT2_STRING + QString(":/") +
			"-" + Property::MASS_STRING + ":/" );
	DEBUG5("Glue::Glue() end\n");
}


void Glue::createPhysicsObject(void)
{
	DEBUG5("Glue::createPhysicsObject() for %s, type %d\n", ASCII(getName()), getObjectType());
	if (theWorldPtr==NULL)
		return;

	assert(theFirstPtr!=NULL);
	assert(theFirstLocalPosPtr!=NULL);
	if (theFirstPtr==NULL)
	{
		DEBUG4("Link: No valid first object found...\n");
		return;
	}
	b2Body* myFirstB2BodyPtr = getB2BodyPtrFor(theFirstPtr, theFirstLocalPosPtr->toPosition());
	assert (myFirstB2BodyPtr);
    theFirstPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

	assert(theSecondPtr!=NULL);
	assert(theSecondLocalPosPtr!=NULL);
	if (theSecondPtr==NULL)
	{
		DEBUG4("Link: No valid second object found...\n");
		return;
	}
	b2Body* mySecondB2BodyPtr = getB2BodyPtrFor(theSecondPtr, theSecondLocalPosPtr->toPosition());
	assert (mySecondB2BodyPtr);
    theSecondPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

	Position myCenter = getTempCenter();
	Vector   myHalfWidth(getTheWidth()/2.0, 0);

	// *** initialise Box2D's revolute joint (from AbstractJoint):
	b2RevoluteJointDef myRevJointDef;
	myRevJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr, (myCenter-myHalfWidth).toB2Vec2());
    myRevJointDef.userData = this;
	myRevJointDef.collideConnected = true;
	theJointPtr = static_cast<b2RevoluteJoint*>(getB2WorldPtr()->CreateJoint(&myRevJointDef));

	// *** initialise Box2D's distance joint (new in Glue classdef):
	b2DistanceJointDef myDisJointDef;
	myDisJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr,
						  (myCenter+myHalfWidth).toB2Vec2(),
						  (myCenter+myHalfWidth).toB2Vec2());
    myDisJointDef.userData = this;
	myDisJointDef.collideConnected = true;
	theLinkPtr = static_cast<b2DistanceJoint*>(getB2WorldPtr()->CreateJoint(&myDisJointDef));
}

Position Glue::getTempCenter() const
{
	if (theFirstPtr==NULL || theFirstLocalPosPtr==NULL ||
		theSecondPtr==NULL || theSecondLocalPosPtr==NULL)
		return getOrigCenter();

	Vector myV1 = (theFirstPtr->getTempCenter()+*theFirstLocalPosPtr).toVector();
	Vector myV2 = (theSecondPtr->getTempCenter()+*theSecondLocalPosPtr).toVector();
	Vector myMiddle = myV1 + myV2;
	myMiddle = 0.5 * myMiddle;

	Vector myDiff = myV1 - myV2;
	double myAngle = myDiff.toAngle() - PI/2.0;

	return Position(myMiddle.toB2Vec2(), myAngle);
}


void Glue::parseProperties(void)
{
	AbstractJoint::parseProperties();

	// *** parse object1 & object2
	// NOTE: if we used the constructor with AbstractObject, (i.e. properties
	// aren't read yet) this will still work because propertyToObjectPtr
	// only modifies theFirstPtr/theSecondPtr if successful
    theFirstPtr =  theProps.property2ObjectPlusVectorPtr(theWorldPtr,
                                    Property::OBJECT1_STRING,
                                    &theFirstLocalPosPtr);
    theSecondPtr = theProps.property2ObjectPlusVectorPtr(theWorldPtr,
                                    Property::OBJECT2_STRING,
                                    &theSecondLocalPosPtr);
}

void Glue::updateOrigCenter(void)
{
}
