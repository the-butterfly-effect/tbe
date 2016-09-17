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


ViewWorldItem::ViewWorldItem(QQuickItem *aParentPtr)
    : QQuickItem(aParentPtr),
      theWorldPtr(nullptr)
{

}

ViewWorldItem::~ViewWorldItem()
{
    DEBUG3ENTRY;
}

void ViewWorldItem::setWorldPtr(World *&aWorldPtr)
{
    // TODO/FIXME: for now, we don't accept loading a new World into the existing object
    assert (nullptr == theWorldPtr);
    assert (nullptr != aWorldPtr);
    theWorldPtr = aWorldPtr;
    setupBackground();
}


void ViewWorldItem::setupBackground(void)
{
    if (theWorldPtr->theBackground.theBackgroundGradient.count() == 0 &&
            theWorldPtr->theBackground.theImageName.isEmpty()) {
        // the default if nothing else specified: a blue gradient background
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
    }
/*
    setBackgroundBrush(Qt::blue);
    QLinearGradient myBackground(0, 0, 0, -getHeight());
    foreach (Background::GradientStop myGS, theWorldPtr->theBackground.theBackgroundGradient)
        myBackground.setColorAt(myGS.thePosition, QColor(
                                    static_cast<int>(myGS.theR * 255),
                                    static_cast<int>(myGS.theG * 255),
                                    static_cast<int>(myGS.theB * 255),
                                    static_cast<int>(myGS.theAlpha * 255)));
    setBackgroundBrush(myBackground);

    // Set a very light 1-pixel pen line on the left and bottom borders.
    // This will prevent the player to move objects outside the borders.
    QPen myPen(QColor(255, 255, 255, 1), 0);
    addLine(0, 0, 0, -4 * getHeight(), myPen);
    addLine(0, 0, 4 * getWidth(), 0, myPen);
*/
}
