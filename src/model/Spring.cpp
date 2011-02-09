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
#include "tbe_global.h"

//// the Spring class' ObjectFactory
class SpringObjectFactory : public ObjectFactory
{
public:
	SpringObjectFactory(void)
	{	announceObjectType("Spring", this); }
	virtual BaseObject* createObject(void) const
	{	return fixObject(new Spring()); }
};
static SpringObjectFactory theSpringObjectFactory;


// this object is a complicated one to get right, because it toys with
// variable width and such... Complicating factor is the fact that the
// physical object is significantly smaller than the image or the real object
//
// let's try to draw:
//
// INITIAL STATE
//  (the DrawObject is drawn over the total outline)
//      +-------------+-------------+
//      |  physobject |  physobject |
//      |   Spring    *  SpringEnd  |
//      |             |             |
//      +-------------+-------------+
//
//      |<------------------------->|
//             getTheWidth()
//  * = what getOrigCenter returns
//
//
//
// DURING SIMULATION
//  (the DrawObject is drawn over the current outline)
//      +---------+===+----------+
//      |         |   |          |
//      | Spring  | * |SpringEnd |
//      |         |   |          |
//      +---------+===+----------+
//
//      |<------------------------->|
//             getTheWidth()
//      |<---------------------->|
//             getTempWidth()
//  * = what getTempCenter returns
//  X = what getOrigCenter returns - WHICH IS NOT IN THE MIDDLE ANYMORE!!!
//      (Klaas doesn't believe that actually matters here)
//
// this is why adjustParameters() and createPhysicsObject()
// have (new) implementations


Spring::Spring()
		:	RectObject( QObject::tr("Spring"),
				QObject::tr("Something springy."),
				"spring20",
				0.4, 0.2, 0.8, 0.0), theOtherEndPtr(NULL)
{
}

Spring::~Spring()
{
	delete theOtherEndPtr;
	theOtherEndPtr = NULL;
}

void Spring::adjustParameters(void)
{
	clearShapeList();
	buildShapeList();

	// if there already is a physicsobject, it's wrong
	if (isPhysicsObjectCreated())
	{
		deletePhysicsObject();
		createPhysicsObject();
	}
}

void Spring::buildShapeList(void)
{
	DEBUG5("Spring::buildShapeList wxh=%fx%f\n", getTheWidth(),getTheHeight());
	b2PolygonShape* myBoxShape = new b2PolygonShape();
	myBoxShape->SetAsBox(getTheWidth()/4.0, getTheHeight()/2.0);
	b2FixtureDef* myBoxFixture = new b2FixtureDef();

	// get mass:  springs always have a mass!
	float myMass;
	if (theProps.property2Float(Property::MASS_STRING, &myMass))
		myBoxFixture->density = myMass / (getTheWidth()*getTheHeight()) / 2.0;
	assert (myMass > 0.0);
	myBoxFixture->shape    = myBoxShape;
	myBoxFixture->userData = this;
	setFriction(myBoxFixture);
	theShapeList.push_back(myBoxFixture);
}

void Spring::createPhysicsObject(void)
{
	clearShapeList();
	buildShapeList();

	RectObject::createPhysicsObject(getOrigCenter()-Vector(0.25*getTheWidth(),0));

	if (theOtherEndPtr==NULL)
	{
		theOtherEndPtr = new SpringEnd(this, getOrigCenter()+0.25*getTheWidth(), getTheWidth()/2.0, getTheHeight());
		theWorldPtr->addObject(theOtherEndPtr);
	}
	theOtherEndPtr->setTheWidth(0.5*getTheWidth());
	theOtherEndPtr->setTheHeight(getTheHeight());
	theOtherEndPtr->setOrigCenter(getOrigCenter()+Vector(0.25*getTheWidth(),0));
	theOtherEndPtr->createPhysicsObject();
}

void Spring::deletePhysicsObject(void)
{
	if(theOtherEndPtr)
		theOtherEndPtr->deletePhysicsObject();
	RectObject::deletePhysicsObject();
}

b2Body* Spring::getB2BodyPtrForPosition(const Position& aRelPosition)
{
	// see explanation above, as this is only evaluated in the static case,
	// we can keep this very simple :-)
	if (aRelPosition.x < 0.0)
		return theB2BodyPtr;
	else
		return theOtherEndPtr->theB2BodyPtr;
}


Position Spring::getTempCenter (void) const
{
	// no physics object, no temp center
	if (isPhysicsObjectCreated()==false)
		return getOrigCenter();

	Vector myP1(theB2BodyPtr->GetPosition());
	Vector myP2(theOtherEndPtr->theB2BodyPtr->GetPosition());
	return Position(0.5*Vector(myP1+myP2), theB2BodyPtr->GetAngle());
}

qreal Spring::getTempWidth() const
{
	if (theOtherEndPtr==NULL)
		return getTheWidth();
	return theOtherEndPtr->getDistance() + getTheWidth();
}

const QString Spring::getToolTip ( ) const
{
	//: Translators: The %1 will be replaced by a phone number.
	return QObject::tr("Send BOOM to me");
}

void Spring::setOrigCenter ( Position new_var )
{
	RectObject::setOrigCenter(new_var);

	Vector myOffset = Vector(0.25*getTheWidth(),0);
	if (theOtherEndPtr!=NULL)
		theOtherEndPtr->setOrigCenter(new_var + myOffset);
}

// ##########################################################################
// ##########################################################################
// ##########################################################################

SpringEnd::SpringEnd(Spring* aDBox, const Position& aPos, qreal aWidth, qreal aHeight)
		:	RectObject( QObject::tr("Spring End"),
				"no tooltip",
				"spring20",
				aWidth, aHeight, 0.1, 0.0), theOtherEndPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(aPos);
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
	theIsMovable = false;
}

SpringEnd::~SpringEnd()
{
	theWorldPtr->removeObject(this);
}

void SpringEnd::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
	// TODO/FIXME: make this 'magic number' configurable
	// and Klaas thinks this number is too big :-(
	float myMagnification = 500.0;
	Vector myUnitVect = myMagnification*Vector(getTempCenter().angle);
	theOtherEndPtr->theB2BodyPtr->ApplyForce((getDistance()*myUnitVect).toB2Vec2(),
											 theOtherEndPtr->getTempCenter().toB2Vec2());
	theB2BodyPtr->ApplyForce(-(getDistance()*myUnitVect).toB2Vec2(),
											 theOtherEndPtr->getTempCenter().toB2Vec2());
}

DrawObject*  SpringEnd::createDrawObject(void)
{
	return NULL;
}

void SpringEnd::createPhysicsObject(void)
{
	if (theB2BodyPtr!=NULL)
		return;
	RectObject::createPhysicsObject();

	// initialise the prismatic (translation) joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(theOtherEndPtr->theB2BodyPtr,
						  theB2BodyPtr,
						  theOtherEndPtr->getOrigCenter().toB2Vec2(),
						  Vector(theOtherEndPtr->getOrigCenter().angle).toB2Vec2());
	myJointDef.userData = NULL;
	myJointDef.collideConnected = false;
	myJointDef.lowerTranslation = - getTheWidth();
	myJointDef.upperTranslation = + getTheWidth();
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = false;

	assert(theJointPtr==NULL);
	theJointPtr = reinterpret_cast<b2PrismaticJoint*>(getB2WorldPtr()->CreateJoint(&myJointDef));
	theWorldPtr->registerCallback(this);
}

void SpringEnd::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal SpringEnd::getDistance(void) const
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}
