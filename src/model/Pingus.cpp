/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#include "Pingus.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "Property.h"
#include "ViewPingus.h"

static const qreal FALLING_TIME   = 0.16; // seconds
static const qreal PINGUS_RADIUS  = 0.14; // m
static const qreal PINGUS_MASS    = 1.50; // kg
static const qreal SPLATTING_IMPULSE = 7.0; // ###
static const qreal SPLATTING_SPF  = 0.02; // seconds per frame
static const qreal WAITING_SPF    = 0.06; // second per frame
static const qreal WAITING_SPEED  = 0.002; // m/s  (i.e. 2 mm/s)
static const qreal WALKING_SPEED  = 0.50; // m/s
static const qreal WALKINGSEQS_PER_SECOND =
    1.5; // 1.5 sequences per horizontal distance of [walking speed*1second]

const unsigned int Pingus::FramesPerState[] = { 8, 8, 8, 1, 1, 16, 6, 9, 9, 1, 5, 1};
int Pingus::theEscapedCount = 0;
int Pingus::theAliveCount = 0;

Pingus::Pingus(const QString &anIconName)
    : CircleObject(QObject::tr("Pingus"),
                   QObject::tr("A penguin walks left or right and turns around when\nit collides with something heavy. It can push\nlight objects around. It also likes to slide down\nslopes but can't take much abuse."),
                   "",
                   PINGUS_RADIUS, PINGUS_MASS, 0.0 ), theIconName(anIconName), theState(FALLING), theAnimationFrameIndex(0)
{
    updateViewPingus();
}


Pingus::~Pingus()
{
}


void Pingus::callbackStep (qreal aDeltaTime, qreal aTotalTime)
{
    DEBUG6("Pingus %p receives callback", this);
    if (isPhysicsObjectCreated() == false)
        return;

    // based on X velocity, determine the right state
    States myNewXSuggestion = WAITING;
    qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
    if (fabs(myXd) > WAITING_SPEED) {
        if (fabs(myXd) > WALKING_SPEED) {
            if (myXd > 0)
                myNewXSuggestion = SLIDERIGHT;
            else
                myNewXSuggestion = SLIDELEFT;
        } else {
            if (myXd > 0)
                myNewXSuggestion = WALKINGRIGHT;
            else
                myNewXSuggestion = WALKINGLEFT;
        }
    }
    else
    {
        if (aTotalTime < 0.5)
            myNewXSuggestion = getStartDirection();
    }

    // based on Y velocity, determine the right state
    // if we're falling *and* there's no LastNormalImpulse
    // *and* there's still a Yd, we're still falling
    // if we're not falling and there's a too big Yd, we start falling
    qreal myYd = theB2BodyPtr->GetLinearVelocity().y;


//printf("Pingus %p myXd:%f myYd:%f normalimpulse: %f, state %d\n",
//     this, myXd, myYd, theLastNormalImpulseReported, theState);
    // do we need to switch state?
    switch (theState) {
    case WALKINGLEFT:
    case WALKINGRIGHT:
    case SLIDELEFT:
    case SLIDERIGHT:
    case WAITING:
    case SLEEPING:
        if (theLastNormalImpulseReported < 0.02)
            goToState(FALLING);
        else
            goToState(myNewXSuggestion); // nothing happens when going to same state
        break;
    case FALLING:
        if (theLastNormalImpulseReported > 0.02 || fabs(myYd) < WAITING_SPEED)
            goToState(myNewXSuggestion);
        break;
    case SPLATTING:
    case EXITINGLEFT:
    case EXITINGRIGHT:
    case DIDEXIT:
    case DEAD:
        // X velocities are not going to affect us, nothing to do
        break;
    }

    switch (theState) {
    case WALKINGLEFT:
    case WALKINGRIGHT:
        callbackStepWalking(aDeltaTime, aTotalTime);
        break;
    case FALLING:
        callbackStepFalling(aDeltaTime, aTotalTime);
        break;
    case SLIDELEFT:
    case SLIDERIGHT:
        callbackStepSliding(aDeltaTime, aTotalTime);
        break;
    case EXITINGLEFT:
    case EXITINGRIGHT:
    case SPLATTING:
        callbackStepSplatting(aDeltaTime, aTotalTime);
        break;
    case WAITING:
    case SLEEPING:
        callbackStepWaiting(aDeltaTime, aTotalTime);
        break;
    case DIDEXIT:
    case DEAD:
        // nothing to do
        break;
    }

    updateViewPingus();
    // decay last reported impulses
    theLastNormalImpulseReported /= 2.0;
}


void Pingus::callbackStepFalling(qreal, qreal aTotalTime)
{
    // is this the first callback in Falling state???
    if (theFallingTimeStart < 0)
        theFallingTimeStart = aTotalTime;

    // calculate the frame to display
    // the falling sequence has 8 images, let's take 0.16s to display them
    // (at 60fps, that means approx 2 displayframes for each animation frame)
    // note that contrary to Splatting, we do repeat the sequence
    theAnimationFrameIndex = Pingus::FramesPerState[FALLING] *
                             fmodf(aTotalTime - theFallingTimeStart, FALLING_TIME) / FALLING_TIME;
}


void Pingus::callbackStepSliding(qreal aTimeStep, qreal)
{
    // Add a bit of friction against the velocity vector
    qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
    Q_ASSERT (fabs(myXd) > WALKING_SPEED);

    qreal myXImpulse = -copysign(0.8, myXd);
    Vector myTotXImpulse = aTimeStep * Vector(myXImpulse, 0.0);
    theB2BodyPtr->ApplyLinearImpulse(
        myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);
}


void Pingus::callbackStepSplatting(qreal, qreal aTotalTime)
{
    // is this the first callback in Splatting/Exiting state???
    if (theSplattingTimeStart < 0) {
        theSplattingTimeStart = aTotalTime;
    }

    // calculate the frame to display
    // the splatting sequence has 16 images, the exiting sequences have 9
    // we display at 0.06 seconds per frame, calculate the frame to display
    theAnimationFrameIndex = (aTotalTime - theSplattingTimeStart) / SPLATTING_SPF;

    // are we done?
    if (theAnimationFrameIndex >= Pingus::FramesPerState[theState]) {
        theAnimationFrameIndex = 0;
        if (theState == SPLATTING)
            goToState(DEAD);
        else
            goToState(DIDEXIT);
    }
}


// used for Waiting and Sleeping
void Pingus::callbackStepWaiting(qreal aTimeStep, qreal aTotalTime)
{
    // First calculate the new animation frame

    // is this the first callback in Waiting state???
    if (theWaitingTimeStart < 0)
        theWaitingTimeStart = aTotalTime;

    // calculate the frame to display
    theAnimationFrameIndex = (aTotalTime - theWaitingTimeStart) / WAITING_SPF;
    // vary horizontally/vertically in case multiple pingus wait or sleep together
    theAnimationFrameIndex += theB2BodyPtr->GetPosition().x * 91.;
    theAnimationFrameIndex += theB2BodyPtr->GetPosition().y * 91.;
    theAnimationFrameIndex %= Pingus::FramesPerState[theState];

    // if we're sleeping, we're done now
    if (SLEEPING == theState)
        return;

    qreal aSign = (getStartDirection()==WALKINGLEFT)?-1.0:1.0;

    // If the Penguin is watching right, let's nudge him and see if it makes hime move
    if (theAnimationFrameIndex == 3) {
        Vector myTotXImpulse = aTimeStep * aSign * Vector(5, 0);
        theB2BodyPtr->ApplyLinearImpulse(
            myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);
    }
    // If the Penguin is watching left, let's nudge him and see if it makes hime move
    if (theAnimationFrameIndex == 0) {
        Vector myTotXImpulse = aTimeStep * aSign * Vector(-5, 0);
        theB2BodyPtr->ApplyLinearImpulse(
            myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);
    }
}


void Pingus::callbackStepWalking(qreal aTimeStep, qreal)
{
    // * check for the horizontal speed and adjust it if needed
    //   by applying a force
    // Note that this is just the P-action of a PID-controller, so there's
    // guaranteed no overshoot as myXd will never reach the WALKING_SPEED.
    // With the current settings it remains 0.0003 m/s below it.
    qreal myXd = theB2BodyPtr->GetLinearVelocity().x;
    Q_ASSERT (fabs(myXd) < WALKING_SPEED);
    qreal myXImpulse = copysign(5.0 * (WALKING_SPEED - fabs(myXd)) / WALKING_SPEED, (theState==WALKINGLEFT)?-1.0:1.0);
    Vector myTotXImpulse = aTimeStep * Vector(myXImpulse, 0);
    theB2BodyPtr->ApplyLinearImpulse(
        myTotXImpulse.toB2Vec2(), getTempCenter().toB2Vec2(), true);

    // * set theAnimationFrameIndex to the horizontal position
    // in WALKING_SPEED [m/s], we have Pingus::FramesPerState[WALKINGLEFT] animation frames to draw
    qreal temp = fmodf(theB2BodyPtr->GetPosition().x, WALKING_SPEED / WALKINGSEQS_PER_SECOND);
    temp /= WALKING_SPEED / WALKINGSEQS_PER_SECOND;
    theAnimationFrameIndex = temp * static_cast<qreal>(Pingus::FramesPerState[WALKINGLEFT]);
}


void Pingus::causeWounded(AbstractObject::WhyWounded aReason)
{
    switch (aReason) {
    case NOTHING:
    case STINGING:
    case WET:
    case EXPLOSION: // (penguin will already die from the impact if too close)
        // do nothing: no issue
        break;
    case SLICING:
    case POISON:
    case HEAT:
        goToState(SPLATTING);
        break;
    }
}


void Pingus::createPhysicsObject()
{
    resetParameters();
    createBallShapeFixture(PINGUS_RADIUS, PINGUS_MASS);
    CircleObject::createPhysicsObject();
    theWorldPtr->registerCallback(this);
    theAliveCount++;
}


ViewObjectPtr  Pingus::createViewObject(float aDefaultDepth)
{
    if (nullptr != theViewObjectPtr)
        return theViewObjectPtr;
    theViewObjectPtr = ViewObject::factoryMethod<ViewPingus>(getThisPtr(), theIconName);
    setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
    updateViewPingus();
    return theViewObjectPtr;
}


void Pingus::deletePhysicsObject()
{
    // nothing much to do here that actually has to do with delete...
    resetParameters();
    clearShapeList();

    CircleObject::deletePhysicsObject();
}

void Pingus::deletePhysicsObjectForReal()
{
    updateViewObject(false);
    getB2WorldPtr()->DestroyBody(theB2BodyPtr);
    theB2BodyPtr = nullptr;
}


Pingus::States Pingus::goToState(Pingus::States aNewState)
{
    // do not go from state A to state A
    if (aNewState == theState)
        return theState;

    Pingus::States myOldState = theState;

    // if we're not yet splatting or dead, we're splatting!
    if (aNewState == SPLATTING) {
        if (theState != DEAD)
            theState = SPLATTING;
    } else {
        // we're not going to be splatting... what do we need to do?
        switch (theState) {
        case WALKINGLEFT:
        case WALKINGRIGHT:
        case SLIDELEFT:
        case SLIDERIGHT:
            theState = aNewState;
            break;
        case FALLING:
            theFallingTimeStart = -1.0;
            theState = aNewState;
            break;
        case EXITINGLEFT:
        case EXITINGRIGHT:
        case SPLATTING:
            // it should not be possible to get out of splatting, other than dead
            if (DEAD == aNewState) {
                theState = DEAD;
                theAliveCount--;
                deletePhysicsObjectForReal();
            }
            if (DIDEXIT == aNewState) {
                theState = DIDEXIT;
                theEscapedCount++;
                deletePhysicsObjectForReal();
            }
            break;
        case WAITING:
        case SLEEPING:
            // prevent switching between waiting and sleeping
            if (WAITING != aNewState && SLEEPING != aNewState) {
                theWaitingTimeStart = -1.0;
                theState = aNewState;
            }
            break;
        case DEAD:
        case DIDEXIT:
            // nothing to be done here...
            break;
        }
    }
    DEBUG5("Pingus %p change state request from %d to %d %s.",
           this, myOldState, aNewState, (theState == myOldState) ? "DENIED" : "approved");
    theAnimationFrameIndex = 0;
    return theState;
}


void Pingus::reportNormalImpulseLength(qreal anImpulseLength, AbstractObject *)
{
    // pop the Pingus if it is maltreated
    // switch between falling and walking/sliding where appropriate
    if (anImpulseLength > SPLATTING_IMPULSE) {
        goToState(SPLATTING);
    }

    if (anImpulseLength > theLastNormalImpulseReported)
        theLastNormalImpulseReported = anImpulseLength;
    // When Walking/Sliding/Waiting, we expect an impulselength of approx 0.05-0.06
}


void Pingus::resetParameters()
{
    theState = getStartDirection();
    theAnimationFrameIndex = 0;
    theFallingTimeStart   = -1.0;
    theSplattingTimeStart = -1.0;
    theWaitingTimeStart   = -1.0;
    theLastNormalImpulseReported = 0.0;
    updateViewPingus();
}


void Pingus::resetPingusCount()
{
    theAliveCount = 0;
    theEscapedCount = 0;
}


void Pingus::startYourExit()
{
    switch (theState) {
    case WALKINGLEFT:
    case SLIDELEFT:
        goToState(EXITINGLEFT);
        break;
    case EXITINGLEFT:
    case EXITINGRIGHT:
    case DIDEXIT:
    case DEAD:
        // do not do anything
        break;
    default:
        goToState(EXITINGRIGHT);
        break;
    }
}


void Pingus::updateViewPingus()
{
    // We know for sure that for the Pingus, this will be a ViewPingus,
    // so no need to use RTTI's dynamic_cast<ViewPingus*> here...
    ViewPingus *theVPPtr = static_cast<ViewPingus *>(theViewObjectPtr.data());
    if (nullptr != theVPPtr)
        theVPPtr->setNewAnimationFrame(theState, theAnimationFrameIndex);
}


///---------------------------------------------------------------------------
///------------------------- PingusSleeping ----------------------------------
///---------------------------------------------------------------------------

//// this class' ObjectFactory
class PingusSleepingObjectFactory : public ObjectFactory
{
public:
    PingusSleepingObjectFactory()
    {
        announceObjectType("PingusSleeping", this);
    }
    AbstractObject *createObject() const override
    {
        return fixObject(new PingusSleeping());
    }
};
static PingusSleepingObjectFactory thePingusSleepingObjectFactory;


PingusSleeping::PingusSleeping()
    : Pingus("pingussleeper")
{
}


PingusSleeping::~PingusSleeping()
{
}


///---------------------------------------------------------------------------
///------------------------- PingusWalkLeft ----------------------------------
///---------------------------------------------------------------------------

//// this class' ObjectFactory
class PingusWalkLeftObjectFactory : public ObjectFactory
{
public:
    PingusWalkLeftObjectFactory()
    {
        announceObjectType("PingusWalkLeft", this);
    }
    AbstractObject *createObject() const override
    {
        return fixObject(new PingusWalkLeft());
    }
};
static PingusWalkLeftObjectFactory thePingusWalkLeftObjectFactory;


PingusWalkLeft::PingusWalkLeft()
    : Pingus("pinguswalkleft")
{
}


PingusWalkLeft::~PingusWalkLeft()
{
}


///---------------------------------------------------------------------------
///------------------------- PingusWalkRight ----------------------------------
///---------------------------------------------------------------------------

//// this class' ObjectFactory
class PingusWalkRightObjectFactory : public ObjectFactory
{
public:
    PingusWalkRightObjectFactory()
    {
        announceObjectType("PingusWalkRight", this);
    }
    AbstractObject *createObject() const override
    {
        return fixObject(new PingusWalkRight());
    }
};
static PingusWalkRightObjectFactory thePingusWalkRightObjectFactory;


PingusWalkRight::PingusWalkRight()
    : Pingus("pinguswalkright")
{
    resetParameters();
}


PingusWalkRight::~PingusWalkRight()
{
}


///---------------------------------------------------------------------------
///---------------------------- PingusExit -----------------------------------
///---------------------------------------------------------------------------

//// this class' ObjectFactory
class PingusExitObjectFactory : public ObjectFactory
{
public:
    PingusExitObjectFactory()
    {
        announceObjectType("PingusExit", this);
    }
    AbstractObject *createObject() const override
    {
        return fixObject(new PingusExit());
    }
};
static PingusExitObjectFactory thePingusExitObjectFactory;


PingusExit::PingusExit()
{
    // We are just a sensor
    b2PolygonShape *mySensorShapePtr = new b2PolygonShape();
    b2Vec2 mySensorShapeV[4];
    mySensorShapeV[0].Set(-0.05, -0.05);
    mySensorShapeV[1].Set( 0.05, -0.05);
    mySensorShapeV[2].Set( 0.05,  0.05);
    mySensorShapeV[3].Set(-0.05,  0.05);
    mySensorShapePtr->Set(mySensorShapeV, 4);
    b2FixtureDef *mySensorDef = new b2FixtureDef();
    mySensorDef->shape    = mySensorShapePtr;
    mySensorDef->isSensor = true;
    mySensorDef->userData = this;
    theShapeList.push_back(mySensorDef);
    theProps.setDefaultPropertiesString(
        Property::ZVALUE_STRING + QString(":0.5/") );
}


PingusExit::~PingusExit()
{
}


void PingusExit::callBackSensor(const ContactInfo &aPoint)
{
    AbstractObject *myOtherObject = nullptr;

    // which one of the two shapes is not me?
    if (aPoint.myFixtureA->GetUserData() == this)
        myOtherObject = aPoint.myFixtureB->GetUserData();
    if (aPoint.myFixtureB->GetUserData() == this)
        myOtherObject = aPoint.myFixtureA->GetUserData();
    if (nullptr == myOtherObject)
        return;

    // is it a Pingus?
    // then tell it to exit!
    Pingus *myPingusPtr = dynamic_cast<Pingus *>(myOtherObject);
    if (nullptr != myPingusPtr)
        myPingusPtr->startYourExit();
}
