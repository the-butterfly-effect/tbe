/* The Butterfly Effect
 * This file copyright (C) 2010,2011,2012  Klaas van Gend
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

#include "BalloonCactus.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "Property.h"


//
//  This source file lists 3 classes:
//    * Balloon
//    * Cactus
//    * BedOfNails
//

constexpr qreal TIMER_NOT_IN_USE = -1.0;

///---------------------------------------------------------------------------
///------------------------ Balloon ------------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
class BalloonObjectFactory : public ObjectFactory
{
public:
    BalloonObjectFactory(void)
    {
        announceObjectType("Balloon", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new Balloon());
    }
};
static BalloonObjectFactory theBalloonObjectFactory;


const qreal Balloon::POPPING_TIME = 0.3;
const qreal Balloon::POPPED_MASS  = 0.6;
const qreal Balloon::POPPED_TIME  = 2.0;


Balloon::Balloon()
    : PolyObject(QObject::tr("Balloon"),
                 QObject::tr("A helium balloon. Lighter than air, it moves up.\nIt will pop when it hits sharp objects or gets squashed."),
                 "Balloon;BalloonPoof;BalloonRest;Empty",
                 "(-0.018,0.18)=(-0.07,0.16)=(-0.12,0.1)=(-0.13,0.017)=(-0.1,-0.08)"
                 "=(-0.03,-0.16)=(0.006,-0.17)=(0.039,-0.16)=(0.10,-0.08)"
                 "=(0.13,0.015)=(0.11,0.11)=(0.07,0.16)=(0.01,0.18)",
                 0.27, 0.36, 0.1, 0.7),
      thePoppingTimeStart(TIMER_NOT_IN_USE)
{
    theState = BALLOON;
    theB2BodyDefPtr->linearDamping  = 2.1f;
    theB2BodyDefPtr->angularDamping = 0.4f;
}

Balloon::~Balloon()
{
}


void Balloon::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
    DEBUG6("Balloon receives callback");

    switch (theState) {
    case BALLOON:
        callbackStepBalloon(aDeltaTime, aTotalTime);
        break;
    case POPPING:
        notifyJoints(JointInterface::DELETED);
        callbackStepPopping(aDeltaTime, aTotalTime);
        break;
    case POPPED:
        callbackStepPopped(aDeltaTime, aTotalTime);
        break;
    case GONE:
        break;
    }
}

void Balloon::callbackStepBalloon(qreal, qreal)
{
    // the upward force...
    // the balloon weighs 100 grams, i.e. it should have a constant gravity
    // pull of 0.981 N.
    //
    // Why isn't that the case?
    // See PolyObject::fillShapeList - the density is calculated over the AABB
    // of the balloon, not the actual surface.
    // so Box2D thinks our Balloon is actually lighter than 100 grams...
    //
    // That's why a 0.8N upwards force suffices :-)
    theB2BodyPtr->ApplyForce(b2Vec2(0, 0.8), (getTempCenter() + Vector(0, 0.1)).toB2Vec2(), true);

    // damping is handled by Box2D - linearDamping and AngularDamping...
}

void Balloon::callbackStepPopped(qreal, qreal aTotalTime)
{
    assert(thePoppingTimeStart > TIMER_NOT_IN_USE);
    qreal myDelta = aTotalTime - thePoppingTimeStart - POPPING_TIME;
    if ( myDelta >= POPPED_TIME)
        goToState(GONE);
}

void Balloon::callbackStepPopping(qreal, qreal aTotalTime)
{
    // is this the first callback in Popping state???
    if (thePoppingTimeStart < 0) {
        thePoppingTimeStart = aTotalTime;
        switchToSmallShape();
    }
    if (aTotalTime - thePoppingTimeStart > POPPING_TIME)
        goToState(POPPED);
}


void Balloon::causeWounded(WhyWounded aReason)
{
    if (aReason == HEAT || aReason == STINGING || aReason == SLICING || aReason == EXPLOSION)
        if (theState == BALLOON)
            goToState(POPPING);
}


void Balloon::createPhysicsObject(void)
{
    theState = BALLOON;
    thePoppingTimeStart = TIMER_NOT_IN_USE;
    clearShapeList();
    fillShapeList();
    PolyObject::createPhysicsObject();
    theWorldPtr->registerCallback(this);
}

void Balloon::deletePhysicsObject(void)
{
    // nothing much to do here that actually has to do with delete...
    theState = BALLOON;
    thePoppingTimeStart = TIMER_NOT_IN_USE;
    clearShapeList();
    fillShapeList();

    PolyObject::deletePhysicsObject();
}

void Balloon::deletePhysicsObjectForReal(void)
{
    updateViewObject(false);
    getB2WorldPtr()->DestroyBody(theB2BodyPtr);
    theB2BodyPtr = nullptr;
}


Balloon::States Balloon::goToState(Balloon::States aNewState)
{
    DEBUG5("Balloon change state request from %d to %d.", theState, aNewState);

    switch (theState) {
    case BALLOON:
        if (aNewState == POPPING) {
            theState = POPPING;
            // everything else will be handled in callbackStepPopping
        }
        if (aNewState == POPPED) {
            // typical case of "this should never happen"
            assert(false);
        }
        break;
    case POPPING:
        if (aNewState == POPPED) {
            theState = POPPED;
        }
        break;
    case POPPED:
        if (aNewState == GONE) {
            theState = GONE;
            deletePhysicsObjectForReal();
        }
        break;
    case GONE:
        // nothing to be done here...
        break;
    }
    return theState;
}

void Balloon::notifyJoints(JointInterface::JointStatus aStatus)
{
    if (theState != BALLOON && aStatus == JointInterface::CREATED)
        return;
    AbstractObject::notifyJoints(aStatus);
}


void Balloon::reportNormalImpulseLength(qreal anImpulseLength, AbstractObject *)
{
    // also pop the balloon if it is maltreated
    // WARNING: Magic number here!!!
    if (anImpulseLength > 1.8 && theState == BALLOON)
        goToState(POPPING);
}


void Balloon::switchToSmallShape(void)
{
    // save the current position - as it is only stored within the B2Body
    // and we'll kill it in the next line...
    Position myCurrentPos = getTempCenter();

    deletePhysicsObjectForReal();
    clearShapeList();

    b2PolygonShape *myRestShape = new b2PolygonShape();
    myRestShape->SetAsBox(0.05, 0.05);
    b2FixtureDef *myRestDef = new b2FixtureDef();
    myRestDef->density = 0.001 / (0.1 * 0.1);
    myRestDef->userData = this;
    myRestDef->shape   = myRestShape;
    theShapeList.push_back(myRestDef);

    PolyObject::createPhysicsObject(myCurrentPos);
}


///---------------------------------------------------------------------------
///------------------------ Cactus -------------------------------------------
///---------------------------------------------------------------------------


//// this class' ObjectFactory
class CactusObjectFactory : public ObjectFactory
{
public:
    CactusObjectFactory(void)
    {
        announceObjectType("Cactus", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new Cactus());
    }
};
static CactusObjectFactory theCactusObjectFactory;


Cactus::Cactus() : PolyObject(QObject::tr("Cactus"),
                                  QObject::tr("Cactus (Cactacea Bulbuous Stingus):\nA cactus has sharp spines."),
                                  "Cactus",
                                  // first the pot:
                                  "(-0.105,-0.038)=(-0.05,-0.2)=(0.05,-0.2)=(0.105,-0.038);"
                                  // then the plant itself:
                                  "(0.045,-0.036)=(0.085,0.012)=(0.092,0.089)=(0.046,0.156)=(0.00,0.171)"
                                  "=(-0.046,0.156)=(-0.092,0.089)=(-0.085,0.012)=(-0.045,-0.036)"
                                  ";(-0.125,-0.2);(0.125,0.2)"
                                  ,
                                  0.25, 0.4, 1.0, 0.3)
{
}

Cactus::~Cactus()
{
}

void Cactus::callBackSensor(const ContactInfo &aPoint)
{
    AbstractObject *myOtherObject = nullptr;

    // which one of the two shapes is not me?
    if (aPoint.myFixtureA->GetUserData() == this)
        myOtherObject = aPoint.myFixtureB->GetUserData();
    if (aPoint.myFixtureB->GetUserData() == this)
        myOtherObject = aPoint.myFixtureA->GetUserData();
    if (myOtherObject == nullptr)
        return;

    // try to sting it!
    myOtherObject->causeWounded(AbstractObject::STINGING);
}

void Cactus::fillShapeList(void)
{
    PolyObject::fillShapeList();

    // And add the sensor to the shapes
    b2Vec2 mySensorShapeV[10];
    mySensorShapeV[0].Set( 0.078, -0.036);
    mySensorShapeV[1].Set( 0.116,  0.012);
    mySensorShapeV[2].Set( 0.125,  0.085);
    mySensorShapeV[3].Set( 0.099,  0.147);
    mySensorShapeV[4].Set( 0.038,  0.2);
    mySensorShapeV[5].Set(-0.038,  0.2);
    mySensorShapeV[6].Set(-0.099,  0.147);
    mySensorShapeV[7].Set(-0.125,  0.085);
    mySensorShapeV[8].Set(-0.116,  0.012);
    mySensorShapeV[9].Set(-0.078, -0.036);
    b2PolygonShape *mySensorShapePtr = new b2PolygonShape();
    mySensorShapePtr->Set(mySensorShapeV, 10);
    b2FixtureDef *mySensorDef = new b2FixtureDef();
    mySensorDef->shape    = mySensorShapePtr;
    mySensorDef->isSensor = true;
    mySensorDef->userData = this;
    theShapeList.push_back(mySensorDef);
}


///---------------------------------------------------------------------------
///------------------------ Bed Of Nails -------------------------------------
///---------------------------------------------------------------------------


// this class' ObjectFactory
class BedOfNailsObjectFactory : public ObjectFactory
{
public:
    BedOfNailsObjectFactory(void)
    {
        announceObjectType("BedOfNails", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new BedOfNails());
    }
};
static BedOfNailsObjectFactory theBedOfNailsObjectFactory;


BedOfNails::BedOfNails()
    : PolyObject(QObject::tr("Bed of nails"),
                 QObject::tr("A wooden board attached to the scene.\nIt has many sharp nails on one side."),
                 "BedOfNails",
                 // first the bar:
                 "(-0.4,-0.075)=(0.4,-0.075)=(0.4,0.006)=(-0.4,0.006);"
                 // then the nails block:
                 "(-0.34,-0.05)=(0.34,-0.05)=(0.34,0.06)=(-0.34,0.06)"
                 ,
                 0.8, 0.15, 0.0, 0.2)
{
}

BedOfNails::~BedOfNails()
{
}

void BedOfNails::callBackSensor(const ContactInfo &aPoint)
{
    AbstractObject *myOtherObject = nullptr;

    // which one of the two shapes is not me?
    if (aPoint.myFixtureA->GetUserData() == this)
        myOtherObject = aPoint.myFixtureB->GetUserData();
    if (aPoint.myFixtureB->GetUserData() == this)
        myOtherObject = aPoint.myFixtureA->GetUserData();
    if (myOtherObject == nullptr)
        return;

    // try to sting it!
    myOtherObject->causeWounded(AbstractObject::STINGING);
}

void BedOfNails::fillShapeList(void)
{
    PolyObject::fillShapeList();

    // And add the sensor to the shapes
    b2PolygonShape *mySensorShapePtr = new b2PolygonShape();
    b2Vec2 mySensorShapeV[4];
    mySensorShapeV[0].Set(-0.335, -0.04);
    mySensorShapeV[1].Set( 0.335, -0.04);
    mySensorShapeV[2].Set( 0.335,  0.07);
    mySensorShapeV[3].Set(-0.335,  0.07);
    mySensorShapePtr->Set(mySensorShapeV, 4);
    b2FixtureDef *mySensorDef = new b2FixtureDef();
    mySensorDef->shape    = mySensorShapePtr;
    mySensorDef->isSensor = true;
    mySensorDef->userData = this;
    theShapeList.push_back(mySensorDef);
}

///---------------------------------------------------------------------------
///-------------------------- CircularSaw ------------------------------------
///---------------------------------------------------------------------------

const static double CIRCRADIUS = 0.19;
const static double CIRCMASS   = 0.4;

// this class' ObjectFactory
class CircularSawObjectFactory : public ObjectFactory
{
public:
    CircularSawObjectFactory(void)
    {
        announceObjectType("CircularSaw", this);
    }
    AbstractObject *createObject(void) const
    override
    {
        return fixObject(new CircularSaw());
    }
};
static CircularSawObjectFactory theCircularSawObjectFactory;

CircularSaw::CircularSaw()
    : CircleObject(QObject::tr("Circular saw"),
                   QObject::tr("A rotating disc with sharp teeth."),
                   "CircularSaw",
                   CIRCRADIUS, CIRCMASS, 0.1)
{
    theProps.setDefaultPropertiesString(
        Property::IMAGE_NAME_STRING + QString(":CircularSaw/") +
        Property::RADIUS_STRING + ":" + QString::number(CIRCRADIUS) + "/" +
        Property::MASS_STRING + ":" + QString::number(CIRCMASS) + "/" );
}

CircularSaw::~CircularSaw()
{
}

void CircularSaw::callBackSensor(const ContactInfo &aPoint)
{
    AbstractObject *myOtherObject = nullptr;

    // which one of the two shapes is not me?
    if (aPoint.myFixtureA->GetUserData() == this)
        myOtherObject = aPoint.myFixtureB->GetUserData();
    if (aPoint.myFixtureB->GetUserData() == this)
        myOtherObject = aPoint.myFixtureA->GetUserData();
    if (myOtherObject == nullptr)
        return;

    // try to sting it!
    myOtherObject->causeWounded(AbstractObject::SLICING);
}


void CircularSaw::createBallShapeFixture(float aRadius, float aMass)
{
    // add the original ball shape
    CircleObject::createBallShapeFixture(aRadius, aMass);

    // and add our sensor
    b2CircleShape *ballShape = new b2CircleShape();
    ballShape->m_radius = aRadius + 0.01;

    b2FixtureDef *ballFixDef = new b2FixtureDef();
    ballFixDef->density = 0;
    ballFixDef->userData = this;
    ballFixDef->shape = ballShape;
    ballFixDef->isSensor = true;

    theShapeList.push_back(ballFixDef);
}

void  CircularSaw::parseProperties(void)
{
    DEBUG5ENTRY;
    CircleObject::parseProperties();

    float myRadius = CIRCRADIUS;
    float myMass = CIRCMASS;
    theProps.property2Float(Property::RADIUS_STRING, &myRadius);
    theProps.property2Float(Property::MASS_STRING, &myMass);
    if (myRadius <= Position::minimalMove)
        myRadius = 0.1;

    createBallShapeFixture(myRadius, myMass);
}
