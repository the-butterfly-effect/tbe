/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012 Klaas van Gend
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
#include "Position.h"
#include "ViewItem.h"
#include "ViewWorldItem.h"
#include "World.h"

#include "tbe_global.h"

static ViewWorldItem* theVWIPtr = nullptr;

ViewWorldItem::ViewWorldItem(QQuickItem *aParentPtr)
    : QQuickItem(aParentPtr),
      theWorldPtr(nullptr)
{
    assert(nullptr == theVWIPtr);
    theVWIPtr = this;
}

ViewWorldItem::~ViewWorldItem()
{
    DEBUG3ENTRY;
    assert(this == theVWIPtr);
    theVWIPtr = nullptr;
}

ViewWorldItem *ViewWorldItem::me()
{
    assert(nullptr != theVWIPtr);
    return theVWIPtr;
}

void ViewWorldItem::setWorldPtr(World *aWorldPtr)
{
    // TODO/FIXME: for now, we don't accept loading a new World into the existing object
    assert (nullptr == theWorldPtr);
    assert (nullptr != aWorldPtr);
    theWorldPtr = aWorldPtr;
    setupBackground();
}


void ViewWorldItem::setupBackground(void)
{
    DEBUG1ENTRY;
    assert(nullptr != theWorldPtr);
    if (theWorldPtr->theBackground.theBackgroundGradient.count() == 0 &&
            theWorldPtr->theBackground.theImageName.isEmpty()) {
        // the default if nothing else specified: a blue gradient background
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
    }
    QVariantMap map;
    for (auto i : theWorldPtr->theBackground.theBackgroundGradient) {
        map.insert(QString("%1").arg(i.thePosition), QColor(i.theR*255, i.theG*255, i.theB*255, i.theAlpha*255));
    }
    QMetaObject::invokeMethod(this, "insertGradient",
                              Q_ARG(QVariant, QVariant::fromValue(map)));
}
