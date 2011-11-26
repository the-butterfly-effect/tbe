/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "AbstractJoint.h"
#include "ViewObject.h"
#include "World.h"

#include <cassert>

/// pointer to World's groundbody.
static b2Body* theGroundBodyPtr = NULL;

AbstractJoint::AbstractJoint(void) : theJointPtr(NULL)
{
}

AbstractJoint::~AbstractJoint()
{
	DEBUG5("AbstractJoint::~AbstractJoint() for %p\n", this);
}

void AbstractJoint::deletePhysicsObject(void)
{
	// TODO/FIXME: there's logic behind this member that needs documenting
	// because otherwise we would have left it to the deletePhysicsObject()
	// of AbstractObject, right?
	DEBUG5("AbstractJoint::deletePhysicsObject(void)\n");
	theJointPtr = NULL;
	if (theViewObjectPtr)
		theViewObjectPtr->setVisible(true);
}

#if 0
b2Body* AbstractJoint::getB2BodyPtrFor(AbstractObject* anObject, const Position& aPosition)
{
	b2Body* myReturn = anObject->getB2BodyPtrForPosition(aPosition);
	if (myReturn == NULL)
	{
		anObject->createPhysicsObject();
		myReturn = anObject->getB2BodyPtrForPosition(aPosition);
	}
	return myReturn;
}


b2Body* AbstractJoint::getGroundBodyPtr(void)
{
	return theGroundBodyPtr;
}


void AbstractJoint::jointWasDeleted(void)
{
	// if this member is called, the joint is already gone
	DEBUG4("AbstractJoint::jointWasDeleted(void) for %p\n", this);
	theJointPtr = NULL;
	if (theViewObjectPtr)
		theViewObjectPtr->setVisible(false);
}

void AbstractJoint::markAsChild(void)
{
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

void AbstractJoint::physicsObjectStatus(JointInterface::JointStatus aStatus)
{
	switch (aStatus)
	{
	case JointInterface::CREATED:
		if (theViewObjectPtr)
			theViewObjectPtr->setVisible(true);
		break;
	case JointInterface::DELETED:
		jointWasDeleted();
		break;
	case JointInterface::POSUPDATE:
		updateOrigCenter();
		break;
	}
}

void AbstractJoint::setGroundBodyPtr(b2Body* aPtr)
{
	theGroundBodyPtr = aPtr;
}

#endif
