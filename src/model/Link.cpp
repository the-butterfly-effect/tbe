/* The Butterfly Effect
 * This file copyright (C) 2010,2012  Klaas van Gend
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

#include "tbe_global.h"
#include "Link.h"
#include "ObjectFactory.h"
#include "ViewLink.h"

#include "Box2D.h"
#include <cassert>

//// this class' ObjectFactory
class LinkObjectFactory : public ObjectFactory
{
public:
    LinkObjectFactory(void)
    {
        announceObjectType("Link", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new Link());
    }
};
static LinkObjectFactory theLinkFactory;

Link::Link(void)
    : AbstractJoint(),
      theFirstPtr(nullptr),
      theSecondPtr(nullptr),
      theFirstLocalPosPtr(nullptr),
      theSecondLocalPosPtr(nullptr)
{
    theToolTip = QObject::tr("A Link is a massless, bodyless connection between two objects.");
    theProps.setDefaultPropertiesString(
        Property::OBJECT1_STRING + QString(":/") +
        Property::OBJECT2_STRING + QString(":/") +
        Property::OVERLAP_STRING + QString(":10/") +
        "-" + Property::MASS_STRING + ":/" +
        "-" + Property::BOUNCINESS_STRING + ":/" +
        "-" + Property::FRICTION_STRING + ":/" +
        "-" + Property::PIVOTPOINT_STRING + ":/" +
        "-" + Property::ROTATABLE_STRING + ":/" +
        "-" + Property::RESIZABLE_STRING + ":/" +
        "-" + Property::TRANSLATIONGUIDE_STRING + ":/" +
        "-" + Property::NOCOLLISION_STRING + ":/" +
        Property::ZVALUE_STRING + QString(":20.0/"));

}

Link::~Link ()
{
    DEBUG5("Link::~Link() %p", this);
    clearObjectReferences();
}


void Link::clearObjectReferences()
{
    AbstractObject::clearObjectReferences();

    theFirstPtr  = nullptr;
    theSecondPtr = nullptr;

    delete theFirstLocalPosPtr;
    theFirstLocalPosPtr  = nullptr;
    delete theSecondLocalPosPtr;
    theSecondLocalPosPtr = nullptr;
}


ViewObjectPtr  Link::createViewObject(float aDefaultDepth)
{
    if (theViewObjectPtr != nullptr)
        return theViewObjectPtr;
    QString myImageName;
    if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true) == false)
        myImageName = getInternalName();
    theViewObjectPtr = ViewObject::factoryMethod<ViewLink>(getThisPtr(), myImageName);
    setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
    return theViewObjectPtr;
}


void Link::createPhysicsObject(void)
{
    if (theWorldPtr == nullptr)
        return;

    assert(theFirstPtr != nullptr);
    assert(theFirstLocalPosPtr != nullptr);
    if (theFirstPtr == nullptr) {
        DEBUG4("Link: No valid first object found...");
        return;
    }
    b2Body *myFirstB2BodyPtr = getB2BodyPtrFor(theFirstPtr, theFirstLocalPosPtr->toPosition());
    assert (myFirstB2BodyPtr);
    theFirstPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

    assert(theSecondPtr != nullptr);
    assert(theSecondLocalPosPtr != nullptr);
    if (theSecondPtr == nullptr) {
        DEBUG4("Link: No valid second object found...");
        return;
    }
    b2Body *mySecondB2BodyPtr = getB2BodyPtrFor(theSecondPtr, theSecondLocalPosPtr->toPosition());
    assert (mySecondB2BodyPtr);
    theSecondPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

    // *** initialise Box2D's distance joint:
    // note: Initialize() uses a global coordinate...
    b2DistanceJointDef myJointDef;

    myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr,
                          (theFirstPtr->getOrigCenter() + *theFirstLocalPosPtr).toB2Vec2(),
                          (theSecondPtr->getOrigCenter() + *theSecondLocalPosPtr).toB2Vec2());
    myJointDef.userData = this;
    theJointPtr = (b2DistanceJoint *) getB2WorldPtr()->CreateJoint(&myJointDef);
}

Position Link::getTempCenter() const
{
    if (theFirstPtr == nullptr || theFirstLocalPosPtr == nullptr ||
            theSecondPtr == nullptr || theSecondLocalPosPtr == nullptr)
        return getOrigCenter();

    Vector myV1 = (theFirstPtr->getTempCenter() + *theFirstLocalPosPtr).toVector();
    Vector myV2 = (theSecondPtr->getTempCenter() + *theSecondLocalPosPtr).toVector();
    Vector myMiddle = myV1 + myV2;
    myMiddle = 0.5 * myMiddle;

    Vector myDiff = myV1 - myV2;
    double myAngle = myDiff.toAngle();

    return Position(myMiddle.toB2Vec2(), myAngle);
}


void Link::parseProperties(void)
{
    AbstractObject::parseProperties();
    if (theWorldPtr == nullptr)
        return;

    // *** parse object1 & object2
    // NOTE: if we used the constructor with baseobject, (i.e. properties
    // aren't read yet) this will still work because propertyToObjectPtr
    // only modifies theFirstPtr/theSecondPtr if successful
    theFirstPtr = theProps.property2ObjectPlusVectorPtr(theWorldPtr,
                                                        Property::OBJECT1_STRING,
                                                        &theFirstLocalPosPtr);

    theSecondPtr = theProps.property2ObjectPlusVectorPtr(theWorldPtr,
                                                         Property::OBJECT2_STRING,
                                                         &theSecondLocalPosPtr);
}


void Link::updateOrigCenter(void)
{
}



void Link::updateViewObject(bool ) const
{
    // no ViewObject: nothing to update ;-)
    if (theViewObjectPtr == nullptr)
        return;

    // Sim running: don't need to adjust objects that are static or asleep
    ViewLink *theVLPtr = dynamic_cast<ViewLink *>(theViewObjectPtr.data());
    assert(theVLPtr != nullptr);

    Vector myV1, myV2;
    if (theFirstPtr != nullptr) {
        myV1 = (theFirstPtr->getTempCenter() + *theFirstLocalPosPtr).toVector();
    }
    else {
        myV1 = getTempCenter() - getTheWidth() * Vector(0.5,0.);
    }
    if (theSecondPtr != nullptr) {
        myV2 = (theSecondPtr->getTempCenter() + *theSecondLocalPosPtr).toVector();
    }
    else {
        myV2 = getTempCenter() + getTheWidth() * Vector(0.5,0.);
    }

    theVLPtr->setEndpoints(myV1, myV2);
}
