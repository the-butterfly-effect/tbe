/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#include "AbstractObject.h"
#include "AbstractObjectSerializer.h"
#include "Box2D.h"
#include "PivotPoint.h"
#include "TranslationGuide.h"
#include "ViewObject.h"
#include "World.h"

#include <QStringList>
#include <cassert>

// I wonder if this should be b2_linearSlop instead of this number...
const float AbstractObject::MINIMUM_DIMENSION = 0.03;

// Static variables
static b2World* theStaticB2WorldPtr = nullptr;


AbstractObject::AbstractObject()
    : theB2BodyPtr(nullptr),
	  hasCustomToolTip(false),
	  theViewObjectPtr(nullptr),
      theChildPivotPointPtr(nullptr),
      theChildTranslationGuidePtr(nullptr),
	  theBounciness(0.5),
      theHeight(1.0),
	  theIsMovable(false),
	  theWidth(1.0),
	  theWorldPtr(nullptr)
{
    theThisPtr = AbstractObjectPtr(nullptr);
    DEBUG5ENTRY;
	theB2BodyDefPtr= new b2BodyDef();

	theProps.setDefaultPropertiesString(
		Property::IMAGE_NAME_STRING + QString(":/") +
		Property::MASS_STRING + QString(":/") +
		Property::BOUNCINESS_STRING + QString(":0.3/") +
		Property::NOCOLLISION_STRING+ QString(":/") +
		Property::PIVOTPOINT_STRING + QString(":/") +
		Property::ROTATABLE_STRING + QString(":false/") +
		Property::TRANSLATIONGUIDE_STRING + QString(":/") +
		Property::ZVALUE_STRING + QString(":2.0/") );
}

AbstractObject::~AbstractObject ( )
{
	DEBUG5("AbstractObject::~AbstractObject() for %p '%s'", this, ASCII(getID()));

	// destroy the Body
	//
	deletePhysicsObject();

	// destroy the ShapeDefs
	//
	clearShapeList();

	// destroy the BodyDef
	//
	delete theB2BodyDefPtr;
	theB2BodyDefPtr=nullptr;

	deleteViewObject();
}


void AbstractObject::clearShapeList()
{
	while(theShapeList.isEmpty()==false)
	{
		b2FixtureDef* myFixtureDefPtr = theShapeList.first();
		if (myFixtureDefPtr!=nullptr)
		{
			const b2Shape* myShapePtr = myFixtureDefPtr->shape;
			delete myShapePtr;
		}
		delete myFixtureDefPtr;
		theShapeList.pop_front();
	}
}


void AbstractObject::createPhysicsObject(void)
{
	createPhysicsObject(theCenter);
}

void AbstractObject::createPhysicsObject(const Position& aPosition)
{
	DEBUG5("AbstractObject::createPhysicsObject() for %s, type %d", ASCII(getName()), getObjectType());
	// first fixup the bodydef with the current position
	assert(theB2BodyDefPtr!=nullptr);
	theB2BodyDefPtr->position.Set(aPosition.x, aPosition.y);
	theB2BodyDefPtr->angle = aPosition.angle;
	theB2BodyDefPtr->type  = getObjectType();
	// do not set mass properties here - that will be done in derived classes
	// (and as such is done already when we get here)

	assert (theB2BodyPtr==nullptr);
	if (theShapeList.count()==0)
		return;

	theB2BodyPtr = getB2WorldPtr()->CreateBody(theB2BodyDefPtr);
	assert(theB2BodyPtr != nullptr);

	// then create the shapes from the shapedefs
	ShapeList::const_iterator myI = theShapeList.begin();
	for (;myI != theShapeList.end(); ++myI)
	{
		(*myI)->restitution = theBounciness;
#ifdef QT_DEBUG
        b2Fixture* myPtr =
#endif
                theB2BodyPtr->CreateFixture(*myI);
#ifdef QT_DEBUG
		b2AABB myAABB;
		b2Transform myT;
		myT.SetIdentity();
        myPtr->GetShape()->ComputeAABB(&myAABB, myT, 0);
		DEBUG5("  Shape* = %p", myPtr);
		DEBUG5("    %fx%f", myAABB.GetExtents().x, myAABB.GetExtents().y);
#endif
	}
	notifyJoints(JointInterface::CREATED);
}

ViewObjectPtr  AbstractObject::createViewObject(float aDefaultDepth)
{
	if (theViewObjectPtr!=nullptr)
		return theViewObjectPtr;
	QString myImageName;
	if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true)==false)
		myImageName = getInternalName();

    theViewObjectPtr = new ViewObject(getThisPtr(), myImageName);

	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	return theViewObjectPtr;
}


void AbstractObject::deletePhysicsObject()
{
	DEBUG5("AbstractObject::deletePhysicsObject() for %p %s", this, ASCII(getID()));

    // we're only setting the pointer to zero - let's Box2D take care
    // of actually removing everything when we do delete world...
    theB2BodyPtr = nullptr;

    // let's also make sure we're getting rid of the joints
    notifyJoints(JointInterface::DELETED);

    theJointList.clear();
}

void AbstractObject::deleteViewObject(void)
{
	// delete the corresponding ViewObject
	// note that delete nullptr is allowed
	delete theViewObjectPtr;
	theViewObjectPtr = nullptr;
}


b2World* AbstractObject::getB2WorldPtr(void) const
{
	assert (theStaticB2WorldPtr);
	return theStaticB2WorldPtr;
}


const AbstractObjectSerializer* AbstractObject::getSerializer(void) const
{
    return new AbstractObjectSerializer(getThisPtr());
}


Position AbstractObject::getTempCenter (void) const
{
	// no physics object, no temp center
	if (isPhysicsObjectCreated()==false)
		return getOrigCenter();

	return Position(theB2BodyPtr->GetPosition(), theB2BodyPtr->GetAngle());
}


const QString AbstractObject::getToolTip() const
{
	// originally we had the concept of a 'Description' property.
	// however, that is not translatable.
	assert (!theProps.doesPropertyExists("Description"));
    return theToolTip;
}


bool AbstractObject::isMovable ( ) const
{
	if (theIsLevelEditor)
		return true;
	else
		return theIsMovable;
}


bool AbstractObject::isRotatable ( ) const
{
	bool myRotatableInfo = false;
	theProps.property2Bool(Property::ROTATABLE_STRING, &myRotatableInfo);
	return myRotatableInfo;
}


void AbstractObject::notifyJoints(JointInterface::JointStatus aStatus)
{
    foreach(JointInterfacePtr j, theJointList)
                j->physicsObjectStatus(aStatus);
        if (aStatus==JointInterface::DELETED)
            theJointList.clear();
}

void AbstractObject::parseProperties(void)
{
	// Bounciness
	float myFloat = 0.5;
	theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
	setTheBounciness(myFloat);

	// Child Pivot Point
    if (theChildPivotPointPtr)
    {
        theWorldPtr->removeObject(theChildPivotPointPtr);
    }
    Vector myDelta;
	if (theProps.property2Vector(Property::PIVOTPOINT_STRING, &myDelta))
	{
        theChildPivotPointPtr = ObjectFactory::createChildObject<PivotPoint>(getThisPtr(), myDelta);
		theChildPivotPointPtr->markAsChild();
		theWorldPtr->addObject(theChildPivotPointPtr);
	}

	// Child Translation Guid
	if (theChildTranslationGuidePtr)
	{
		theWorldPtr->removeObject(theChildTranslationGuidePtr);
	}
	float myAngle;
	if (theProps.property2Float(Property::TRANSLATIONGUIDE_STRING, &myAngle))
	{
        theChildTranslationGuidePtr = ObjectFactory::createChildObject<TranslationGuide>(getThisPtr(), myAngle);
        theChildTranslationGuidePtr->markAsChild();
		theWorldPtr->addObject(theChildTranslationGuidePtr);
	}

	// NoCollision
	QString myNoCollisionObjectIDs;
	theProps.property2String(Property::NOCOLLISION_STRING, &myNoCollisionObjectIDs);
	QStringList myObjIDList = myNoCollisionObjectIDs.split(";", QString::SkipEmptyParts);
	QStringList::iterator myI = myObjIDList.begin();
	while (myI != myObjIDList.end())
	{
        AbstractObjectPtr myObjPtr = theWorldPtr->findObjectByID(*myI);
        if (myObjPtr!=nullptr)
            theWorldPtr->addNoCollisionCombo(getThisPtr(), myObjPtr);
		++myI;
	}
}


void AbstractObject::setTheB2WorldPtr(b2World* aPtr)
{
	theStaticB2WorldPtr = aPtr;
}


void AbstractObject::setTheHeight ( qreal new_var, bool mustRunParseProperties )
{
	if (new_var>AbstractObject::MINIMUM_DIMENSION)
		theHeight = new_var;
	if (mustRunParseProperties)
		parseProperties();
}

void AbstractObject::setTheWidth ( qreal new_var, bool mustRunParseProperties )
{
	if (new_var>AbstractObject::MINIMUM_DIMENSION)
		theWidth = new_var;
	if (mustRunParseProperties)
		parseProperties();
}



void  AbstractObject::setViewObjectZValue(float aDefaultValue)
{
	assert(theViewObjectPtr != nullptr);
	// if no property with a float type found, leave aDefaultValue is unchanged
	theProps.property2Float(Property::ZVALUE_STRING, &aDefaultValue, false);
	theViewObjectPtr->setZValue(aDefaultValue);
}

void AbstractObject::updateViewObject(bool isSimRunning) const
{
	// no ViewObject: nothing to update ;-)
	if(theViewObjectPtr == nullptr)
		return;

	// no b2body: no part of simulation
	// PROBLEM: joints also don't have a b2Body - that's why they have their own
	// overriden version of this member...
	if (theB2BodyPtr==nullptr)
	{
		if (!isSimRunning)
		{
			// No sim running: adjust object drawing using static position & dimensions
			theViewObjectPtr->adjustObjectDrawing(theWidth, theHeight, theCenter);
		}
		theViewObjectPtr->setNewImageIndex(getImageIndex());
		return;
	}

	// Sim running: don't need to adjust objects that are static or asleep
	if (theB2BodyPtr->IsAwake() || theB2BodyPtr->GetMass()>0.0001 || !isSimRunning)
	{
		theViewObjectPtr->adjustObjectDrawing(getTempWidth(),
											  getTempHeight(),
											  getTempCenter());
	}
	theViewObjectPtr->setNewImageIndex(getImageIndex());
}
