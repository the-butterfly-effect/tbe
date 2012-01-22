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

#include "AbstractUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"

AbstractUndoCommand::AbstractUndoCommand(
        ViewObject* anViewObjectPtr,
        const QString& anUndoName,
        QUndoCommand* parent)
    : QUndoCommand(parent),
      theViewObjPtr(anViewObjectPtr)
{
    AbstractObject* myObjectPtr = theViewObjPtr->getAbstractObjectPtr();

    // This is the undo action: %1 will contain e.g. "Move"
    // and %2 might contain BowlingBall
    setText( QString("%1 %2").arg(anUndoName)
             .arg(myObjectPtr->getName()) );
    theOrigPos    = theNewPos    = myObjectPtr->getOrigCenter();
    theOrigWidth  = theNewWidth  = myObjectPtr->getTheWidth();
    theOrigHeight = theNewHeight = myObjectPtr->getTheHeight();
}


AbstractUndoCommand::~AbstractUndoCommand()
{
    DEBUG1ENTRY;
}


void AbstractUndoCommand::commit(void)
{
    DEBUG4ENTRY;
    UndoSingleton::push(this);
    setDecoratorStateUndoRedo();
}


bool AbstractUndoCommand::isViewObjectColliding(void)
{
    // if theVOADPtr exists, we probably have at least one
    // object colliding. Let's check.
    int myCollisionCount = 0;
    QList<QGraphicsItem*> myCollidingItems = theViewObjPtr->collidingItems ( Qt::IntersectsItemShape );
    foreach(QGraphicsItem* i, myCollidingItems)
    {
        if (i != &(theViewObjPtr->theDecorator))
            myCollisionCount++;
    }
    return myCollisionCount>0;
 }


bool AbstractUndoCommand::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    // It's time to finalize everything
    // and push the Undo on the stack
    commit();

    // we've completely handled the event, we're done
    return true;
}


void AbstractUndoCommand::redo(void)
{
    qDebug() << Q_FUNC_INFO << text();
    theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
    setDecoratorStateUndoRedo();
}


void AbstractUndoCommand::setDecoratorStateMouseMove(void)
{
    if (isViewObjectColliding())
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::COMBINED);
    else
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::PROXY);
}


void AbstractUndoCommand::setDecoratorStateUndoRedo(void)
{
    if (isViewObjectColliding())
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::CROSS);
    else
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::NONE);
}


void AbstractUndoCommand::setDecoratorImage(const QString& anImageName)
{
    Q_ASSERT(theViewObjPtr!=NULL);
    theViewObjPtr->theDecorator.setDecoratorImage(anImageName, this);
    setDecoratorStateMouseMove();
}


void AbstractUndoCommand::undo(void)
{
    qDebug() << Q_FUNC_INFO << text();

    // in the case of InsertUndoCommand, we won't have a ViewObject left
    // when we get here ;-)
    if (theViewObjPtr)
        theViewObjPtr->setNewGeometry(theOrigPos, theOrigWidth, theOrigHeight);
    setDecoratorStateUndoRedo();
}
