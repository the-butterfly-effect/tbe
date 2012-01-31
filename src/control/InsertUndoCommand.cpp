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

#include "AbstractObject.h"
#include "InsertUndoCommand.h"
#include <QGraphicsScene>
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "World.h"


InsertUndoCommand::InsertUndoCommand(
        ViewObject* anViewObjectPtr, QString anActionString)
    : AbstractUndoCommand(anViewObjectPtr, anActionString, NULL),
      theTBGPtr(NULL)
{
    DEBUG3ENTRY;
}


ViewObject* InsertUndoCommand::createAOandVO(ToolboxGroup* anToolboxGroupPtr)
{
    // get an object to work with
    AbstractObject* myAOPtr = anToolboxGroupPtr->getObject();

    // put it in the middle of the world
    Position myPos(World::getWorldPtr()->getTheWorldWidth()/2.0,
                   World::getWorldPtr()->getTheWorldWidth()/2.0, myAOPtr->getOrigCenter().angle);
    myPos = myPos - Vector(myAOPtr->getTheWidth()/2.0, myAOPtr->getTheHeight()/2.0);
    myAOPtr->setOrigCenter(myPos);

    // and create its ViewObject pointer
    ViewObject* myVOPtr = myAOPtr->createViewObject();
    return myVOPtr;
}


bool InsertUndoCommand::createInsertUndoCommand(
        ToolboxGroup* anToolboxGroup)
{
    qDebug() << Q_FUNC_INFO;
    // extract the AbstractObject and the ViewObject
    ViewObject* myVOPtr = createAOandVO(anToolboxGroup);
    Q_ASSERT(myVOPtr!=NULL);

    InsertUndoCommand* myInsertPtr = reinterpret_cast<InsertUndoCommand*>
                                      (UndoSingleton::createUndoCommand(
                                         myVOPtr,
                                         ActionIcon::ACTION_INSERT));
    myVOPtr->setVisible(true);
    myInsertPtr->theTBGPtr = anToolboxGroup;
    myInsertPtr->commit();
    return true;
}


void InsertUndoCommand::redo(void)
{
    qDebug() << Q_FUNC_INFO << text();

    if (theViewObjPtr==NULL)
        theViewObjPtr = createAOandVO(theTBGPtr);
    else
    {
         if (theViewObjPtr->isVisible()==false)
             theViewObjPtr = createAOandVO(theTBGPtr);
         theViewObjPtr->setVisible(true);
    }
    Q_ASSERT(theViewObjPtr!=NULL);

    // the ViewObject already exists when we get here,
    // but World should always check if it needs to be added...
    World::getWorldPtr()->addObject(theViewObjPtr->getAbstractObjectPtr());

    // our parent also has to do some things...
    AbstractUndoCommand::redo();
}


void InsertUndoCommand::undo(void)
{
    qDebug() << Q_FUNC_INFO << text();

    AbstractObject* myAOPtr = theViewObjPtr->getAbstractObjectPtr();

    // remove from the world and viewworld
    bool myResult = World::getWorldPtr()->removeObject(myAOPtr);
    Q_ASSERT(myResult==true);
    theViewObjPtr->setVisible(false);

    // return to the toolbox
    theTBGPtr->addObject(myAOPtr);

    AbstractUndoCommand::undo();
}
