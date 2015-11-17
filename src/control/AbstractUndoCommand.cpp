/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#include "AbstractUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"

AbstractUndoCommand::AbstractUndoCommand(
        ViewObject* anViewObjectPtr,
        const QString& anUndoName,
        QUndoCommand* parent)
    : QUndoCommand(parent),
      theViewObjPtr(anViewObjectPtr),
      handleDecoratorOnDestructionBool(true)
{
    AbstractObjectPtr myObjectPtr = theViewObjPtr->getAbstractObjectPtr();

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
    if (handleDecoratorOnDestructionBool)
    {
        setDecoratorStateUndoRedo();
        clearDecoratorPointerToMe();
    }
    UndoSingleton::notifyGone(this);
}


void AbstractUndoCommand::clearDecoratorPointerToMe()
{
    theViewObjPtr->theDecorator.clearUndoPointer();
}


void AbstractUndoCommand::commit(void)
{
    DEBUG4ENTRY;
    UndoSingleton::push(this);
    resetDecoratorPosition();
    setDecoratorStateUndoRedo();
    clearDecoratorPointerToMe();
}


bool AbstractUndoCommand::isViewObjectColliding(void)
{
    // We need to check whether the ViewObject or any of the child items
    // (but not the ViewObjectAbstractDecorator) is colliding with any
    // other object (but not the ViewObject or the VOAD).
    int myCollisionCount = 0;

    QList<QGraphicsItem*> myCollidingItems = theViewObjPtr->collidingItems ( Qt::IntersectsItemShape );
    QList<QGraphicsItem*> myChildItems = theViewObjPtr->childItems();

    // Make the CollidingItems list complete for all childs
    foreach(QGraphicsItem* i, myChildItems)
    {
        if (i != &(theViewObjPtr->theDecorator))
            myCollidingItems += i->collidingItems(Qt::IntersectsItemShape );
    }
    // Add theViewObject to the list as well
    myChildItems += theViewObjPtr;

    // Finally... Check for collisions.
    foreach(QGraphicsItem* j, myCollidingItems)
    {
        if (!myChildItems.contains(j))
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
	DEBUG3("AbstractUndoCommand::redo for '%s'", ASCII(text()));

    // in the case of DeleteUndoCommand, we won't have a ViewObject left
    // when we get here ;-)
    if (theViewObjPtr)
    {
        theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
        setDecoratorStateUndoRedo();
    }
}


void AbstractUndoCommand::resetDecoratorPosition()
{
    Q_ASSERT(theViewObjPtr!=nullptr);
    theViewObjPtr->theDecorator.setPos( QPointF(0,0) );
}


void AbstractUndoCommand::setDecoratorImage(const QString& anImageName)
{
    Q_ASSERT(theViewObjPtr!=nullptr);
    theViewObjPtr->theDecorator.setDecoratorImage(anImageName, this);
    setDecoratorStateMouseMove();
}


void AbstractUndoCommand::setDecoratorPosition(Vector aScenePos)
{
    Q_ASSERT(theViewObjPtr!=nullptr);
    theViewObjPtr->theDecorator.setPos( theViewObjPtr->mapFromScene(aScenePos.toQPointF())
                                        - theViewObjPtr->boundingRect().center() );
}


bool AbstractUndoCommand::setDecoratorStateMouseMove(void)
{
    if (isViewObjectColliding() || theNewPos.y-0.5*theNewHeight<0)
    {
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::COMBINED);
        return true;
    }
    else
    {
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::PROXY);
        return false;
    }
}


bool AbstractUndoCommand::setDecoratorStateUndoRedo(void)
{
    if ((isViewObjectColliding() || theNewPos.y-0.5*theNewHeight<0) && theViewObjPtr->isVisible()==true)
    {
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::CROSS);
        return true;
    }
    else
    {
        theViewObjPtr->theDecorator.setCrossState(ViewObjectActionDecorator::NONE);
        return false;
    }
}


void AbstractUndoCommand::undo(void)
{
	DEBUG3("AbstractUndoCommand::undo for '%s'", ASCII(text()));

    // in the case of InsertUndoCommand, we won't have a ViewObject left
    // when we get here ;-)
    if (theViewObjPtr)
    {
        theViewObjPtr->setNewGeometry(theOrigPos, theOrigWidth, theOrigHeight);
        setDecoratorStateUndoRedo();
    }
}
