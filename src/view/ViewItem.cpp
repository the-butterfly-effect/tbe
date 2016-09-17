/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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
#include "ResizeAwareQuickWidget.h"
#include "ViewItem.h"

ViewItem::ViewItem(QQuickItem *aParentPtr)
                       : QQuickItem(aParentPtr)
{
    // Nothing to do here...
}

#if 0
// TODO: create a destructor, because we need to take the AO from the world upon destruction.

void
ViewItem::adjustObjectDrawingFromAO()
{
    assert(theAOPtr!=nullptr);

    // convert width and height from SI to pixels and set them
    QSize mySize = Vector(theAOPtr->theWidth, theAOPtr->theHeight).toQSize();
    parentItem()->setWidth(mySize.width());
    parentItem()->setHeight(mySize.height());

    // convert center position from SI to pixels and set the top-left position
    parentItem()->setRotation(theAOPtr->theCenter.angleInDegrees());
    QPointF myCenter = theAOPtr->theCenter.toQPointF();
    qreal myHalfW =  mySize.width() / 2.;
    qreal myHalfH =  mySize.height() / 2.;
    parentItem()->setX(myCenter.x() - myHalfW);
    parentItem()->setY(myCenter.y() - myHalfH);

    // TODO: Frame number

    // and make sure that we are updated whenever the UI changes a parameter,
    // so we can update the AO and check for collisions
    connect (parentItem(), SIGNAL(heightChanged()), this, SLOT(parentParamChanged()));
    connect (parentItem(), SIGNAL(widthChanged()),  this, SLOT(parentParamChanged()));
    connect (parentItem(), SIGNAL(xChanged()),      this, SLOT(parentParamChanged()));
    connect (parentItem(), SIGNAL(yChanged()),      this, SLOT(parentParamChanged()));
    connect (parentItem(), SIGNAL(zChanged()),      this, SLOT(parentParamChanged()));
    connect (parentItem(), SIGNAL(rotationChanged()),this,SLOT(parentParamChanged()));
}


ViewItem *ViewItem::findVIinVO(QQuickItem *anVOPtr)
{
    assert (anVOPtr!=nullptr);
    ViewItem* myVIPtr = nullptr;
    QList<QQuickItem*> myChilds = anVOPtr->childItems();
    for (auto I : myChilds) {
        if (I->objectName() == "theVI")
            myVIPtr = qobject_cast<ViewItem*>(I);
    }
    return myVIPtr;
}


void ViewItem::setAbstractObjectPtr(AbstractObject *anAOPtr)
{
    assert (anAOPtr != nullptr);
    theAOPtr = anAOPtr;

    // TODO: retrieve image info
    //

    // TODO: update isHResize / isVResize / isRotate
    rand();
    parentItem()->setProperty("isHResize", true); //rand() % 2);
    parentItem()->setProperty("isVResize", true); // rand() % 2);
    parentItem()->setProperty("isRotate", true); // rand() % 2);

    adjustObjectDrawingFromAO();
}
#endif
