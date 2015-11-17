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
    {	announceObjectType("Link", this); }
    AbstractObject* createObject(void) const override
    {	return fixObject(new Link()); }
};
static LinkObjectFactory theLinkFactory;

Link::Link(void)
    : AbstractJoint(),
      theFirstPtr(NULL),
      theSecondPtr(NULL),
      theFirstLocalPosPtr(NULL),
      theSecondLocalPosPtr(NULL)
{
    theToolTip=QObject::tr("A Link is a massless, bodyless connection between two objects.");
    theProps.setDefaultPropertiesString(
                Property::OBJECT1_STRING + QString(":/") +
                Property::OBJECT2_STRING + QString(":/") +
                Property::OVERLAP_STRING + QString(":10/") +
                "-" + Property::MASS_STRING + ":/" );
}

Link::~Link ()
{
    // nothing real to do ?!?
    theFirstPtr  = NULL;
    theSecondPtr = NULL;

    delete theFirstLocalPosPtr;
    theFirstLocalPosPtr  = NULL;
    delete theSecondLocalPosPtr;
    theSecondLocalPosPtr = NULL;
}

ViewObject*  Link::createViewObject(float aDefaultDepth)
{
    if (theViewObjectPtr!=NULL)
        return theViewObjectPtr;
    QString myImageName;
    if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true)==false)
        myImageName = getInternalName();
    theViewObjectPtr = new ViewLink(getThisPtr(), myImageName);
    setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
    return theViewObjectPtr;
}


void Link::createPhysicsObject(void)
{
    if (theWorldPtr==NULL)
        return;

    assert(theFirstPtr!=NULL);
    assert(theFirstLocalPosPtr!=NULL);
    if (theFirstPtr==NULL)
    {
		DEBUG4("Link: No valid first object found...");
        return;
    }
    b2Body* myFirstB2BodyPtr = getB2BodyPtrFor(theFirstPtr, theFirstLocalPosPtr->toPosition());
    assert (myFirstB2BodyPtr);
    theFirstPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

    assert(theSecondPtr!=NULL);
    assert(theSecondLocalPosPtr!=NULL);
    if (theSecondPtr==NULL)
    {
		DEBUG4("Link: No valid second object found...");
        return;
    }
    b2Body* mySecondB2BodyPtr = getB2BodyPtrFor(theSecondPtr, theSecondLocalPosPtr->toPosition());
    assert (mySecondB2BodyPtr);
    theSecondPtr->addJoint(std::dynamic_pointer_cast<JointInterface>(getThisPtr()));

    // *** initialise Box2D's distance joint:
    // note: Initialize() uses a global coordinate...
    b2DistanceJointDef myJointDef;

    myJointDef.Initialize(myFirstB2BodyPtr, mySecondB2BodyPtr,
                          (theFirstPtr->getOrigCenter()+*theFirstLocalPosPtr).toB2Vec2(),
                          (theSecondPtr->getOrigCenter()+*theSecondLocalPosPtr).toB2Vec2());
    myJointDef.userData = this;
    theJointPtr = (b2RevoluteJoint*) getB2WorldPtr()->CreateJoint(&myJointDef);
}

Position Link::getTempCenter() const
{
    if (theFirstPtr==NULL || theFirstLocalPosPtr==NULL ||
            theSecondPtr==NULL || theSecondLocalPosPtr==NULL)
        return getOrigCenter();

    Vector myV1 = (theFirstPtr->getTempCenter()+*theFirstLocalPosPtr).toVector();
    Vector myV2 = (theSecondPtr->getTempCenter()+*theSecondLocalPosPtr).toVector();
    Vector myMiddle = myV1 + myV2;
    myMiddle = 0.5 * myMiddle;

    Vector myDiff = myV1 - myV2;
    double myAngle = myDiff.toAngle();

    return Position(myMiddle.toB2Vec2(), myAngle);
}


void Link::parseProperties(void)
{
    AbstractObject::parseProperties();
    if (theWorldPtr==NULL)
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
    if(theViewObjectPtr == NULL)
        return;

    // no b2body: no part of simulation
    // PROBLEM: joints also don't have a b2Body - that's why they have their own
    // overriden version of this member...
    if (theFirstPtr==NULL || theSecondPtr==NULL)
    {
        // problem
		DEBUG1("Link::updateViewObject()  while first or second is zero");
        assert(false);
    }

    // Sim running: don't need to adjust objects that are static or asleep
    ViewLink* theVLPtr = dynamic_cast<ViewLink*>(theViewObjectPtr);
    assert(theVLPtr!=NULL);

    Vector myV1 = (theFirstPtr->getTempCenter()+*theFirstLocalPosPtr).toVector();
    Vector myV2 = (theSecondPtr->getTempCenter()+*theSecondLocalPosPtr).toVector();
    theVLPtr->setEndpoints(myV1, myV2);
}
