/* The Butterfly Effect
 * This file copyright (C) 2010,2013,2014  Klaas van Gend
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

#include "TriggerExplosion.h"
#include "tbe_global.h"
#include "ViewDetonatorBox.h"

#include <cassert>

//// the DetonatorBox class' ObjectFactory
class DetonatorBoxObjectFactory : public ObjectFactory
{
public:
	DetonatorBoxObjectFactory(void)
	{	announceObjectType("DetonatorBox", this); }
    AbstractObject* createObject(void) const override
    {	return fixObject(new DetonatorBox()); }
};
static DetonatorBoxObjectFactory theDetonatorBoxObjectFactory;


const qreal  DetonatorBox::ACTIVATED_TIME = 0.5;
const qreal  DetonatorBox::RINGING_TIME   = 0.5;
const qreal  STARTDISTANCE=0.2f;
const Vector DetonatorBox::HANDLEOFFSET   = Vector(0,STARTDISTANCE);

DetonatorBox::DetonatorBox()
    :	RectObject( QObject::tr("Detonator Box"),
                    "",
                    "DetonatorBoxDone;DetonatorBoxActivated;DetonatorBoxRinging;DetonatorBoxDone",
                    0.33, 0.35, 4.0, 0.0),
      theState(ARMED),
      isTriggered(false),
      theActivationStartTime(999.f),
      theHandle(nullptr)
{
	theProps.setDefaultPropertiesString(
				Property::PHONENUMBER_STRING + QString(":/") );
	thePhoneNumber = "";
}

DetonatorBox::~DetonatorBox()
{
	deletePhysicsObject();
}

void DetonatorBox::callbackStep (qreal /*aTimeStep*/, qreal aTotalTime)
{
	switch(theState)
	{
	case ARMED:
		if (isTriggered)
		{
			goToState(ACTIVATED);
			theActivationStartTime=aTotalTime;
		}
		break;
	case ACTIVATED:
		if (aTotalTime > theActivationStartTime + ACTIVATED_TIME)
		{
			goToState(RINGING);
		}
		break;
	case RINGING:
		if (aTotalTime > theActivationStartTime + ACTIVATED_TIME + RINGING_TIME)
		{
			notifyExplosions();
			goToState(DONE);
		}
		break;
	case DONE:
		break;
	}
}

ViewObject*  DetonatorBox::createViewObject(float aDefaultDepth)
{
    if (theViewObjectPtr!=NULL)
		return theViewObjectPtr;

	QString myImageName;
	if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true)==false)
		myImageName = getName();

    theViewObjectPtr = new ViewDetonatorBox(getThisPtr(), myImageName);
	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	updateViewObject(false);

	return theViewObjectPtr;
}

void DetonatorBox::createPhysicsObject(void)
{
	RectObject::createPhysicsObject();
    theHandle->createPhysicsObject();

	isTriggered = false;
	theActivationStartTime = 0.0f;
	theState = ARMED;
	theWorldPtr->registerCallback(this);
}

void DetonatorBox::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
    theHandle->deletePhysicsObject();
}

QStringList DetonatorBox::getAllPhoneNumbers(void)
{
	QRegExp myRX("^\\d.*");
	assert(theWorldPtr!=NULL);
	QStringList myList = theWorldPtr->getAllIDs().filter(myRX);
	if (myList.contains(getCurrentPhoneNumber())==false)
		myList.append(getCurrentPhoneNumber());
	myList.append(getEmptyString());
	return myList;
}

QString DetonatorBox::getCurrentPhoneNumber(void) const
{
	QString myPhoneNumber = thePhoneNumber;
	if (myPhoneNumber.isEmpty())
		theProps.property2String(Property::PHONENUMBER_STRING, &myPhoneNumber, true);
	if (myPhoneNumber.isEmpty() || myPhoneNumber.isNull())
		myPhoneNumber = getEmptyString();
	return myPhoneNumber;
}

QString DetonatorBox::getEmptyString() const
{
	return QObject::tr("(empty)");
}


const QString DetonatorBox::getToolTip ( ) const
{
	//: Translators: The %1 will be replaced by a phone number.
	return QObject::tr("Send BOOM to %1").arg(getCurrentPhoneNumber());
}

qreal DetonatorBox::getZValue(void)
{
	return theViewObjectPtr->zValue();
}


DetonatorBox::States DetonatorBox::goToState(DetonatorBox::States aNewState)
{
	DEBUG4("DetonatorBox from state %d to state %d", theState, aNewState);
	theState = aNewState;
	return theState;
}

void DetonatorBox::notifyExplosions(void)
{
    AbstractObjectPtr myObjectToSignal = theWorldPtr->findObjectByID(getCurrentPhoneNumber());
    Dynamite* myDynamite = dynamic_cast<Dynamite*>(myObjectToSignal.get());

	// did the user select a wrong phone number?
	if (myDynamite == NULL)
		return;
	myDynamite->trigger();
}


void  DetonatorBox::registerChildObjects(void)
{
    theHandle = ObjectFactory::createChildObject<DetonatorBoxHandle>(this);
    assert(theWorldPtr);
    theWorldPtr->addObject(theHandle);
    setOrigCenter(getOrigCenter());
    theWorldPtr->addNoCollisionCombo(getThisPtr(),theHandle);
}

void DetonatorBox::setOrigCenter ( const Position& aNewPos )
{
    RectObject::setOrigCenter(aNewPos);

    if (theHandle)
    {
        theHandle->setOrigCenter(getOrigCenter()+1.4*HANDLEOFFSET);
        theHandle->updateViewObject(false);
    }
}

void DetonatorBox::setTriggered(void)
{
	// let's just be nice asynchronously
	isTriggered = true;
}


void DetonatorBox::updateViewObject(bool isSimRunning) const
{
    RectObject::updateViewObject(isSimRunning);
    if (!isSimRunning)
        if (theHandle)
            theHandle->updateViewObject(false);
}


// ##########################################################################
// ##########################################################################
// ##########################################################################

DetonatorBoxHandle::DetonatorBoxHandle(DetonatorBox* aDBox)
	:	RectObject( QObject::tr("Detonator Box Handle"),
					QObject::tr("Push Here To BOOM"),
					"DetonatorBoxHandle",
                    0.25, 0.20, 0.1, 0.0), theDBoxPtr(aDBox), theJointPtr(NULL)
{
	setOrigCenter(Position(0,0));
    theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

DetonatorBoxHandle::~DetonatorBoxHandle()
{
//	theWorldPtr->unregisterCallback(this);
}

void DetonatorBoxHandle::callbackStep (qreal /*aTimeStep*/, qreal /*aTotalTime*/)
{
	// if down, stay down and signal the box
	if (theJointPtr->GetJointTranslation() <= theJointPtr->GetLowerLimit())
	{
		theJointPtr->SetMaxMotorForce(0);
		theDBoxPtr->setTriggered();
	}
}

ViewObject*  DetonatorBoxHandle::createViewObject(float aZ)
{
	RectObject::createViewObject(aZ);
	setViewObjectZValue(theDBoxPtr->getZValue()/1.1);
	return theViewObjectPtr;
}

void DetonatorBoxHandle::createPhysicsObject(void)
{
	if (isPhysicsObjectCreated())
		return;
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
	theWorldPtr->registerCallback(this);
}

void DetonatorBoxHandle::deletePhysicsObject(void)
{
	RectObject::deletePhysicsObject();
	// the deletePhysics will already kill the joint, thanks to Box2D.
	theJointPtr = NULL;
}

qreal DetonatorBoxHandle::getDistance(void)
{
	if (theJointPtr==NULL)
		return 0;
	return theJointPtr->GetJointTranslation();
}

// ##########################################################################
// ##########################################################################
// ##########################################################################


//// the Dynamite class' ObjectFactory
class DynamiteObjectFactory : public ObjectFactory
{
public:
	DynamiteObjectFactory(void)
	{	announceObjectType("Dynamite", this); }
    AbstractObject* createObject(void) const override
	{	return fixObject(new Dynamite()); }
};
static DynamiteObjectFactory theDynamiteObjectFactory;

const qreal  Dynamite::RINGING_TIME   = 0.2;
const qreal  Dynamite::DYNAMITE_MASS  = 0.8;

Dynamite::Dynamite()
	: PolyObject(QObject::tr("Dynamite"),
				 "",
				 "Dynamite;DynamiteActive;DynamiteRinging;DynamiteBoom;Empty",
				 "(-0.215,-0.16)=(0.215,-0.16)=(0.215,0.02)=(-0.15,0.16)=(-0.215,0.16)",
				 0.43, 0.32, DYNAMITE_MASS, 0.1),
      theState(WAITING),
      theTrigger(false),
      theActiveStartTime(0.0f),
      theBoomStartTime(999.f)
{
}

Dynamite::~Dynamite()
{
}

void Dynamite::callbackStep (qreal /*aTimeStep*/, qreal aTotalTime)
{
	switch(theState)
	{
	case WAITING:
		if (theTrigger)
		{
			goToState(ACTIVE);
			theActiveStartTime=aTotalTime;
		}
		break;
	case ACTIVE:
		if (aTotalTime > theActiveStartTime + RINGING_TIME)
		{
			goToState(RINGING);
		}
		break;
	case RINGING:
		if (aTotalTime > theActiveStartTime + 2*RINGING_TIME)
		{
			theBoomStartTime = aTotalTime;
			goToState(BOOM);
		}
		break;
	case BOOM:
		manageParticles(aTotalTime-theBoomStartTime);
		if (aTotalTime > theActiveStartTime + 3*RINGING_TIME)
		{
			goToState(GONE);
		}
		break;
	case GONE:
		manageParticles(aTotalTime-theBoomStartTime);
		break;
	}
}

void Dynamite::createPhysicsObject(void)
{
	PolyObject::createPhysicsObject();

	clearShapeList();
	fillShapeList();

	theWorldPtr->registerCallback(this);
	theActiveStartTime = 0.0f;
	theState = WAITING;

	theTrigger = false;
}

void Dynamite::deletePhysicsObject(void)
{
	theSplatterList.clear();
	// nothing much to do here that actually has to do with delete...
	theState = WAITING;
	clearShapeList();
	fillShapeList();
	PolyObject::deletePhysicsObject();
}

void Dynamite::deletePhysicsObjectForReal(void)
{
	updateViewObject(false);
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = NULL;
}


void Dynamite::explode(void)
{
	const int NUM_SPLATS = 13;
	for (int i=0; i< NUM_SPLATS; i++)
	{
        std::shared_ptr<ExplosionSplatter> mySplatterPtr =
                ObjectFactory::createChildObject<ExplosionSplatter>();

		// startpos = dynamite center + angle + distance from center
		Position myStart = getTempCenter();
		myStart.angle += 2*PI*(float)i/NUM_SPLATS;
		myStart = myStart + Vector(1.3*ExplosionSplatter::theRadius,0);

		// HACK HACK: A regular detonation front is close to sonic speed (1000 m/s)
		// we're not going to go anywhere near that speed here...
        mySplatterPtr->setAll(theWorldPtr, myStart, 20.0, DYNAMITE_MASS/NUM_SPLATS, this);
        theSplatterList.push_back(mySplatterPtr);
	}
}

const QString Dynamite::getToolTip ( ) const
{
	//: Translators: the \n means "newline" - keep it. The %1 will be replaced by a phone number
	return QObject::tr("Dynamite: invented by Alfred Nobel. \n Dial %1 for a nice explosion.").arg(getID());
}

Dynamite::States Dynamite::goToState(Dynamite::States aNewState)
{
	DEBUG4("Dynamite from state %d to state %d", theState, aNewState);

	if (theState == RINGING && aNewState == BOOM)
	{
		explode();
		deletePhysicsObjectForReal();
	}

	theState = aNewState;
	return theState;
}

void Dynamite::manageParticles(float aDeltaTime)
{
	int mySplattersLeft = theSplatterList.count();
	if (mySplattersLeft > 0)
	{
		// the following was tuned for full speed with aTimeStep equals 25 frames i.e. 0.040 s.
		//
		// The perimeter of the detonation front is linearly increasing,
		// (perimeter of a circle is 2*pi*radius, remember?)
		// so the power of the explosion decays linearly.
		//
		// let's assume the decay is complete after 2 seconds, i.e. the mass of
		// the total number of splatters left at that point is reduced to zero
		const float DECAY_TIME = 1.5;

		float myMassLeft = DYNAMITE_MASS*(DECAY_TIME-aDeltaTime)/DECAY_TIME;
		if (myMassLeft < 0.0)
		{
			while(theSplatterList.count() > 0)
			{
                AbstractObjectPtr myP = theSplatterList.last();
				theSplatterList.pop_back();
				theWorldPtr->removeObject(myP);
            }
		}
		else
            foreach(AbstractObjectPtr e, theSplatterList)
            {
                ExplosionSplatter* p = dynamic_cast<ExplosionSplatter*>(e.get());
                assert(p);
                p->setMass( myMassLeft / mySplattersLeft );
            }
	}
}

void Dynamite::removeMe(AbstractObjectPtr aDeadSplatterPtr)
{
	assert(aDeadSplatterPtr != NULL);
	theSplatterList.removeAll(aDeadSplatterPtr);
}

// ##########################################################################
// ##########################################################################
// ##########################################################################

const qreal ExplosionSplatter::theRadius = 0.04;
const int   ExplosionSplatter::COLLISION_GROUP_INDEX = 3;

// note that the mass will be redone during setAll()
ExplosionSplatter::ExplosionSplatter()
    : CircleObject("ExplosionSplatter","", "ColaSplatter",
                   theRadius, 0.001,  1.0),
      theDynamitePtr(NULL)
{
	DEBUG5("ExplosionSplatter::ExplosionSplatter");

	// the first shape is already set in the AbstractBall constructor
	// we only need to set the groupIndex so the various splatters do not collide
	theShapeList[0]->filter.groupIndex = - COLLISION_GROUP_INDEX;
}

ExplosionSplatter::~ExplosionSplatter()
{
	DEBUG5("ExplosionSplatter::~ExplosionSplatter()");
	// contrary to most objects, we need to take ourselves really out of the
	// physics simulation...
	getB2WorldPtr()->DestroyBody(theB2BodyPtr);
}


void ExplosionSplatter::reportNormalImpulseLength(qreal)
{
	// oww we hit something.
	// that may be the end for us
	//   - depending on whether we are reversing on our path or not

	// FIXME/TODO: technically speaking, we should check if we hit
	// a static/kinetic or a dynamic object.
	//   - In case of static/kinetic: 100% bounce
	//   - In case of dynamic: stick to it so we can propel it properly
	//
	// but that's significantly more complex than this...
	//
	// IDEA: can we vary bounciness during collision detection for
	// different types of objects?

	// compare against theStartVelocityVector
	b2Vec2 myVelocity = theB2BodyPtr->GetLinearVelocity();

	if (((myVelocity.x>=0) != (theStartVelocityVector.dx>=0)) &&
			((myVelocity.y>=0) != (theStartVelocityVector.dy>=0)))
	{
        theDynamitePtr->removeMe(getThisPtr());
        theWorldPtr->removeMe(getThisPtr(), 0.05);
	}
}


void ExplosionSplatter::setAll(World* aWorldPtr,
                               const Position& aStartPos,
                               qreal aVelocity,
                               qreal aSplatterMass,
                               Dynamite* aDynamitePtr)
{
	DEBUG5("ExplosionSplatter::setAll(%p, (%f,%f,%f), %f, %f)",
		   aWorldPtr, aStartPos.x, aStartPos.y, aStartPos.angle,
		   aVelocity, aSplatterMass);

    theDynamitePtr = aDynamitePtr;
    setOrigCenter(aStartPos);
	createPhysicsObject();

	qreal myAngle = aStartPos.angle;
	theStartVelocityVector = Vector(aVelocity * cos(myAngle), aVelocity * sin(myAngle));
	theB2BodyPtr->SetLinearVelocity(theStartVelocityVector.toB2Vec2());
	setMass(aSplatterMass);

    aWorldPtr->addObject(getThisPtr());
}

void ExplosionSplatter::setMass( qreal aSplatterMass )
{
	b2MassData myMD;
	myMD.mass = aSplatterMass;
	myMD.center = b2Vec2(0,0);
	myMD.I = 0.0008 * aSplatterMass;
	theB2BodyPtr->SetMassData(&myMD);
}
