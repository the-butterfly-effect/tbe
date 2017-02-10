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

#ifndef LINK_H
#define LINK_H

#include "AbstractObject.h"
#include "AbstractJoint.h"
#include "Position.h"

#include <QObject>

/// The Link class is a joint, it links two objects (bodies) together
/// whilst keeping the connection points at a constant distance
///
/// <!-- the x/y/angle in the object tag is ignored and/or recalculated when needed -->
/// <object type="Link" X="2.74" Y="3.44" angle="2.12">
///    <property key="object1">Skyhook@(1,2)</property>
///    <property key="object2">Chain3@(-1,2)</property>
///    <property key="ImageName">flat-chain</property>
/// </object>

class Link : public AbstractJoint
{
public:
    Link(void);

    virtual ~Link();

    /// (overridden from AbstractObject to remove extra sharedptrs)
    void clearObjectReferences() override;

    /** (overridden from AbstractJoint to fixup aspect ratio and overlap)
      * @returns pointer to ViewItem
      */
    ViewItem* createViewItem(float aDefaultDepth = 2.0) override;

    /// overridden from AbstractObject
    /// (this class does not have a body, only a joint)
    void createPhysicsObject(void) override;

    /// overridden from AbstractObject
    /// returns the Name of the object.
    const QString getName ( ) const override
    {
        return QObject::tr("Link");
    }

    /** Get the current center position of the object.
     * @return the value of theCenter
     */
    Position getTempCenter ( ) const override;

    qreal getTempWidth() const override
    { return theLength; }

    qreal getTempHeight() const override
    { return 0.04; }

    /// overridden from AbstractObject
    /// returns true if the object can be rotated by the user
    bool isRotatable ( ) const override
    {
        return false;
    }

    /// overridden from AbstractObject
    /// parses all properties that Link understands
    void  parseProperties(void) override;

    /// implemented from BaseJoint
    void updateOrigCenter(void) override;

private:
    AbstractObjectPtr theFirstPtr;
    AbstractObjectPtr theSecondPtr;
    Vector     *theFirstLocalPosPtr;
    Vector     *theSecondLocalPosPtr;

    /// contains the length, which will be returned by getTempWidth().
    mutable qreal       theLength;
};

#endif // LINK_H
