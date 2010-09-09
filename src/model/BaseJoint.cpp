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

#include "BaseJoint.h"
#include "DrawObject.h"
#include "World.h"

#include <cassert>

/// pointer to World's groundbody.
static b2Body* theGroundBodyPtr = NULL;

BaseJoint::BaseJoint(void) : theJointPtr(NULL)
{
}



DrawObject*  BaseJoint::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	if (isPhysicsObjectCreated()==false)
		createPhysicsObject();
	QString myImageName = theProps.getPropertyNoDefault(Property::IMAGE_NAME_STRING);
	if (myImageName.isEmpty())
		return NULL;
	theDrawObjectPtr = new DrawObject(this, myImageName);
	setDrawObjectZValue(2.0);
	return theDrawObjectPtr;
}

void BaseJoint::deletePhysicsObject(void)
{
	if (theJointPtr)
		getB2WorldPtr()->DestroyJoint(theJointPtr);
	theJointPtr = NULL;
}


b2Body* BaseJoint::getGroundBodyPtr(void)
{
	return theGroundBodyPtr;
}


void BaseJoint::jointWasDeleted(void)
{
	// if this member is called, the joint is already gone
	DEBUG2("BaseJoint::jointWasDeleted(void) for %p\n", this);
	theJointPtr = NULL;
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
		createPhysicsObject();
		break;
	case JointInterface::DELETED:
		deletePhysicsObject();
		break;
	case JointInterface::POSUPDATE:
		deletePhysicsObject();
		updateOrigCenter();
		createPhysicsObject();
		break;
	}
}

void BaseJoint::setGroundBodyPtr(b2Body* aPtr)
{
	theGroundBodyPtr = aPtr;
}
