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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "ResizeUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "AbstractObject.h"

#include <QGraphicsSceneMouseEvent>
#include <cmath>


ResizeUndoCommand::ResizeUndoCommand(
        ViewObject* anViewObjectPtr)
    : AbstractUndoCommand(anViewObjectPtr, QObject::tr("Resize"), NULL)
{
    qDebug() << Q_FUNC_INFO;


    // now it's time to figure out:
    //     are we allowed to resize horizontally
    //     or vertically, or both???
    theResizingOptions = anViewObjectPtr->getAbstractObjectPtr()->isResizable();
    qDebug() << QString("resizing options: %1").arg(theResizingOptions);
    switch (theResizingOptions)
    {
    case AbstractObject::NORESIZING:
        // I hope not - this should not be possible
        Q_ASSERT(false);
        break;
    case AbstractObject::HORIZONTALRESIZE:
        setupProxyImage("../images/ProxyResizeHori.svg");
        break;
    case AbstractObject::VERTICALRESIZE:
        setupProxyImage("../images/ProxyResizeVerti.svg");
        break;
    case AbstractObject::TOTALRESIZE:
        setupProxyImage("../images/ProxyResizeTotal.svg");
        break;
    }
}


bool ResizeUndoCommand::mouseMoveEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    QPointF myNewMousePosLocal = toLocalPos(anEventPtr->scenePos());
    qreal myDeltaLength;
    if ( theAxis&(HEIGHTBOTTOM|HEIGHTTOP))
    {
        myDeltaLength = myNewMousePosLocal.y() - theButtonDownLength;
        theNewHeight = theOrigHeight + fabs(myDeltaLength);
        theNewPos = theOrigPos + Vector(0, myDeltaLength/2.0);
    }
    else
    {
        myDeltaLength = myNewMousePosLocal.x() - theButtonDownLength;
        theNewWidth = theOrigWidth + fabs(myDeltaLength);
        theNewPos = theOrigPos + Vector(myDeltaLength/2.0, 0);
    }

    theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
    return true;
}


bool ResizeUndoCommand::mousePressEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qDebug() << Q_FUNC_INFO;

    // we need local coordinates for this one
    // (make sure to take angle into account)
    theButtonDownPosition = toLocalPos(anEventPtr->scenePos());

    // are we within 1/3 of any of the edges?
    theAxis = NONE;
    if (fabs(theButtonDownPosition.x())>theOrigWidth/6)
    {
        if(theButtonDownPosition.x()<0)
            theAxis = WIDTHLEFT;
        else
            theAxis = WIDTHRIGHT;
    }
    if (fabs(theButtonDownPosition.y())>theOrigHeight/6)
    {
        if(theButtonDownPosition.y()<0)
            theAxis |= HEIGHTBOTTOM;
        else
            theAxis |= HEIGHTTOP;
    }

    // if myAxis is empty, we're definitely not ok, let's ignore the click.
    qDebug() << QString("myAxis before filter: %1").arg(theAxis);
    if (theAxis == NONE)
        goto ignoreClick;

    // match edges with actions
    switch(theResizingOptions)
    {
    case AbstractObject::NORESIZING:
        // I hope not - this should not be possible
        Q_ASSERT(false);
        break;
    case AbstractObject::HORIZONTALRESIZE:
        if (!(theAxis & (WIDTHLEFT|WIDTHRIGHT)))
            goto ignoreClick;
        theAxis &= (WIDTHLEFT|WIDTHRIGHT);
        theButtonDownLength = theButtonDownPosition.x();
        break;
    case AbstractObject::VERTICALRESIZE:
        if (!(theAxis & (HEIGHTBOTTOM|HEIGHTTOP)))
            goto ignoreClick;
        theAxis &= (HEIGHTBOTTOM|HEIGHTTOP);
        theButtonDownLength = theButtonDownPosition.y();
        break;
    case AbstractObject::TOTALRESIZE:
        // clicked near one of the corners? - ignore
        if ( (theAxis&(HEIGHTBOTTOM|HEIGHTTOP)) &&
             (theAxis & (WIDTHLEFT|WIDTHRIGHT)))
            goto ignoreClick;
        // horizontal or vertical?
        if ( theAxis&(HEIGHTBOTTOM|HEIGHTTOP))
            theButtonDownLength = theButtonDownPosition.y();
        else
            theButtonDownLength = theButtonDownPosition.x();
        break;
    }

    // if we got here, myAxis only has one bit set.
    qDebug() << QString("myAxis after filter: %1").arg(theAxis);
    return false;

ignoreClick:
    qDebug() << "myAxis not good - ignore click";
    theAxis = NONE;
    theButtonDownLength = 0;
    deleteProxyImage();
    delete this;
    return true;
}


bool ResizeUndoCommand::mouseReleaseEvent(QGraphicsSceneMouseEvent* anEventPtr)
{
    qDebug() << Q_FUNC_INFO;
    // first, make sure the on-screen position is up-to-date
    mousePressEvent(anEventPtr);

    // now, it's time to finalize everything
    // and push the Undo on the stack
    UndoSingleton::push(this);
    deleteProxyImage();

    // we've completely handled the event, we're done
    return true;
}


QPointF ResizeUndoCommand::toLocalPos(const QPointF& aScenePos)
{
    QPointF myRelPos = aScenePos - theOrigPos.toQPointF();
    qreal myCos = cos(theOrigPos.angle);
    qreal mySin = sin(theOrigPos.angle);

    QPointF myAnswer;
    myAnswer.setX(myRelPos.x()*myCos - myRelPos.y()*mySin);
    myAnswer.setY(-myRelPos.x()*mySin - myRelPos.y()*myCos);
    return myAnswer;
}
