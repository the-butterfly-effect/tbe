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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "BaseJoint.h"
#include "DrawObject.h"
#include "World.h"

#include <cassert>

/// pointer to World's groundbody.
static b2Body* theGroundBodyPtr = NULL;

BaseJoint::BaseJoint(void) : theJointPtr(NULL)
{
}

BaseJoint::~BaseJoint()
{
	DEBUG5("BaseJoint::~BaseJoint() for %p\n", this);
}


DrawObject*  BaseJoint::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	QString myImageName = theProps.getPropertyNoDefault(Property::IMAGE_NAME_STRING);
	if (myImageName.isEmpty())
		return NULL;
	theDrawObjectPtr = new DrawObject(this, myImageName);
	setDrawObjectZValue(5.0);
	return theDrawObjectPtr;
}

void BaseJoint::deletePhysicsObject(void)
{
	DEBUG5("BaseJoint::deletePhysicsObject(void)\n");
	theJointPtr = NULL;
	if (theDrawObjectPtr)
		theDrawObjectPtr->setVisible(true);
}


b2Body* BaseJoint::getB2BodyPtrFor(BaseObject* anObject, const Position& aPosition)
{
	b2Body* myReturn = anObject->getB2BodyPtrForPosition(aPosition);
	if (myReturn == NULL)
	{
		anObject->createPhysicsObject();
		myReturn = anObject->getB2BodyPtrForPosition(aPosition);
	}
	return myReturn;
}


b2Body* BaseJoint::getGroundBodyPtr(void)
{
	return theGroundBodyPtr;
}


void BaseJoint::jointWasDeleted(void)
{
	// if this member is called, the joint is already gone
	DEBUG4("BaseJoint::jointWasDeleted(void) for %p\n", this);
	theJointPtr = NULL;
	if (theDrawObjectPtr)
		theDrawObjectPtr->setVisible(false);
}

void BaseJoint::markAsChild(void)
{
	theProps.setProperty(Property::ISCHILD_STRING, "yes");
}

void BaseJoint::physicsObjectStatus(JointInterface::JointStatus aStatus)
{
	switch (aStatus)
	{
	case JointInterface::CREATED:
		if (theDrawObjectPtr)
			theDrawObjectPtr->setVisible(true);
		break;
	case JointInterface::DELETED:
		jointWasDeleted();
		break;
	case JointInterface::POSUPDATE:
		updateOrigCenter();
		break;
	}
}

void BaseJoint::setGroundBodyPtr(b2Body* aPtr)
{
	theGroundBodyPtr = aPtr;
}
