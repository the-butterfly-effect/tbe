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

#include "Scenery.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "ViewObject.h"
#include "Property.h"
#include "ObjectFactory.h"

// this class' ObjectFactory
class SceneryObjectFactory : public ObjectFactory
{
public:
    SceneryObjectFactory(void)
    {
        announceObjectType("Scenery", this);
    }
    AbstractObject *createObject(void) const override
    {
        return fixObject(new Scenery());
    }
};
static SceneryObjectFactory theFactory;



Scenery::Scenery( ) : AbstractObject()
{
    // only keep DESCRIPTION, IMAGE_NAME and ZVALUE
    theProps.setDefaultPropertiesString(
        QString("-")   + Property::BOUNCINESS_STRING  +
        QString(":/-") + Property::FRICTION_STRING    +
        QString(":/-") + Property::PIVOTPOINT_STRING  +
        QString(":/-") + Property::RESIZABLE_STRING   +
        QString(":/-") + Property::TRANSLATIONGUIDE_STRING   +
        QString(":/-") + Property::NOCOLLISION_STRING   +
        QString(":/-") + Property::ROTATABLE_STRING   +
        QString(":/")  + Property::ZVALUE_STRING + ":0.1/" );

    DEBUG5("Scenery::Scenery done");
}


Scenery::~Scenery( )
{
    ;
}

