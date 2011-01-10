/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#include "BaseObject.h"
#include "DrawObject.h"
#include "World.h"
#include "Box2D.h"
#include "BaseObjectSerializer.h"
#include "PivotPoint.h"
#include "TranslationGuide.h"

// Static variables
static b2World* theStaticB2WorldPtr = NULL;

//   ***********************************************
//   *                                             *
//   * NOTE: the ObjectFactory class definition is *
//   *       below the BaseObject definition       *
//   *                                             *
//   ***********************************************


// Constructors/Destructors
//  

BaseObject::BaseObject ( ) 
{
	DEBUG6("BaseObject::BaseObject() for %p\n", this);
	initAttributes();
}

BaseObject::~BaseObject ( ) 
{
	DEBUG5("BaseObject::~BaseObject() for %p\n", this);

	// destroy the Body
	//
	deletePhysicsObject();
	
	// destroy the ShapeDefs
	//
	clearShapeList();
	
	// destroy the BodyDef
	//
	if (theB2BodyDefPtr)
	{
		delete theB2BodyDefPtr;
		theB2BodyDefPtr=NULL;
	}

	// delete the corresponding DrawObject
	//
	if (theDrawObjectPtr)
	{
		delete theDrawObjectPtr;
		theDrawObjectPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

b2World* BaseObject::getB2WorldPtr(void) const
{
	assert (theStaticB2WorldPtr);
	return theStaticB2WorldPtr;
}

Position BaseObject::getTempCenter (void) const
{
	// no physics object, no temp center
	if (isPhysicsObjectCreated()==false)
		return getOrigCenter();
	
	return Position(theB2BodyPtr->GetPosition(), theB2BodyPtr->GetAngle());
}

bool BaseObject::isMovable ( ) const
{
	if (theIsLevelEditor)
		return true;
	else
		return theIsMovable;
}

void BaseObject::ForWorldOnly::setTheB2WorldPtr(b2World* aPtr)
{
	theStaticB2WorldPtr = aPtr;
}


// Other methods
//  


void BaseObject::clearShapeList()
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

DrawObject*  BaseObject::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	QString myImageName;
	if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true)==false)
		myImageName = getName();

	theDrawObjectPtr = new DrawObject(this, myImageName);

	setDrawObjectZValue(2.0); // will set ZValue different if set in property
	return theDrawObjectPtr;
}

void BaseObject::createPhysicsObject(void)
{
	createPhysicsObject(theCenter);
}

void BaseObject::createPhysicsObject(Position aPosition)
{
	DEBUG5("BaseObject::createPhysicsObject() for %s, type %d\n", ASCII(getName()), getObjectType());
	// first fixup the bodydef with the current position
	assert(theB2BodyDefPtr!=NULL);
	theB2BodyDefPtr->position.Set(aPosition.x, aPosition.y);
	theB2BodyDefPtr->angle = aPosition.angle;
	theB2BodyDefPtr->type  = getObjectType();
	// do not set mass properties here - that will be done in derived classes
	// (and as such is done already when we get here)
	
	// then create the body (or not)
	if (theB2BodyPtr!=NULL)
		deletePhysicsObject();
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
		DEBUG5("  Shape* = %p\n", myPtr);
	}
//	theB2BodyPtr->SetMassFromShapes();
	DEBUG5("Object %s has mass %f kg\n", ASCII(getName()),
		theB2BodyPtr->GetMass());
	notifyJoints(JointInterface::CREATED);
}

void BaseObject::deletePhysicsObject()
{
	DEBUG5("BaseObject::deletePhysicsObject() for %p\n", this);
	// have B2World destroy the body - that will automatically destroy
	// the shapes
	if (theB2BodyPtr!=NULL)
		getB2WorldPtr()->DestroyBody(theB2BodyPtr);
	theB2BodyPtr = NULL;
	// let's also make sure we're getting rid of the joints
	notifyJoints(JointInterface::DELETED);
}


bool BaseObject::deregister(void)
{
	deletePhysicsObject();
	if (theDrawObjectPtr)
		delete theDrawObjectPtr;
	theDrawObjectPtr=NULL;
	theWorldPtr->removeObject(this);
	// note that we do not delete ourselves!
	return true;
}

const BaseObjectSerializer* BaseObject::getSerializer(void) const
{
	return new BaseObjectSerializer(this);
}

void BaseObject::initAttributes ( ) 
{
	DEBUG5("BaseObject::initAttributes\n");
	theB2BodyDefPtr= new b2BodyDef();
	theB2BodyPtr=NULL;
	
	theWidth = 1.0;
	theHeight = 1.0;
	theBounciness = 1.0;
	// don't need to initialise theCenter - it has a default constructor
	
	theDrawObjectPtr = NULL;
	theWorldPtr = NULL;

	// TODO - we need to move this into Level, once Toolbox is implemented
	theIsMovable = true;

	theProps.setDefaultPropertiesString(
		Property::IMAGE_NAME_STRING + QString(":/") +
		Property::MASS_STRING + QString(":/") +
		Property::BOUNCINESS_STRING + QString(":0.3/") +
		Property::NOCOLLISION_STRING+ QString(":/") +
		Property::PIVOTPOINT_STRING + QString(":/") +
		Property::TRANSLATIONGUIDE_STRING + QString(":/") +
		Property::ZVALUE_STRING + QString(":2.0/") );
}

//bool BaseObject::isSleeping() const
//{
//	if (isPhysicsObjectCreated()) 
//		return theB2BodyPtr->IsSleeping(); 
//	else 
//		return false;
//}

void BaseObject::notifyJoints(JointInterface::JointStatus aStatus)
{
	foreach(JointInterface* j, theJointList)
		j->physicsObjectStatus(aStatus);
}

void BaseObject::parseProperties(void)
{
	// use the default if nothing else...
	float myFloat;
	theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
	setTheBounciness(myFloat);

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
		BaseObject* myObjPtr = theWorldPtr->findObjectByID(*myI);
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
}


bool BaseObject::reregister(void)
{
	if (theDrawObjectPtr != NULL)
	{
		delete theDrawObjectPtr;
		theDrawObjectPtr = NULL;
	}
	// the addObject also forces creation of the drawObject
	// and adds it to the DrawWorld - if it exists.
	// fortunately, reregister is only used when a drawworld exists :-)
	theWorldPtr->addObject(this);
	assert(theDrawObjectPtr != NULL);
	theDrawObjectPtr->focusInEvent();
	return true;
}

void  BaseObject::setDrawObjectZValue(float aDefaultValue)
{
	assert(theDrawObjectPtr != NULL);
	// if no property with a float type found, leave aDefaultValue is unchanged
	theProps.property2Float(Property::ZVALUE_STRING, &aDefaultValue, false);
	theDrawObjectPtr->setZValue(aDefaultValue);
}

void BaseObject::setOrigCenter ( Position new_var )
{
	theCenter.x = new_var.x;
	theCenter.y = new_var.y;
	theCenter.angle = new_var.angle;
}


//////////////////////////////////////////////////////////////////////////////

/** small, only locally used class
  */
class FactoryList
{
public:
	void insert(const QString& aName, ObjectFactory* theFactoryPtr)
		{ theMap[aName]=theFactoryPtr; }
	const ObjectFactory* getFactoryPtr(const QString& aName) const
		{ return theMap.value(aName); }

	ObjectFactory::ObjectFactoryList* getAllFactories(void)
	{
		ObjectFactory::ObjectFactoryList* myList = new ObjectFactory::ObjectFactoryList();
		TheMap::iterator myI = theMap.begin();
		while (myI != theMap.end())
		{
			myList->push_back(*myI);
			++myI;
		}
		return myList;
	}

private:
	typedef QMap<QString, ObjectFactory*> TheMap;
	TheMap theMap;
};
static FactoryList* theFactoryListPtr=NULL;


void
ObjectFactory::announceObjectType(const QString& anObjectTypeName, ObjectFactory* aThisPtr)
{
	DEBUG4("ObjectFactory::announceObjectType(\"%s\", %p)\n", 
			ASCII(anObjectTypeName), aThisPtr);
	if (theFactoryListPtr==NULL)
		theFactoryListPtr = new FactoryList();
	theFactoryListPtr->insert(anObjectTypeName,aThisPtr);
	aThisPtr->theFactoryName = anObjectTypeName;
}

BaseObject* 
ObjectFactory::createObject(
		const QString& aName, 
		const Position aPosition,
		const qreal aWidth,
		const qreal anHeight)
{
	const ObjectFactory* myFactoryPtr = theFactoryListPtr->getFactoryPtr(aName);
	DEBUG5("ObjectFactory::createObject(\"%s\") Factory=%p\n", ASCII(aName), myFactoryPtr);
	if (myFactoryPtr == NULL)
	{
		DEBUG1("There is no factory for Object type %s\n", ASCII(aName));
		return NULL;
	}
	BaseObject* myObjectPtr = myFactoryPtr->createObject();
	DEBUG5("  object created = %p, i18n name = '%s'\n", myObjectPtr, ASCII(myObjectPtr->getName()));
	assert (aName.contains(" ")==false);
	myObjectPtr->theInternalName = aName;
	myObjectPtr->theCenter=aPosition;
	if (aWidth!=1.0)
		myObjectPtr->theWidth=aWidth;
	if (anHeight!=1.0)
		myObjectPtr->theHeight=anHeight;
	return myObjectPtr;
}


BaseObject* ObjectFactory::fixObject(BaseObject* anObject) const
{
	anObject->theInternalName = theFactoryName;
	return anObject;
}

ObjectFactory::ObjectFactoryList* ObjectFactory::getAllFactories(void)
{
	if (theFactoryListPtr==NULL)
		return NULL;
	return theFactoryListPtr->getAllFactories();
}
