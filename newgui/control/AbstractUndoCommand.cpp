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
      theViewObjPtr(anViewObjectPtr),
      theVOADPtr(NULL)
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


void AbstractUndoCommand::deleteProxyImage(void)
{
    Q_ASSERT(theViewObjPtr!=NULL);
    Q_ASSERT(theVOADPtr!=NULL);
    delete theVOADPtr;
    theVOADPtr = NULL;
}


bool AbstractUndoCommand::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    DEBUG4ENTRY;

    // It's time to finalize everything
    // and push the Undo on the stack
    UndoSingleton::push(this);
    deleteProxyImage();

    // we've completely handled the event, we're done
    return true;
}


void AbstractUndoCommand::redo(void)
{
    qDebug() << Q_FUNC_INFO << text();
    theViewObjPtr->setNewGeometry(theNewPos, theNewWidth, theNewHeight);
}


void AbstractUndoCommand::setupProxyImage(const QString& anImageName)
{
    Q_ASSERT(anImageName.isEmpty()==false);
    Q_ASSERT(theViewObjPtr!=NULL);
    Q_ASSERT(theVOADPtr==NULL);
    theVOADPtr = new ViewObjectActionDecorator(theViewObjPtr, anImageName, this);
}


void AbstractUndoCommand::undo(void)
{
    qDebug() << Q_FUNC_INFO << text();
    theViewObjPtr->setNewGeometry(theOrigPos, theOrigWidth, theOrigHeight);

}
