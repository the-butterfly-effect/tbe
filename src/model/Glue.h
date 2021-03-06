/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef GLUE_H
#define GLUE_H

#include "AbstractObject.h"
#include "AbstractJoint.h"
#include "Position.h"

#include <QObject>

// forward declarations
class b2DistanceJoint;


/// The Glue class is a joint, it links two objects (bodies) together.
/// Due to using a distant joint and a revolute joint, both objects are really
/// locked together - unmovably.
///
/// <!-- the x/y/angle in the object tag are ignored and recalculated,
///      the width/height are used to display the image and distance the two joints-->
/// <object type="Glue" X="2.74" Y="3.44" width="0.1" height="0.1" angle="2.12">
///    <property key="object1">Bar@(-0.75,0)</property>
///    <property key="object2">Stop</property>
///    <property key="ImageName">GlueBlob</property>
/// </object>
class Glue : public AbstractJoint
{
public:
    Glue();

    virtual ~Glue();

    /// (overridden from AbstractObject to remove extra sharedptrs)
    virtual void clearObjectReferences() override;

    /// overridden from AbstractObject
    /// (this class does not have a body, only two joints)
    void createPhysicsObject(void) override;

    /// overridden from AbstractObject
    /// returns the Name of the object.
    const QString getName ( ) const override
    {
        return QObject::tr("Glue");
    }

    /** Get the current center position of the object.
     * @return the value of theCenter
     */
    Position getTempCenter ( ) const override;

    /// overridden from AbstractObject
    /// returns true if the object can be rotated by the user
    bool isRotatable ( ) const override
    {
        return false;
    }

    /// overridden from AbstractObject
    /// parses all properties that Glue understands
    /// - only "object1" and "object2"
    void  parseProperties(void) override;

    /// implemented from AbstractJoint
    void updateOrigCenter(void) override;

private:
    AbstractObjectPtr theFirstPtr;
    AbstractObjectPtr theSecondPtr;
    Vector     *theFirstLocalPosPtr;
    Vector     *theSecondLocalPosPtr;

    b2DistanceJoint *theLinkPtr;
};

#endif // GLUE_H
