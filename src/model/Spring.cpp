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


#define STARTDISTANCE 0.2f
const Vector Spring::HANDLEOFFSET   = Vector(0,STARTDISTANCE);

Spring::Spring()
		:	RectObject( QObject::tr("Detonator Box"),
				"",
				"spring20",
				0.33, 0.35, 4.0, 0.0), theHandleObjectPtr(NULL)
{
	theProps.setDefaultPropertiesString(
		Property::PHONENUMBER_STRING + QString(":/") );
}

Spring::~Spring()
{
	delete theHandleObjectPtr;
	theHandleObjectPtr = NULL;
}

void Spring::callbackStep (qreal /*aTimeStep*/, qreal aTotalTime)
{
}

void Spring::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();
	if (theHandleObjectPtr)
		theHandleObjectPtr->createPhysicsObject();
}

void Spring::deletePhysicsObject(void)
{
	if(theHandleObjectPtr)
		theHandleObjectPtr->deletePhysicsObject();
	RectObject::deletePhysicsObject();
}


const QString Spring::getToolTip ( ) const
{
	//: Translators: The %1 will be replaced by a phone number.
	return QObject::tr("Send BOOM to me");
}

void Spring::reset(void)
{
	RectObject::reset();

	// the reset will already do a delete+create
	// of the handle objectptr, so no need to reset it specifically here...
	if (theHandleObjectPtr==NULL)
	{
		theHandleObjectPtr = new SpringHandle(this, getOrigCenter()+HANDLEOFFSET);
		theWorldPtr->addObject(theHandleObjectPtr);
	}

	theWorldPtr->registerCallback(this);
}

void Spring::setOrigCenter ( Position new_var )
{
	RectObject::setOrigCenter(new_var);

	if (theHandleObjectPtr!=NULL)
		theHandleObjectPtr->setOrigCenter(new_var+HANDLEOFFSET);
}

// ##########################################################################
// ##########################################################################
// ##########################################################################

SpringHandle::SpringHandle(Spring* aDBox, const Position& aPos)
		:	RectObject( QObject::tr("Spring End"),
				"no tooltip",
				"spring20",
				0.25, 0.26, 0.1, 0.0), theDBoxPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(aPos);
	createPhysicsObject();
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
	theIsMovable = false;
}

SpringHandle::~SpringHandle()
{
	theWorldPtr->removeObject(this);
}

void SpringHandle::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
	// if down, stay down and signal the box
	if (theJointPtr->GetJointTranslation() <= theJointPtr->GetLowerLimit())
	{
		theJointPtr->SetMaxMotorForce(0);
	}
}

DrawObject*  SpringHandle::createDrawObject(void)
{
	RectObject::createDrawObject();
	// redo the ZValue: BaseObject will set it to 2.0 (default for DrawObjects)
	// if not in Properties, set to 1.9: the Handle draws behind the Box
	setDrawObjectZValue(1.9);
	return theDrawObjectPtr;
}

void SpringHandle::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();

	// initialise the prismatic (translation) joint:
	// note: Initialize() uses a global coordinate...
	b2PrismaticJointDef myJointDef;
	myJointDef.Initialize(theDBoxPtr->theB2BodyPtr, theB2BodyPtr, getOrigCenter().toB2Vec2(), Vector(0,1.0).toB2Vec2());
	myJointDef.userData = NULL;
	myJointDef.collideConnected = false;
	myJointDef.maxMotorForce = 120.0f;
	myJointDef.motorSpeed = 2.0;
	myJointDef.lowerTranslation = - STARTDISTANCE/2.0f;
	myJointDef.upperTranslation = 0.0;
	myJointDef.enableLimit = true;
	myJointDef.enableMotor = true;

	assert(theJointPtr==NULL);
	theJointPtr = reinterpret_cast<b2PrismaticJoint*>(getB2WorldPtr()->CreateJoint(&myJointDef));
}

void SpringHandle::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal SpringHandle::getDistance(void)
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}

void SpringHandle::reset(void)
{
	RectObject::reset();
	theWorldPtr->registerCallback(this);
}
