/* The Butterfly Effect
 * This file copyright (C) 2011,2014 Klaas van Gend
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

#include <QGraphicsScene>

#include "AbstractObject.h"
#include "InsertUndoCommand.h"
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


ViewObject* InsertUndoCommand::createVOfromAO(AbstractObjectPtr anAOPtr)
{
    // put our shiny new object in the middle of the world
    Position myPos(World::getWorldPtr()->getTheWorldWidth()/2.0,
                   World::getWorldPtr()->getTheWorldWidth()/2.0, anAOPtr->getOrigCenter().angle);
    myPos = myPos - Vector(anAOPtr->getTheWidth()/2.0, anAOPtr->getTheHeight()/2.0);
    anAOPtr->setOrigCenter(myPos);

    // and create its ViewObject pointer
    ViewObject* myVOPtr = anAOPtr->createViewObject();
    return myVOPtr;
}


bool InsertUndoCommand::createInsertUndoCommand(
        ToolboxGroup* anToolboxGroupPtr)
{
	DEBUG3ENTRY;
	// extract the AbstractObject from the toolbox
    AbstractObjectPtr myAOPtr = anToolboxGroupPtr->getObject();
	Q_ASSERT(myAOPtr!=NULL);

    InsertUndoCommand* myInsertPtr = createInsertUndoCommandIntern(myAOPtr);
    myInsertPtr->theTBGPtr = anToolboxGroupPtr;
    myInsertPtr->commit();
    return true;
}

bool InsertUndoCommand::createInsertUndoCommand(AbstractObjectPtr anAOPtr)
{
	DEBUG3ENTRY
    InsertUndoCommand* myInsertPtr = createInsertUndoCommandIntern(anAOPtr);
    myInsertPtr->theTBGPtr = NULL;
    myInsertPtr->commit();
    return true;
}

InsertUndoCommand* InsertUndoCommand::createInsertUndoCommandIntern(
        AbstractObjectPtr anAOPtr)
{
	DEBUG3ENTRY
    // extract the AbstractObject and the ViewObject
    ViewObject* myVOPtr = createVOfromAO(anAOPtr);
    Q_ASSERT(myVOPtr!=NULL);

    InsertUndoCommand* myInsertPtr = reinterpret_cast<InsertUndoCommand*>
                                      (UndoSingleton::createUndoCommand(
                                         myVOPtr,
                                         ActionIcon::ACTION_INSERT));
    myVOPtr->setVisible(true);
    return myInsertPtr;
}


void InsertUndoCommand::redo(void)
{
	DEBUG3("InsertUndoCommand::redo for '%s'", ASCII(text()));

    if (theTBGPtr)
    {
        // Insert based on toolbox
        if (theViewObjPtr==NULL)
            theViewObjPtr = createVOfromAO(theTBGPtr->getObject());
        else
        {
             if (theViewObjPtr->isVisible()==false)
                 theViewObjPtr = createVOfromAO(theTBGPtr->getObject());;
             theViewObjPtr->setVisible(true);
        }
    }
    else
    {
        // Insert based on Level Creator menu
        if (theViewObjPtr->isVisible()==false)
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
	DEBUG3("InsertUndoCommand::undo for '%s'", ASCII(text()));

    AbstractObjectPtr myAOPtr = theViewObjPtr->getAbstractObjectPtr();

    // remove from the world and viewworld
    bool myResult = World::getWorldPtr()->removeObject(myAOPtr);
    Q_ASSERT(myResult==true);
    /* and remove compiler warning: */ (void)myResult;

    theViewObjPtr->setVisible(false);

    if (theTBGPtr)
    {
        // return to the toolbox
        theTBGPtr->addObject(myAOPtr);
    }
    else
    {
        // not created from toolbox, nothing to do...
    }

    AbstractUndoCommand::undo();
}
