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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "AbstractObject.h"
#include "Box2D.h"
#include "ViewObject.h"

// I wonder if this should be b2_linearSlop instead of this number...
const float AbstractObject::MINIMUM_DIMENSION = 0.03;

// Static variables
static b2World* theStaticB2WorldPtr = NULL;


AbstractObject::AbstractObject()
    : 	theB2BodyPtr(NULL),
      theB2BodyDefPtr(NULL),
      theViewObjectPtr(NULL),
      theWorldPtr(NULL)
{
}

AbstractObject::~AbstractObject ( )
{
	DEBUG5("AbstractObject::~AbstractObject() for %p\n", this);

	// destroy the Body
	//
	deletePhysicsObject();

	// destroy the ShapeDefs
	//
	clearShapeList();

	// destroy the BodyDef
	//
	delete theB2BodyDefPtr;
	theB2BodyDefPtr=NULL;

	deleteViewObject();
}


void AbstractObject::clearShapeList()
{
	while(theShapeList.isEmpty()==false)
	{
		b2FixtureDef* myFixtureDefPtr = theShapeList.first();
		if (myFixtureDefPtr!=NULL)
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
	DEBUG5("AbstractObject::createPhysicsObject() for %s, type %d\n", ASCII(getName()), getObjectType());
	// first fixup the bodydef with the current position
	assert(theB2BodyDefPtr!=NULL);
	theB2BodyDefPtr->position.Set(aPosition.x, aPosition.y);
	theB2BodyDefPtr->angle = aPosition.angle;
	theB2BodyDefPtr->type  = getObjectType();
	// do not set mass properties here - that will be done in derived classes
	// (and as such is done already when we get here)

	assert (theB2BodyPtr==NULL);
	if (theShapeList.count()==0)
		return;

	theB2BodyPtr = getB2WorldPtr()->CreateBody(theB2BodyDefPtr);
	assert(theB2BodyPtr != NULL);

	// then create the shapes from the shapedefs
	ShapeList::const_iterator myI = theShapeList.begin();
	for (;myI != theShapeList.end(); ++myI)
	{
		(*myI)->restitution = theBounciness;
		b2Fixture* myPtr = theB2BodyPtr->CreateFixture(*myI);
#ifndef NDEBUG
		b2AABB myAABB;
		b2Transform myT;
		myT.SetIdentity();
		myPtr->GetShape()->ComputeAABB(&myAABB, myT);
		DEBUG5("  Shape* = %p\n", myPtr);
		DEBUG5("    %fx%f\n", myAABB.GetExtents().x, myAABB.GetExtents().y);
#endif
	}
	qDebug () << QString("Object %1 has mass %2 kg\n").arg(getName())
				 .arg(theB2BodyPtr->GetMass());
	notifyJoints(JointInterface::CREATED);
}

ViewObject*  AbstractObject::createViewObject(float aDefaultDepth)
{
	if (theViewObjectPtr!=NULL)
		return theViewObjectPtr;
	QString myImageName;
	if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true)==false)
		myImageName = getInternalName();

	theViewObjectPtr = new ViewObject(this, myImageName);

	setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
	return theViewObjectPtr;
}


void AbstractObject::deletePhysicsObject()
{
	DEBUG5("AbstractObject::deletePhysicsObject() for %p %s\n", this, ASCII(getID()));

	// we're only setting the pointer to zero - let's Box2D take care
	// of actually removing everything when we do delete world...
	theB2BodyPtr = NULL;

	// let's also make sure we're getting rid of the joints
	notifyJoints(JointInterface::DELETED);
}

void AbstractObject::deleteViewObject(void)
{
	// delete the corresponding DrawObject
	// note that delete NULL is allowed
	delete theViewObjectPtr;
	theViewObjectPtr = NULL;
}


b2World* AbstractObject::getB2WorldPtr(void) const
{
	assert (theStaticB2WorldPtr);
	return theStaticB2WorldPtr;
}

void AbstractObject::initAttributes ( )
{
	DEBUG5("AbstractObject::initAttributes\n");
	theB2BodyDefPtr= new b2BodyDef();
	theB2BodyPtr=NULL;

	theWidth = 1.0;
	theHeight = 1.0;
	theBounciness = 1.0;
	// don't need to initialise theCenter - it has a default constructor

	theViewObjectPtr = NULL;
	theWorldPtr = NULL;

	// TODO - we need to move this into Level, once Toolbox is implemented
	theIsMovable = true;

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


void AbstractObject::parseProperties(void)
{
	// use the default if nothing else...
	float myFloat;
	theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
	setTheBounciness(myFloat);

#if 0 // directly copied from AbstractObject.cpp - not used for newgui yet
	Vector myDelta;
	if (theProps.property2Vector(Property::PIVOTPOINT_STRING, &myDelta))
	{
		PivotPoint* myPP = new PivotPoint(this, myDelta);
		myPP->markAsChild();
		theWorldPtr->addObject(myPP);
	}
	float myAngle;
	if (theProps.property2Float(Property::TRANSLATIONGUIDE_STRING, &myAngle))
	{
		TranslationGuide* myTJ = new TranslationGuide(this, myAngle);
		myTJ->markAsChild();
		theWorldPtr->addObject(myTJ);
	}

	QString myNoCollisionObjectIDs;
	theProps.property2String(Property::NOCOLLISION_STRING, &myNoCollisionObjectIDs);
	QStringList myObjIDList = myNoCollisionObjectIDs.split(";", QString::SkipEmptyParts);
	QStringList::iterator myI = myObjIDList.begin();
	while (myI != myObjIDList.end())
	{
		AbstractObject* myObjPtr = theWorldPtr->findObjectByID(*myI);
		if (myObjPtr!=NULL)
			theWorldPtr->addNoCollisionCombo(this, myObjPtr);
		++myI;
	}

	// For normal situations, i.e. created by Level for World
	// the setupCache() is run twice. But that's not a problem.
	// For inserted objects (i.e. drag from toolbox), this is
	// the only time setupCache is called. Let's cherish it.
	if (theDrawObjectPtr)
		theDrawObjectPtr->setupCache();
#endif
}


void AbstractObject::notifyJoints(JointInterface::JointStatus aStatus) const
{
	foreach(JointInterface* j, theJointList)
		j->physicsObjectStatus(aStatus);
}


void AbstractObject::setTheB2WorldPtr(b2World* aPtr)
{
	theStaticB2WorldPtr = aPtr;
}


void  AbstractObject::setViewObjectZValue(float aDefaultValue)
{
	assert(theViewObjectPtr != NULL);
	// if no property with a float type found, leave aDefaultValue is unchanged
	theProps.property2Float(Property::ZVALUE_STRING, &aDefaultValue, false);
	theViewObjectPtr->setZValue(aDefaultValue);
}
