/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#include <QGraphicsSceneMouseEvent>
#include <cmath>

#include "ResizeUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "AbstractObject.h"

ResizeUndoCommand::ResizeUndoCommand(
    ViewObjectPtr anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("Resize %1"), nullptr),
      theButtonDownLength(999.f)
{
    DEBUG3ENTRY;

    // now it's time to figure out:
    //     are we allowed to resize horizontally
    //     or vertically, or both???
    if (theIsLevelCreator)
        theResizingOptions = AbstractObject::TOTALRESIZE;
    else
        theResizingOptions = anViewObjectPtr->getAbstractObjectPtr()->isResizable();
    //theResizingOptions = AbstractObject::TOTALRESIZE;
    switch (theResizingOptions) {
    case AbstractObject::NORESIZING:
        // I hope not - this should not be possible
        Q_ASSERT(false);
        break;
    case AbstractObject::HORIZONTALRESIZE:
        setDecoratorImage("ProxyResizeHori");
        break;
    case AbstractObject::VERTICALRESIZE:
        setDecoratorImage("ProxyResizeVerti");
        break;
    case AbstractObject::TOTALRESIZE:
        setDecoratorImage("ProxyResizeTotal");
        break;
    }
}


bool ResizeUndoCommand::basicMoveEvent(const Position &aNewPos, qreal aNewWidth, qreal aNewHeight)
{
    bool myReturnvalue = true;
    // only commit local values if they are larger than minimum size
    // NOTE: TODO/FIXME: the below code isn't good enough
    if (aNewWidth > AbstractObject::MINIMUM_DIMENSION &&
            aNewHeight > AbstractObject::MINIMUM_DIMENSION) {
        theNewPos = aNewPos;
        theNewWidth = aNewWidth;
        theNewHeight = aNewHeight;
        theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
        myReturnvalue = true;
    } else {
        myReturnvalue = false;
    }
    theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
    setDecoratorStateMouseMove();
    return myReturnvalue;
}


bool ResizeUndoCommand::mouseMoveEvent(QGraphicsSceneMouseEvent *anEventPtr)
{
    QPointF myNewMousePosLocal = toLocalPos(anEventPtr->scenePos());
    qreal myDeltaLength; // in world coordinates

    // first create local values
    Position myNewPos;
    qreal    myNewWidth = theNewWidth;
    qreal    myNewHeight = theNewHeight;

    if ( theAxis & (HEIGHTBOTTOM | HEIGHTTOP)) {
        myDeltaLength = (myNewMousePosLocal.y() - theButtonDownLength) / THESCALE;
        myNewPos = theOrigPos + Vector(0, myDeltaLength / 2.0);
        if (theAxis & HEIGHTBOTTOM)
            myDeltaLength = -myDeltaLength;
        myNewHeight = theOrigHeight + myDeltaLength;
    } else {
        myDeltaLength = (myNewMousePosLocal.x() - theButtonDownLength) / THESCALE;
        myNewPos = theOrigPos + Vector(myDeltaLength / 2.0, 0);
        if (theAxis & WIDTHLEFT)
            myDeltaLength = -myDeltaLength;
        myNewWidth = theOrigWidth + myDeltaLength;
    }
    return basicMoveEvent(myNewPos, myNewWidth, myNewHeight);
}

bool ResizeUndoCommand::mousePressEvent(QGraphicsSceneMouseEvent *anEventPtr)
{
    DEBUG3ENTRY;

    // we need local coordinates for this one
    // (make sure to take angle into account)
    theButtonDownPosition = toLocalPos(anEventPtr->scenePos());

    // are we within 1/3 of any of the edges?
    theAxis = NONE;
    if (fabs(theButtonDownPosition.x()) / THESCALE > theOrigWidth / 6) {
        if (theButtonDownPosition.x() < 0)
            theAxis = WIDTHLEFT;
        else
            theAxis = WIDTHRIGHT;
    }
    if (fabs(theButtonDownPosition.y()) / THESCALE > theOrigHeight / 6) {
        if (theButtonDownPosition.y() < 0)
            theAxis |= HEIGHTBOTTOM;
        else
            theAxis |= HEIGHTTOP;
    }

    // if myAxis is empty, we're definitely not ok, let's ignore the click.
    if (theAxis == NONE)
        goto ignoreClick;

    // match edges with actions
    switch (theResizingOptions) {
    case AbstractObject::NORESIZING:
        // I hope not - this should not be possible
        Q_ASSERT(false);
        break;
    case AbstractObject::HORIZONTALRESIZE:
        if (!(theAxis & (WIDTHLEFT | WIDTHRIGHT)))
            goto ignoreClick;
        theAxis &= (WIDTHLEFT | WIDTHRIGHT);
        theButtonDownLength = theButtonDownPosition.x();
        break;
    case AbstractObject::VERTICALRESIZE:
        if (!(theAxis & (HEIGHTBOTTOM | HEIGHTTOP)))
            goto ignoreClick;
        theAxis &= (HEIGHTBOTTOM | HEIGHTTOP);
        theButtonDownLength = theButtonDownPosition.y();
        break;
    case AbstractObject::TOTALRESIZE:
        // clicked near one of the corners? - ignore
        if ( (theAxis & (HEIGHTBOTTOM | HEIGHTTOP)) &&
                (theAxis & (WIDTHLEFT | WIDTHRIGHT)))
            goto ignoreClick;
        // horizontal or vertical?
        if ( theAxis & (HEIGHTBOTTOM | HEIGHTTOP))
            theButtonDownLength = theButtonDownPosition.y();
        else
            theButtonDownLength = theButtonDownPosition.x();
        break;
    }

    // if we got here, myAxis only has one bit set.
    return false;

ignoreClick:
    theAxis = NONE;
    theButtonDownLength = 0;
    setDecoratorStateUndoRedo();
    delete this;
    return true;
}


QPointF ResizeUndoCommand::toLocalPos(const QPointF &aScenePos)
{
    QPointF myRelPos = aScenePos - theOrigPos.toQPointF();
    qreal myCos = cos(theOrigPos.angle);
    qreal mySin = sin(theOrigPos.angle);

    QPointF myAnswer;
    myAnswer.setX(myRelPos.x()*myCos - myRelPos.y()*mySin);
    myAnswer.setY(-myRelPos.x()*mySin - myRelPos.y()*myCos);
    return myAnswer;
}
