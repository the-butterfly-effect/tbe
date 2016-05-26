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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "Position.h"
#include "Property.h"
#include "AbstractObject.h"
#include "World.h"
#include <QtCore/QStringList>

#include "tbe_global.h"

// these are the identifiers of the properties in the XML file
// do not translate - ever!
const char *Property::BOUNCINESS_STRING  = "Bounciness";
const char *Property::FRICTION_STRING    = "Friction";
const char *Property::IMAGE_NAME_STRING  = "ImageName";
const char *Property::MASS_STRING        = "Mass";
const char *Property::NOCOLLISION_STRING = "NoCollision";
const char *Property::OBJECT_NAME_STRING = "ObjectName";
const char *Property::PHONENUMBER_STRING = "PhoneNumber";
const char *Property::PIVOTPOINT_STRING  = "PivotPoint";
const char *Property::POLYGONS_STRING    = "Polygons";
const char *Property::RADIUS_STRING      = "Radius";
const char *Property::RESIZABLE_STRING   = "Resizable";
const char *Property::ROTATABLE_STRING   = "Rotatable";
const char *Property::SPRING_CONSTANT_STRING = "SpringConstant";
const char *Property::THRUST_STRING      = "Thrust";
const char *Property::TRANSLATIONGUIDE_STRING = "TranslationGuide";
const char *Property::ZVALUE_STRING      = "ZValue";
const char *Property::OVERLAP_STRING     = "Overlap";


const char *Property::NONE_STRING        = "none";
const char *Property::HORIZONTAL_STRING  = "horizontal";
const char *Property::VERTICAL_STRING    = "vertical";
const char *Property::TOTALRESIZE_STRING = "totalresize";

const char *Property::TORQUE_STRING      = "Torque";
const char *Property::SPEED_STRING       = "Speed";
const char *Property::FORCE_STRING       = "Force";

const char *Property::S_LESSTHAN = "lessthan";
const char *Property::S_MORETHAN = "morethan";
const char *Property::S_EQUALMORE = "equalmore";
const char *Property::S_STATE_CH = "statechanged";

const char *Property::S_XCHANGED = "xchanged";
const char *Property::S_XBELOW   = "xbelow";
const char *Property::S_XOVER    = "xover";
const char *Property::S_YCHANGED = "ychanged";
const char *Property::S_YBELOW   = "ybelow";
const char *Property::S_YOVER    = "yover";
const char *Property::S_ACHANGED = "anglechanged";
const char *Property::S_ANYTHING = "anythingchanged";

const char *Property::OBJECT_STRING  = "object";
const char *Property::OBJECT1_STRING = "object1";
const char *Property::OBJECT2_STRING = "object2";
const char *Property::COLLIDE_STRING = "collide";

const char *Property::ISCHILD_STRING = "__isChild";


PropertyList::PropertyList(const PropertyList &anOriginal)
    : theProperties(anOriginal.theProperties),
      theDefaultProperties(anOriginal.theDefaultProperties)

{
    // nothing to do...
}


void PropertyList::clear (void)
{
    theProperties.clear();
}


bool PropertyList::doesPropertyExists(const QString &aKey) const
{
    return theProperties.find(aKey) != theProperties.end();
}

QStringList PropertyList::getDefaultPropertyList(void) const
{
    QStringList myProps;
    PropertyMap::const_iterator myI = theDefaultProperties.begin();
    while (myI != theDefaultProperties.end()) {
        myProps.push_back( myI.key() );
        ++myI;
    }
    return myProps;
}

void PropertyList::list(void) const
{
    DEBUG1("DEBUGGING: Requested listing of properties for object");
    PropertyMap::const_iterator myI = theProperties.begin();
    while (myI != theProperties.end()) {
        char myBuffer[256];
        strncpy (myBuffer, ASCII(myI.key()), 255);
        DEBUG1("  '%s' = '%s'", myBuffer, ASCII(myI.value()));
        ++myI;
    }
}


bool PropertyList::property2Bool(const QString &aPropertyName,
                                 bool *aBool,
                                 bool useDefault) const
{
    bool myResult;

    // we run two passes here:
    // 1) use getPropertyNoDefault
    // 2) use getDefaultProperty
    // 3) a third pass indicates trouble and we return false

    int myPass = 1;
    QString myValue = getPropertyNoDefault(aPropertyName).toLower().trimmed();
    while (myPass < 3) {
        if (myValue.isEmpty() && useDefault == false)
            return false;

        // if myValue is empty, none of these will ever trigger
        // if myValue is nonempty and we parse, we're happy
        if (myValue == "true") {
            myResult = true;
            break;
        }
        if (myValue == "yes") {
            myResult = true;
            break;
        }
        if (myValue == "false") {
            myResult = false;
            break;
        }
        if (myValue == "no") {
            myResult = false;
            break;
        }

        // ok, myValue was empty or unparsed. let's get the
        // default and go again...
        myValue = getDefaultProperty(aPropertyName).toLower();
        myPass++;
    }
    if (myPass >= 3)
        return false;

    *aBool = myResult;
    return true;
}

bool PropertyList::property2Float(const QString &aPropertyName,
                                  float *aFloat,
                                  bool useDefault) const

{
    QString myValue = getPropertyNoDefault(aPropertyName);
    if (myValue.isEmpty() && useDefault == false)
        return false;

    bool isOK = false;
    float myFloat = myValue.toFloat(&isOK);
    if (isOK == false) {
        myFloat = getDefaultProperty(aPropertyName).toFloat(&isOK);
        if (isOK == false)
            return false;
    }

    *aFloat = myFloat;
    return true;
}


AbstractObjectPtr PropertyList::property2ObjectPtr(
    World *aWPtr,
    const QString &aPropertyName)
{
    QString myValue = getPropertyNoDefault(aPropertyName);
    if (myValue.isEmpty())
        return nullptr;
    return aWPtr->findObjectByID(myValue);
}


AbstractObjectPtr PropertyList::property2ObjectPlusVectorPtr(
    World *aWPtr,
    const QString &aPropertyName,
    Vector **aVectorPtrPtr)
{
    QStringList myStrings = getPropertyNoDefault(aPropertyName).split("@");
    if (myStrings.count() > 2 || myStrings.count() == 0)
        return nullptr;

    AbstractObjectPtr myAOPtr = aWPtr->findObjectByID(myStrings[0]);
    if (myAOPtr == nullptr)
        return nullptr;

    Vector *myVPtr = new Vector();
    if (myStrings.count() == 2) {
        if (myVPtr->fromString(myStrings[1]) == false) {
            delete myVPtr;
            myVPtr = nullptr;
            myAOPtr = nullptr;
        }
    }

    *aVectorPtrPtr = myVPtr;
    return myAOPtr;
}


bool PropertyList::property2String(const QString &aPropertyName, QString *aString,
                                   bool useDefault) const
{
    QString myValue = getPropertyNoDefault(aPropertyName);
    if (myValue.isEmpty()) {
        if (useDefault == false)
            return false;
        myValue = getDefaultProperty(aPropertyName);
        if (myValue.isEmpty())
            return false;
    }

    *aString = myValue;
    return true;
}

bool PropertyList::property2Vector(const QString &aPropertyName, Vector *aPosition,
                                   bool useDefault) const
{
    QString myValue = getPropertyNoDefault(aPropertyName).trimmed();
    if (myValue.isEmpty() && useDefault == false)
        return false;

    Vector myVector;
    bool isOK = myVector.fromString(myValue);
    if (isOK) {
        *aPosition = myVector;
        return true;
    }

    myValue = getDefaultProperty(aPropertyName).trimmed();
    if (myValue.isEmpty())
        return false;
    isOK = myVector.fromString(myValue);
    if (isOK) {
        *aPosition = myVector;
        return true;
    }

    return false;
}


void PropertyList::setDefaultPropertiesString(const QString &aSeparableString)
{
    if (aSeparableString.isEmpty())
        return;
    QStringList myKeyValues = aSeparableString.split("/");
    QStringList::iterator myI = myKeyValues.begin();

    while ((myI != myKeyValues.end()) && ((*myI).isNull() == false)) {
        if ((*myI).contains(":") != false) {
            QStringList myKV = (*myI).split(":");
            QString myKey   = myKV[0];
            QString myValue = myKV[1];
            if (myKey.startsWith("-")) {
                theDefaultProperties.remove(myKey.remove(0, 1));
                goto next;
            }
            if (theDefaultProperties.contains(myKey))
                theDefaultProperties.remove(myKey);
            theDefaultProperties.insert(myKey, myValue);
        }
next:
        ++myI;
    }
}


void PropertyList::setProperty(const QString &aKey, bool aValue)
{
    theProperties[aKey] = aValue ? "true" : "false";
}

void PropertyList::setProperty(const QString &aKey, const char *aValue)
{
    theProperties[aKey] = aValue;
}

void PropertyList::setProperty(const QString &aKey, const float aValue)
{
    theProperties[aKey] = QString::number(aValue);
}

void PropertyList::setProperty(const QString &aKey, const QString &aValue)
{
    theProperties[aKey] = aValue;
}



bool operator==(const PropertyList &p1, const PropertyList &p2)
{
    // thanks to QMap<> for already including operator==
    return p1.theProperties == p2.theProperties;
}

bool operator!=(const PropertyList &p1, const PropertyList &p2)
{
    // thanks to QMap<> for already including operator!=
    return p1.theProperties != p2.theProperties;
}
