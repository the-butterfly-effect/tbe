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

#ifndef SCENERY_H
#define SCENERY_H

#include "RectObject.h"


/** The Scenery class is just foreground/background graphics
 *  Never moveable
 *
 *  Scenerys do not have a body or shape in simulation
 */
class Scenery : public AbstractObject
{
public:
    Scenery();

    virtual ~Scenery();

    /// Make sure there is no physics body by overriding
    /// the creational member of AbstractObject.
    virtual void createPhysicsObject(const Position &) override
    { };

    /// returns the Name of the object.
    virtual const QString getName ( ) const override
    {
        return QObject::tr("Scenery");
    }

    /// Scenery has no mass. But no b2Body will be created either :-)
    virtual b2BodyType getObjectType(void) const override
    {
        return b2_staticBody;
    }

    /// returns true if the object can be rotated by the user
    /// Scenerys cannot be adjusted by the player
    virtual bool isRotatable ( ) const override
    {
        return false;
    }
};

#endif // Scenery_H
