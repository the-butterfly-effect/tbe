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
#include "Translator.h"
#include "TranslationGuide.h"
#include "ViewItem.h"
#include "ViewObject.h"
#include "ViewWorldItem.h"
#include "World.h"

#include <QStringList>
#include <cassert>

// I wonder if this should be b2_linearSlop instead of this number...
const float AbstractObject::MINIMUM_DIMENSION = 0.03;

// Static variables
static b2World *theStaticB2WorldPtr = nullptr;



AbstractObject::AbstractObject()
    : AbstractObject("", "", 1.0, 1.0, 0.0, 0.5, "")
{
    DEBUG5("AbstractObject::AbstractObject() for %p", this);
}

AbstractObject::AbstractObject(const QString &aTooltip,
                               const QString &aImageName,
                               qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness,
                               const QString &aPropertiesText)
    : theB2BodyPtr(nullptr),
      theToolTip(aTooltip),
      hasCustomToolTip(false),
      theViewObjectPtr(nullptr),
      theViewItemPtr(nullptr),
      theChildPivotPointPtr(nullptr),
      theChildTranslationGuidePtr(nullptr),
      theBounciness(aBounciness),
      theHeight(aHeight),
      theIsMovable(false),
      theWidth(aWidth),
      theWorldPtr(nullptr)
{
    DEBUG5("AbstractObject::AbstractObject(...) for %p '%s'", this, ASCII(aImageName));
    theThisPtr = AbstractObjectPtr(nullptr);
    DEBUG5ENTRY;
    theB2BodyDefPtr = new b2BodyDef();

    theProps.setDefaultPropertiesString(
        Property::IMAGE_NAME_STRING + QString(":%1/").arg(aImageName) +
        Property::BOUNCINESS_STRING + QString(":%1/").arg(theBounciness) +
        Property::FRICTION_STRING + QString(":/") +
        Property::NOCOLLISION_STRING + QString(":/") +
        Property::ROTATABLE_STRING + QString(":false/") +
        Property::RESIZABLE_STRING + QString(":none/") +
        Property::ZVALUE_STRING + QString(":2.0/") );

    if (aMass > 0.001)
        theProps.setDefaultPropertiesString(
            QString("%1:%2/").arg(Property::MASS_STRING).arg(QString::number(aMass)) +
            QString("%1:/").arg(Property::PIVOTPOINT_STRING) +
            QString("%1:/").arg(Property::TRANSLATIONGUIDE_STRING));
    else {
        theProps.removeProperty(Property::MASS_STRING);
        theProps.removeProperty(Property::PIVOTPOINT_STRING);
        theProps.removeProperty(Property::TRANSLATIONGUIDE_STRING);
    }

    // and overrule the default props set above if needed...
    theProps.setDefaultPropertiesString(aPropertiesText);
}

AbstractObject::~AbstractObject ( )
{
    DEBUG5("AbstractObject::~AbstractObject() for %p '%s'", this, ASCII(getID()));

    // destroy the ShapeDefs
    //
    clearShapeList();

    // destroy the BodyDef
    //
    delete theB2BodyDefPtr;
    theB2BodyDefPtr = nullptr;

    deleteViewObject();
}

void AbstractObject::causeWounded(AbstractObject::WhyWounded)
{
    // nothing to do, 'normal' objects cannot 'die'
}

void AbstractObject::clearObjectReferences()
{
    // destroy the Body, if still necessary
    //
    deletePhysicsObject();

    // destroy the references to the joints
    DEBUG3("  deleting %lu joints...", theJointList.size());
    theJointList.clear();
    theChildPivotPointPtr = nullptr;
    theChildTranslationGuidePtr = nullptr;
}


void AbstractObject::clearShapeList()
{
    while (theShapeList.isEmpty() == false) {
        b2FixtureDef *myFixtureDefPtr = theShapeList.first();
        if (myFixtureDefPtr != nullptr) {
            const b2Shape *myShapePtr = myFixtureDefPtr->shape;
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

void AbstractObject::createPhysicsObject(const Position &aPosition)
{
    DEBUG5("AbstractObject::createPhysicsObject() for %s, type %d", ASCII(getName()), getObjectType());
    // first fixup the bodydef with the current position
    assert(theB2BodyDefPtr != nullptr);
    theB2BodyDefPtr->position.Set(aPosition.x, aPosition.y);
    theB2BodyDefPtr->angle = aPosition.angle;
    theB2BodyDefPtr->type  = getObjectType();
    // do not set mass properties here - that will be done in derived classes
    // (and as such is done already when we get here)

    assert (theB2BodyPtr == nullptr);
    if (theShapeList.count() == 0)
        return;

    theB2BodyPtr = getB2WorldPtr()->CreateBody(theB2BodyDefPtr);
    assert(theB2BodyPtr != nullptr);

    // then create the shapes from the shapedefs
    ShapeList::const_iterator myI = theShapeList.begin();
    for (; myI != theShapeList.end(); ++myI) {
        (*myI)->restitution = theBounciness;
#ifdef QT_DEBUG
        b2Fixture *myPtr =
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
    if (theViewObjectPtr != nullptr)
        return theViewObjectPtr;
    theViewObjectPtr = ViewObject::factoryMethod<ViewObject>(getThisPtr(), getImageName());

    theViewObjectPtr->setZValue(calculateZValue(aDefaultDepth)); // will set ZValue different if set in property
    return theViewObjectPtr;
}


ViewItem *AbstractObject::createViewItemInt(float aDefaultDepth, const QString& aVOType, const QString& anImageName, const QString& extraOptions)
{
    ViewWorldItem* myVWIPtr = ViewWorldItem::me();
    assert(nullptr != myVWIPtr);

    QString myImageName;
    if (anImageName.isEmpty()) {
        myImageName = getImageName();
    }
    else
        myImageName = anImageName;

    theViewItemPtr = myVWIPtr->createViewItem(aVOType, getThisPtr(), calculateZValue(aDefaultDepth),
                                              QString("imageName: \"%1\"; %2").arg(myImageName).arg(extraOptions));
    return theViewItemPtr;
}


void AbstractObject::deleteViewItem()
{
    if (nullptr == theViewItemPtr)
        return;
    ViewItem* myVIPtr = theViewItemPtr;
    theViewItemPtr = nullptr;
    myVIPtr->setParent(nullptr);
    myVIPtr->setParentItem(nullptr);
    myVIPtr->deleteLater();
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
    DEBUG3ENTRY;
    // clear the corresponding ViewObject
    theViewObjectPtr.clear();
    assert(theViewObjectPtr.data() == nullptr);
}


b2World *AbstractObject::getB2WorldPtr(void) const
{
    assert (theStaticB2WorldPtr);
    return theStaticB2WorldPtr;
}


const QString AbstractObject::getImageName() const
{
    QString myImageName;
    if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true) == false)
        myImageName = getInternalName();
    return myImageName;
}


const AbstractObjectSerializer *AbstractObject::getSerializer(void) const
{
    return new AbstractObjectSerializer(getThisPtr());
}


Position AbstractObject::getTempCenter (void) const
{
    // no physics object, no temp center
    if (isPhysicsObjectCreated() == false)
        return getOrigCenter();

    return Position(theB2BodyPtr->GetPosition(), theB2BodyPtr->GetAngle());
}


const QString AbstractObject::getToolTip() const
{
    // originally we had the concept of a 'Description' property.
    // however, that is not translatable.
    assert (!theProps.doesPropertyExists("Description"));
    return TheGetText(theToolTip);
}


AbstractObject::SizeDirections AbstractObject::isResizable ( )
{
    QString myString;
    SizeDirections myResizeInfo = NORESIZING;
    if (theProps.property2String(Property::RESIZABLE_STRING, &myString)) {
        // We do not need to check for none, that's the default.
        if (myString == Property::HORIZONTAL_STRING)
            myResizeInfo = HORIZONTALRESIZE;
        if (myString == Property::VERTICAL_STRING)
            myResizeInfo = VERTICALRESIZE;
        if (myString == Property::TOTALRESIZE_STRING)
            myResizeInfo = TOTALRESIZE;
    }
    return myResizeInfo;
}


bool AbstractObject::isMovable ( ) const
{
    if (theIsLevelCreator)
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
    foreach (JointInterfacePtr j, theJointList)
        j->physicsObjectStatus(aStatus);
    if (aStatus == JointInterface::DELETED)
        theJointList.clear();
}

void AbstractObject::parseProperties(void)
{
    // Bounciness
    float myFloat = 0.5;
    theProps.property2Float(Property::BOUNCINESS_STRING, &myFloat);
    setTheBounciness(myFloat);

    // Child Pivot Point
    if (theChildPivotPointPtr) {
        theWorldPtr->removeObject(theChildPivotPointPtr);
        theChildPivotPointPtr = nullptr;
    }
    Vector myDelta;
    if (theProps.property2Vector(Property::PIVOTPOINT_STRING, &myDelta)) {
        if (theWorldPtr) {
            theChildPivotPointPtr = ObjectFactory::createChildObject<PivotPoint>(getThisPtr(), myDelta);
            theChildPivotPointPtr->markAsChild();
            theWorldPtr->addObject(theChildPivotPointPtr);
        }
    }

    // Child Translation Guid
    if (theChildTranslationGuidePtr) {
        theWorldPtr->removeObject(theChildTranslationGuidePtr);
        theChildTranslationGuidePtr = nullptr;
    }
    float myAngle;
    if (theProps.property2Float(Property::TRANSLATIONGUIDE_STRING, &myAngle)) {
        theChildTranslationGuidePtr = ObjectFactory::createChildObject<TranslationGuide>(getThisPtr(),
                                                                                         myAngle);
        theChildTranslationGuidePtr->markAsChild();
        theWorldPtr->addObject(theChildTranslationGuidePtr);
    }

    // NoCollision
    QString myNoCollisionObjectIDs;
    theProps.property2String(Property::NOCOLLISION_STRING, &myNoCollisionObjectIDs);
    QStringList myObjIDList = myNoCollisionObjectIDs.split(";", QString::SkipEmptyParts);
    QStringList::iterator myI = myObjIDList.begin();
    while (myI != myObjIDList.end()) {
        QList<AbstractObjectPtr> myObjPtrs = theWorldPtr->findObjectsByID(*myI);
        for (auto i : myObjPtrs)
            theWorldPtr->addNoCollisionCombo(getThisPtr(), i);
        ++myI;
    }

    // force parsing of resize info
    isResizable();
}


void AbstractObject::setTheB2WorldPtr(b2World *aPtr)
{
    theStaticB2WorldPtr = aPtr;
}


void AbstractObject::setTheHeight ( qreal new_var, bool mustRunParseProperties )
{
    if (new_var > AbstractObject::MINIMUM_DIMENSION)
        theHeight = new_var;
    if (mustRunParseProperties)
        parseProperties();
}

void AbstractObject::setTheWidth ( qreal new_var, bool mustRunParseProperties )
{
    if (new_var > AbstractObject::MINIMUM_DIMENSION)
        theWidth = new_var;
    if (mustRunParseProperties)
        parseProperties();
}



float AbstractObject::calculateZValue(float aDefaultValue)
{
    // we have several cases:
    //  a) aDefaultValue = 2.0 ,  default property set -> use default property
    //  b) aDefaultValue !=2   ,  default property set -> use aDefaultValue
    //  c) aDefaultValue = 2.0 ,  non-default property set -> use non-default property
    //  d) aDefaultValue !=2   ,  non-default property set -> use non-default property
    float myTemp = -99.684;
    theProps.property2Float(Property::ZVALUE_STRING, &myTemp, true);
    if (aDefaultValue == 2.0 && myTemp != -99.684)
        aDefaultValue = myTemp;
    return aDefaultValue;
}

void AbstractObject::updateViewObject(bool isSimRunning) const
{
    // no b2body: no part of simulation
    // PROBLEM: joints also don't have a b2Body - that's why they have their own
    // overriden version of this member...
    if (theB2BodyPtr == nullptr) {
        if (!isSimRunning) {
            // No sim running: adjust object drawing using static position & dimensions
            if (nullptr != theViewObjectPtr)
                theViewObjectPtr->adjustObjectDrawing(theWidth, theHeight, theCenter);
            if (nullptr != theViewItemPtr)
                theViewItemPtr->adjustObjectDrawingFromAO();
        }
        if (nullptr != theViewObjectPtr) {
            theViewObjectPtr->setNewImageIndex(getImageIndex());
        }
        // TODO: the ViewItemPtr setImageIndex one
        return;
    }

    // Sim running: don't need to adjust objects that are static or asleep
    if (theB2BodyPtr->IsAwake() || theB2BodyPtr->GetMass() > 0.0001 || !isSimRunning) {
        if (nullptr != theViewObjectPtr)
            theViewObjectPtr->adjustObjectDrawing(getTempWidth(),
                                                  getTempHeight(),
                                                  getTempCenter());
        if (nullptr != theViewItemPtr)
            theViewItemPtr->adjustObjectDrawingFromAO();
    }
    if (nullptr != theViewObjectPtr)
        theViewObjectPtr->setNewImageIndex(getImageIndex());
    // TODO: the ViewItemPtr setImageIndex one
}
