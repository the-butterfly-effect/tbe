/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "tbe_global.h"
#include "AbstractObject.h"
#include "Goal.h"
#include "Pingus.h"
#include "Position.h"
#include "Property.h"
#include "World.h"
#include "GoalEditor.h"

#include <QStringList>

Goal::Goal() : isFail(false)
{
    // nothing to do here
}

Goal::~Goal()
{
    // nothing to do here
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////// GoalDistance //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalDistance::GoalDistance()
    : theType(NOTYPE), theLimit(0.0f), theFirstPtr(nullptr), theSecondPtr(nullptr)
{
    // nothing to do here
}

GoalDistance::~GoalDistance()
{
    // nothing to do here
}

bool GoalDistance::checkForSuccess(void)
{
    // whine loudly in a debug build or softly in a release build
    assert(theFirstPtr != nullptr);
    assert(theSecondPtr != nullptr);
    if (theFirstPtr == nullptr || theSecondPtr == nullptr)
        return false;

    Vector myDistanceVector = (theFirstPtr->getTempCenter().toVector())
                              - (theSecondPtr->getTempCenter().toVector());
    switch (theType) {
    case NOTYPE:
        return false;
    case LESSTHAN:
        if (myDistanceVector.length() < theLimit)
            return true;
        break;
    case MORETHAN:
        if (myDistanceVector.length() > theLimit)
            return true;
        break;
    }
    return false;
}

bool GoalDistance::parseProperties(World *aWPtr)
{
    assert(aWPtr != nullptr);
    if (aWPtr == nullptr)
        return false;

    // there are 4 types of properties: ("lessthan" OR "morethan") AND "object1" AND "object2"
    // but we expect only to have 3 properties - any other number is wrong
    if (theProps.getPropertyCount() != 3) {
        DEBUG2("wrong number of properties at beginning of parseProperties - not good");
        return false;
    }


    if (theProps.property2Float(Property::S_LESSTHAN, &theLimit, false))
        theType = LESSTHAN;
    if (theProps.property2Float(Property::S_MORETHAN, &theLimit, false))
        theType = MORETHAN;
    if (theType == NOTYPE) {
        DEBUG2("no valid property found");
        return false;
    }


    // parse object1 and object2
    theFirstPtr = theProps.property2ObjectPtr(aWPtr, Property::OBJECT1_STRING);
    if (theFirstPtr == nullptr) {
        DEBUG2("%s is not an existing, valid object", Property::OBJECT1_STRING);
        return false;
    }
    theSecondPtr = theProps.property2ObjectPtr(aWPtr, Property::OBJECT2_STRING);
    if (theSecondPtr == nullptr) {
        DEBUG2("%s is not an existing, valid object", Property::OBJECT2_STRING);
        return false;
    }

    return true;
}

QString GoalDistance::goalToStringList() const
{
    // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
    QString myString = QString("%1;%2;%3;%4;%5")
                       .arg(GoalSerializer::getColumnZero()[GoalSerializer::DISTANCE])
                       .arg(theFirstPtr->getID())
                       .arg(theType == MORETHAN ? ">" : "<")
                       .arg(QString::number(theLimit))
                       .arg(theSecondPtr->getID());
    return myString;
}



//////////////////////////////////////////////////////////////////////////////
////////////////////////////// GoalPositionChange ////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalPositionChange::GoalPositionChange()
    : theType(NOTYPE), theBOPtr(nullptr), theLimit(0.0f)
{
    // nothing to do here
}

GoalPositionChange::~GoalPositionChange()
{
    // nothing to do here
}

bool GoalPositionChange::checkForSuccess(void)
{
    // whine loudly in a debug build or softly in a release build
    assert(theBOPtr != nullptr);
    if (theBOPtr == nullptr)
        return false;

    Position myNewPos = theBOPtr->getTempCenter();
    Position myOldPos = theBOPtr->getOrigCenter();
    const float MINCHANGE = 0.01;
    switch (theType) {
    case NOTYPE:
        return false;
    case XCHANGED:
        if (fabs(myNewPos.x - myOldPos.x) > MINCHANGE)
            return true;
        break;
    case XBELOW:
        if (myNewPos.x < theLimit)
            return true;
        break;
    case XOVER:
        if (myNewPos.x > theLimit)
            return true;
        break;
    case YCHANGED:
        if (fabs(myNewPos.y - myOldPos.y) > MINCHANGE)
            return true;
        break;
    case YBELOW:
        if (myNewPos.y < theLimit)
            return true;
        break;
    case YOVER:
        if (myNewPos.y > theLimit)
            return true;
        break;
    case ANGLECHANGED:
        if (fabs(myNewPos.angle - myOldPos.angle) > MINCHANGE)
            return true;
        break;
    case ANYTHINGCHANGED:
        if (myNewPos == myOldPos)
            return false;
        return true;
    }
    return false;
}

bool GoalPositionChange::parseProperties(World *aWPtr)
{
    assert(aWPtr != nullptr);
    if (aWPtr == nullptr)
        return false;

    // there are 5 types of properties: (xchanged/ychanged/anglechanged/anythingchanged AND object)
    // but we expect only to have 2 properties - any other number is wrong
    if (theProps.getPropertyCount() != 2) {
        DEBUG2("wrong number of properties at beginning of parseProperties - not good");
        return false;
    }


    // parse *changed - no value, only a key
    if (theProps.doesPropertyExists(Property::S_XCHANGED))
        theType = XCHANGED;
    if (theProps.property2Float(Property::S_XBELOW, &theLimit, false))
        theType = XBELOW;
    if (theProps.property2Float(Property::S_XOVER, &theLimit, false))
        theType = XOVER;
    if (theProps.doesPropertyExists(Property::S_YCHANGED))
        theType = YCHANGED;
    if (theProps.property2Float(Property::S_YBELOW, &theLimit, false))
        theType = YBELOW;
    if (theProps.property2Float(Property::S_YOVER, &theLimit, false))
        theType = YOVER;
    if (theProps.doesPropertyExists(Property::S_ACHANGED))
        theType = ANGLECHANGED;
    if (theProps.doesPropertyExists(Property::S_ANYTHING))
        theType = ANYTHINGCHANGED;
    if (theType == NOTYPE) {
        DEBUG2("no valid property found");
        return false;
    }

    // parse object
    theBOPtr = theProps.property2ObjectPtr(aWPtr, Property::OBJECT_STRING);
    if (theBOPtr == nullptr) {
        DEBUG2("%s is not an existing, valid object", Property::OBJECT_STRING);
        return false;
    }
    return true;
}


QString GoalPositionChange::goalToStringList() const
{
    QString myVariable;
    QString myCondition;
    QString myLimit;

    if ((theType & 0x03) == 1)
        myCondition = GoalEditor::getT10nOf_change();
    if ((theType & 0x03) == 2)
        myCondition = "<";
    if ((theType & 0x03) == 3)
        myCondition = ">";

    if ((theType & 0x03) == 2 || (theType & 0x03) == 3)
        myLimit = QString::number(theLimit);

    // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is not present here)
    QString myString = QString("%1;%2;%3;%4;")
                       .arg(GoalSerializer::getColumnZero()[theType / 4])
                       .arg(theBOPtr->getID())
                       .arg(myCondition)
                       .arg(myLimit);
    return myString;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// GoalStateChange /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalStateChange::GoalStateChange()
    : theType(NOTYPE), theBOPtr(nullptr), theState(0)
{
    // nothing to do here
}

GoalStateChange::~GoalStateChange()
{
    // nothing to do here
}

bool GoalStateChange::checkForSuccess(void)
{
    // whine loudly in a debug build or softly in a release build
    assert(theBOPtr != nullptr);
    if (theBOPtr == nullptr)
        return false;

    // TODO/FIXME: theImageIndex isn't exactly the state, but for now it is...
    int myState = theBOPtr->getImageIndex();

    switch (theType) {
    case NOTYPE:
        return false;
    case STATECHANGE:
        if (myState != theState)
            return true;
        break;
    case STATEOVER:
        if (myState > theState)
            return true;
        break;
    }
    return false;
}

bool GoalStateChange::parseProperties(World *aWPtr)
{
    assert(aWPtr != nullptr);
    if (aWPtr == nullptr)
        return false;

    // we expect only to have 2 properties - any other number is wrong
    if (theProps.getPropertyCount() != 2) {
        DEBUG2("wrong number of properties at beginning of parseProperties - not good");
        return false;
    }

    if (theProps.doesPropertyExists(Property::S_STATE_CH))
        theType = STATECHANGE;
    // because we don't have a property2Int, we have to fake it:
    float myState;
    if (theProps.property2Float(Property::S_MORETHAN, &myState, false))
        theType = STATEOVER;
    theState = myState;
    if (NOTYPE == theType) {
        DEBUG2("no valid property found");
        return false;
    }


    // parse object
    theBOPtr = theProps.property2ObjectPtr(aWPtr, Property::OBJECT_STRING);
    if (nullptr == theBOPtr) {
        DEBUG2("%s is not an existing, valid object", Property::OBJECT_STRING);
        return false;
    }

    // in case of S_STATE_CH, we need to store the current state
    if (STATECHANGE == theType)
        // TODO/FIXME for newgui, for now, with no state at all:
//      theState = theBOPtr->getImageIndex();
        theState = 0;
    return true;
}

QString GoalStateChange::goalToStringList() const
{
    QString myString;
    switch (theType) {
    case NOTYPE:
        myString = ";;;;";
        break;
    case STATECHANGE:
        // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is not present here)
        myString = QString("%1;%2;%3;;")
                   .arg(GoalSerializer::getColumnZero()[GoalSerializer::STATE])
                   .arg(theBOPtr->getID())
                   .arg(GoalEditor::getT10nOf_change());
        break;
    case STATEOVER:
        // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is not present here)
        myString = QString("%1;%2;%3;%4;")
                   .arg(GoalSerializer::getColumnZero()[GoalSerializer::STATE])
                   .arg(theBOPtr->getID())
                   .arg(">")
                   .arg(QString::number(theState));
        break;
    }
    return myString;
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////// GoalExitedPingusCounter /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GoalEscapedPingusCounter::GoalEscapedPingusCounter()
    : theLimit(0)
{
    // nothing to do here
}

GoalEscapedPingusCounter::~GoalEscapedPingusCounter()
{
    // nothing to do here
}

bool GoalEscapedPingusCounter::checkForSuccess(void)
{
    if (isFail == false && Pingus::getEscapedPingusCount() > theLimit - 0.01)
        return true;
    if (isFail == true && Pingus::getAlivePingusCount() < theLimit - 0.01)
        return true;
    return false;
}

bool GoalEscapedPingusCounter::parseProperties(World *aWPtr)
{
    assert(aWPtr != nullptr);

    const char *myCompare = Property::S_EQUALMORE;
    if (isFail)
        myCompare = Property::S_LESSTHAN;

    if (nullptr == aWPtr) {
        DEBUG2("GoalEscapedPingusCounter::parseProperties() aWPtr doesn't exist?");
        return false;
    }

    // we expect only to have 1 properties - any other number is wrong
    if (theProps.getPropertyCount() != 1) {
        DEBUG2("GoalEscapedPingusCounter::parseProperties() got %d properties (>1), not good",
               theProps.getPropertyCount());
        return false;
    }
    theProps.list();
    if (!theProps.doesPropertyExists(myCompare)) {
        DEBUG2("GoalEscapedPingusCounter::parseProperties() didn't get %s property", myCompare);
        return false;
    }
    theProps.property2Float(myCompare, &theLimit, false);
    return true;
}

QString GoalEscapedPingusCounter::goalToStringList() const
{
    QString myString;
    const char *myCompare;
    if (isFail)
        myCompare = "<";
    else
        myCompare = ">=";
    myString = QString("%1;;%2;%3;")
               .arg(GoalSerializer::getColumnZero()[GoalSerializer::ESCPINGUS])
               .arg(myCompare)
               .arg(QString::number(theLimit));
    return myString;
}
