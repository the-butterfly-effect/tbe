/* The Butterfly Effect
 * This file copyright (C) 2011,2014,2015 Klaas van Gend
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
#include "Hint.h"
#include "InsertUndoCommand.h"
#include "UndoSingleton.h"
#include "ViewObject.h"
#include "World.h"


InsertUndoCommand::InsertUndoCommand(
        ViewObjectPtr anViewObjectPtr, QString anActionString)
    : AbstractUndoCommand(anViewObjectPtr, anActionString, nullptr),
      theTBGPtr(nullptr), theAOPtr(nullptr)
{
    DEBUG3ENTRY;
}


ViewObjectPtr InsertUndoCommand::createVOfromAO(AbstractObjectPtr anAOPtr)
{
    // put our shiny new object in the middle of the world
    Position myPos(World::getWorldPtr()->getTheWorldWidth()/2.0,
                   World::getWorldPtr()->getTheWorldWidth()/2.0, anAOPtr->getOrigCenter().angle);
    myPos = myPos - Vector(anAOPtr->getTheWidth()/2.0, anAOPtr->getTheHeight()/2.0);
    anAOPtr->setOrigCenter(myPos);

    // and create its ViewObject pointer
    ViewObjectPtr myVOPtr = anAOPtr->createViewObject();
    return myVOPtr;
}


bool InsertUndoCommand::createInsertUndoCommand(
        ToolboxGroup* anToolboxGroupPtr)
{
	// WARNING:  this is code duplication
	//           see createInsertUndoCommand(toolboxgroup + hint) above!
	DEBUG3ENTRY;
	// extract the AbstractObject from the toolbox
    AbstractObjectPtr myAOPtr = anToolboxGroupPtr->getObject();
	Q_ASSERT(myAOPtr!=nullptr);

    InsertUndoCommand* myInsertPtr = createInsertUndoCommandIntern(myAOPtr);
    myInsertPtr->theTBGPtr = anToolboxGroupPtr;
    myInsertPtr->commit();
	return true;
}

bool InsertUndoCommand::createInsertUndoCommand(ToolboxGroup *anToolboxGroupPtr, Hint *aHintPtr)
{
	// WARNING:  this is code duplication
	//           see createInsertUndoCommand(just a toolboxgroup) above!
	DEBUG3ENTRY;
	// extract the AbstractObject from the toolbox
	AbstractObjectPtr myAOPtr = anToolboxGroupPtr->getObject();
	Q_ASSERT(myAOPtr!=nullptr);

	InsertUndoCommand* myInsertPtr = createInsertUndoCommandIntern(myAOPtr);

	// Get position, size and rotation UPDATES from Hint
	// Note that the original object from the TBG already has some of this,
	// we only override when needed...
	aHintPtr->updateFromHint(myInsertPtr->theNewWidth,     Hint::WIDTH_STRING);
	aHintPtr->updateFromHint(myInsertPtr->theNewHeight,    Hint::HEIGHT_STRING);
	aHintPtr->updateFromHint(myInsertPtr->theNewPos.x,     Hint::XPOS_STRING);
	aHintPtr->updateFromHint(myInsertPtr->theNewPos.y,     Hint::YPOS_STRING);
	aHintPtr->updateFromHint(myInsertPtr->theNewPos.angle, Hint::ANGLE_STRING);

	// TODO later: get extra info from Hint (phone numbers, etc)

	myInsertPtr->theTBGPtr = anToolboxGroupPtr;
	myInsertPtr->commit();
	return true;
}

bool InsertUndoCommand::createInsertUndoCommand(AbstractObjectPtr anAOPtr)
{
	DEBUG3ENTRY
    InsertUndoCommand* myInsertPtr = createInsertUndoCommandIntern(anAOPtr);
    myInsertPtr->theTBGPtr = nullptr;
    myInsertPtr->theAOPtr  = anAOPtr;
    myInsertPtr->commit();
    return true;
}

InsertUndoCommand* InsertUndoCommand::createInsertUndoCommandIntern(
        AbstractObjectPtr anAOPtr)
{
    DEBUG3ENTRY;
    // extract the AbstractObject and the ViewObject
    ViewObjectPtr myVOPtr = createVOfromAO(anAOPtr);
    Q_ASSERT(myVOPtr!=nullptr);

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
        if (theViewObjPtr==nullptr)
            theViewObjPtr = createVOfromAO(theTBGPtr->getObject());
    }
    else
    {
        // In the case of insert in the LevelCreator, we kept a pointer to
        // the AbstractObject. That's why it didn't get deleted and the
        // ViewObject thus is still around, too... yay!
    }
    Q_ASSERT(theViewObjPtr!=nullptr);
    theViewObjPtr->setVisible(true);

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
