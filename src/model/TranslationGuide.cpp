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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "Box2D.h"
#include "ObjectFactory.h"
#include "tbe_global.h"
#include "TranslationGuide.h"
#include "World.h"

//// this class' ObjectFactory
class TranslationGuideObjectFactory : public ObjectFactory
{
public:
    TranslationGuideObjectFactory(void)
    {
        announceObjectType("TranslationGuide", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new TranslationGuide());
    }
    bool isObjectForInsertList(void) const override
    {
        return false;
    }
};
static TranslationGuideObjectFactory theRFactory;



TranslationGuide::TranslationGuide()
    : AbstractJoint(),
      theObjectPtr(nullptr),
      theDirection(0.f)
{
    DEBUG5ENTRY;
    initTG_Attributes ();
}

TranslationGuide::TranslationGuide(AbstractObjectPtr anAbstractObject, qreal aDirection)
    : AbstractJoint(), theObjectPtr(anAbstractObject)
{
    DEBUG5("TranslationGuide::TranslationGuide(%p, %f)",
           anAbstractObject.get(), aDirection);
    theDirection = aDirection;
    updateOrigCenter();
    initTG_Attributes();
}

TranslationGuide::~TranslationGuide()
{
}


void TranslationGuide::clearObjectReferences()
{
    AbstractObject::clearObjectReferences();

    theObjectPtr = nullptr;
}


void TranslationGuide::createPhysicsObject(void)
{
    if (theWorldPtr == nullptr)
        return;

    // *** parse object/object1 - if we didn't get theFirstPtr from the constructor
    if (theObjectPtr == nullptr) {
        theObjectPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT1_STRING);
        if (theObjectPtr == nullptr)
            theObjectPtr = theProps.property2ObjectPtr(theWorldPtr, Property::OBJECT_STRING);
    }
    if (theObjectPtr == nullptr) {
        DEBUG1("TranslationGuide: No valid object found...");
        return;
    }
    b2Body *myFirstB2BodyPtr = getB2BodyPtrFor(theObjectPtr,
                                               getOrigCenter() - theObjectPtr->getOrigCenter());
    theObjectPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));
    assert (myFirstB2BodyPtr);

    // Contrary to PivotPoint, there is no object2, so use the ground body.
    // available as theGroundBodyPtr...
    b2Body *mySecondB2BodyPtr = getGroundBodyPtr();

    // *** initialise Box2D's joint:
    // note: Initialize() uses a global coordinate...
    b2PrismaticJointDef myJointDef;
    myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr, getOrigCenter().toB2Vec2(),
                          Vector(theDirection).toB2Vec2());
    myJointDef.userData = this;
    myJointDef.collideConnected = areObjectsColliding;

    // set motor speed and/or torque
    // note that we have the + defined in the mathematical way,
    // with the y in the opposite direction from QT, that means a minus somewhere
    float myForce = 1000.0;
    myJointDef.enableMotor = theProps.property2Float(Property::FORCE_STRING, &myForce);
    float myMotorSpeed = 0.0;
    if (theProps.property2Float(Property::SPEED_STRING, &myMotorSpeed))
        myJointDef.enableMotor = true;
    myJointDef.maxMotorForce = myForce;
    myJointDef.motorSpeed = -myMotorSpeed;

    theJointPtr = (b2PrismaticJoint *) getB2WorldPtr()->CreateJoint(&myJointDef);
}

void TranslationGuide::initTG_Attributes ( )
{
    areObjectsColliding = false;

    theToolTip = QObject::tr("Objects are limited to only move along one axis");
    theProps.setDefaultPropertiesString(
        Property::OBJECT_STRING + QString(":/") +
        Property::SPEED_STRING + QString(":/") +
        Property::FORCE_STRING + QString(":/") +
        Property::COLLIDE_STRING + QString(":false/") +
        "-" + Property::MASS_STRING + ":/" );
}

void TranslationGuide::parseProperties(void)
{
    AbstractJoint::parseProperties();
    theProps.property2Bool(Property::COLLIDE_STRING, &areObjectsColliding);
}


void TranslationGuide::updateOrigCenter(void)
{
    setOrigCenter(theObjectPtr->getOrigCenter());
}

