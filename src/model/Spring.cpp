/* The Butterfly Effect
 * This file copyright (C) 2010,2011  Klaas van Gend
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

#include "Spring.h"
#include "ObjectFactory.h"
#include "tbe_global.h"

//// the Spring class' ObjectFactory
class SpringObjectFactory : public ObjectFactory
{
public:
    SpringObjectFactory(void)
    {
        announceObjectType("Spring", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new Spring());
    }
};
static SpringObjectFactory theSpringObjectFactory;


// this object is a complicated one to get right, because it toys with
// variable width and such... Complicating factor is the fact that the
// physical object is significantly smaller than the image or the real object
//
// let's try to draw:
//
// INITIAL STATE
//  (the ViewObject is drawn over the total outline)
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
//  (the ViewObject is drawn over the current outline)
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
    :   RectObject( QObject::tr("Spring"),
                    QObject::tr("A loose spring. When a force is applied to it,\n"
                                "it retracts and expands."),
                    "spring20",
                    0.4, 0.2, 0.8, 0.0),
        theOtherEndPtr(nullptr),
        theSpringWidth(1.0f)
{
    theProps.setDefaultPropertiesString(
        Property::SPRING_CONSTANT_STRING + QString(":320.0/") ); // 6.5kg over 0.2m @g=9.81
}

Spring::~Spring()
{
    deletePhysicsObject();
    theOtherEndPtr = nullptr;
}

void Spring::adjustParameters(void)
{
    clearShapeList();
    buildShapeList();

    // if there already is a physicsobject, it's wrong
    if (isPhysicsObjectCreated()) {
        deletePhysicsObject();
        createPhysicsObject();
    }
}

void Spring::buildShapeList(void)
{
    DEBUG5("Spring::buildShapeList wxh=%fx%f", getTheWidth(), getTheHeight());
    b2PolygonShape *myBoxShape = new b2PolygonShape();
    myBoxShape->SetAsBox(getTheWidth() / 4.0, getTheHeight() / 2.0);
    b2FixtureDef *myBoxFixture = new b2FixtureDef();

    // get mass:  springs always have a mass!
    float myMass;
    if (theProps.property2Float(Property::MASS_STRING, &myMass))
        myBoxFixture->density = myMass / (getTheWidth() * getTheHeight()) / 2.0;
    assert (myMass > 0.0);
    myBoxFixture->shape    = myBoxShape;
    myBoxFixture->userData = this;
    setFriction(myBoxFixture);
    theShapeList.push_back(myBoxFixture);
}


void Spring::clearObjectReferences()
{
    theOtherEndPtr = nullptr;
}


void Spring::createPhysicsObject(void)
{
    clearShapeList();
    buildShapeList();

    RectObject::createPhysicsObject(getOrigCenter() - Vector(0.25 * getTheWidth(), 0));

    SpringEnd *myOtherEndPtr = dynamic_cast<SpringEnd *>(theOtherEndPtr.get());
    if (myOtherEndPtr == nullptr) {
        theOtherEndPtr = ObjectFactory::createChildObject<SpringEnd>
                         (this, getOrigCenter() + 0.25 * getTheWidth(), getTheWidth() / 2.0, getTheHeight());
        myOtherEndPtr = dynamic_cast<SpringEnd *>(theOtherEndPtr.get());
        assert(myOtherEndPtr != nullptr);
        myOtherEndPtr->theProps.setDefaultPropertiesString(
            QString("%1:%2/").arg(Property::NOCOLLISION_STRING)
            .arg(theProps.getPropertyNoDefault(Property::NOCOLLISION_STRING)));
        theWorldPtr->addObject(theOtherEndPtr);
    }
    myOtherEndPtr->setTheWidth(0.5 * getTheWidth());
    myOtherEndPtr->setTheHeight(getTheHeight());
    myOtherEndPtr->setOrigCenter(getOrigCenter() + Vector(0.25 * getTheWidth(), 0));
    myOtherEndPtr->createPhysicsObject();
    theProps.property2Float(Property::SPRING_CONSTANT_STRING, &(myOtherEndPtr->theSpringConstant));
}

void Spring::deletePhysicsObject(void)
{
    if (theOtherEndPtr)
        theOtherEndPtr->deletePhysicsObject();
    RectObject::deletePhysicsObject();
}

b2Body *Spring::getB2BodyPtrForPosition(const Position &aRelPosition)
{
    if (isPhysicsObjectCreated() == false)
        createPhysicsObject();
    if (aRelPosition.x < 0.0)
        return theB2BodyPtr;
    else {
        SpringEnd *myOtherEndPtr = dynamic_cast<SpringEnd *>(theOtherEndPtr.get());
        if (myOtherEndPtr)
            return myOtherEndPtr->theB2BodyPtr;
        else
            return nullptr;
    }
}


Position Spring::getTempCenter (void) const
{
    // no physics object, no temp center
    if (isPhysicsObjectCreated() == false)
        return getOrigCenter();

    Vector myP1(theB2BodyPtr->GetPosition());

    SpringEnd *myOtherEndPtr = dynamic_cast<SpringEnd *>(theOtherEndPtr.get());
    assert(myOtherEndPtr);
    Vector myP2(myOtherEndPtr->theB2BodyPtr->GetPosition());
    return Position(0.5 * Vector(myP1 + myP2), theB2BodyPtr->GetAngle());
}

qreal Spring::getTempWidth() const
{
    if (theOtherEndPtr == nullptr)
        return getTheWidth();
    SpringEnd *myOtherEndPtr = dynamic_cast<SpringEnd *>(theOtherEndPtr.get());
    assert(myOtherEndPtr);
    return myOtherEndPtr->getDistance() + getTheWidth();
}

void Spring::setOrigCenter ( const Position &aNewPos )
{
    RectObject::setOrigCenter(aNewPos);

    Vector myOffset = Vector(0.25 * getTheWidth(), 0);
    if (theOtherEndPtr != nullptr)
        theOtherEndPtr->setOrigCenter(aNewPos + myOffset);
}

// ##########################################################################
// ##########################################################################
// ##########################################################################

SpringEnd::SpringEnd(Spring *aDBox, const Position &aPos, qreal aWidth, qreal aHeight)
    :   RectObject( QObject::tr("Spring End"),
                    "no tooltip",
                    "spring20",
                    aWidth, aHeight, 0.1, 0.0),
        theOtherEndPtr(aDBox),
        theJointPtr(nullptr),
        theSpringConstant(1.0f)
{
    setOrigCenter(aPos);
    theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

SpringEnd::~SpringEnd()
{
}


void SpringEnd::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
    Vector myAngle  = Vector(getTempCenter().angle);
    Vector myFVector = getDistance() * theSpringConstant * myAngle;
    // don't forget: action = -reaction  -> we need equal opposing forces on both end...
    theOtherEndPtr->theB2BodyPtr->ApplyForce( myFVector.toB2Vec2(),
                                              theOtherEndPtr->theB2BodyPtr->GetPosition(),
                                              true);
    theB2BodyPtr->ApplyForce( (-1.0 * myFVector).toB2Vec2(),
                              theB2BodyPtr->GetPosition(),
                              true);
}

void SpringEnd::createPhysicsObject(void)
{
    if (theB2BodyPtr != nullptr)
        return;
    RectObject::createPhysicsObject();

    // initialise the prismatic (translation) joint:
    // note: Initialize() uses a global coordinate...
    b2PrismaticJointDef myJointDef;
    myJointDef.Initialize(theOtherEndPtr->theB2BodyPtr,
                          theB2BodyPtr,
                          theOtherEndPtr->getOrigCenter().toB2Vec2(),
                          Vector(theOtherEndPtr->getOrigCenter().angle).toB2Vec2());
    myJointDef.userData = nullptr;
    myJointDef.collideConnected = false;
    myJointDef.lowerTranslation = - getTheWidth();
    myJointDef.upperTranslation = + 4 * getTheWidth();
    myJointDef.enableLimit = true;
    myJointDef.enableMotor = false;

    assert(theJointPtr == nullptr);
    theJointPtr = reinterpret_cast<b2PrismaticJoint *>(getB2WorldPtr()->CreateJoint(&myJointDef));
    theWorldPtr->registerCallback(this);
}

void SpringEnd::deletePhysicsObject(void)
{
    RectObject::deletePhysicsObject();
    // the deletePhysics will already kill the joint, thanks to Box2D.
    theJointPtr = nullptr;
}

qreal SpringEnd::getDistance(void) const
{
    if (theJointPtr == nullptr)
        return 0;
    return theJointPtr->GetJointTranslation();
}
