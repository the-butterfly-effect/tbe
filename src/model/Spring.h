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

#ifndef SPRING_H
#define SPRING_H

#include "AbstractBall.h"
#include "RectObject.h"
#include "PolyObject.h"
#include "World.h"

#include <QStringList>

class SpringHandle;
class ExplosionSplatter;


/** this class implements the Spring for an explosion
 *  it has a handle object at the top that can be pressed,
 *  once pressed deep enough, it will send a trigger.
 *
 *  Note that we cheat on reality big time here. The original boxes were
 *  noting more than a dynamo - producing enough current for a spark.
 *  In our demo, we just hooked those wires up to a mobile phone...
 *  Suuuuure that will work ;-)
 */
class Spring : public RectObject
{
public:
	Spring();
	virtual ~Spring();

	/// overridden from RectObject to be able to create the other SpringEnd
	/// and because this class wants to register for callbacks and
	/// needs to restart its state machine
	virtual void createPhysicsObject(void);

	/// overridden from BaseObject to allow for the handle
	virtual void deletePhysicsObject(void);

	/**
	 * Get the current Position of the object.
	 * This is the current center, i.e. where the object is now.
	 * For Spring, this is right inbetween the centers of both physobjects
	 *
	 * @return the value of theCenter
	 */
	virtual Position getTempCenter ( ) const;

	/// overridden from RectObject to make sure
	/// we can display the phone number
	virtual const QString getToolTip ( ) const;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject in order to also move the handle
	virtual void setOrigCenter ( Position new_var );

	/// get the actual width - with compression accounted for
	virtual qreal getTempWidth() const;

protected:
	/// this member fixes up the physical model based on new width or height
	/// overridden from RectObject, assuming that springs are never
	/// extremely tall or wide
	virtual void adjustParameters(void);

	void buildShapeList(void);

private:
	/// offset of center of the handle to the center of the box
	const static Vector HANDLEOFFSET;

	/// pointer to the handle (separate object)
	SpringHandle* theHandleObjectPtr;

	/// the real value of the total spring object
	qreal theSpringWidth;

private:
	// disable copy constructor / assignment operator
	Spring(const Spring& aBORefToCopy);
	Spring& operator = (const Spring& aBORefToCopy);

	friend class SpringHandle;
};


/** specific class to handle the nocollision (towards Spring itself)
  * and no serialization
  * (because it is part of Spring and shouldn't exist by itself)
  */
class SpringHandle : public RectObject, public SimStepCallbackInterface
{
private:
	/// @param aDBox pointer to a Spring, the only object allowed to create a Handle
	SpringHandle(Spring* aDBox, const Position& aPos, qreal aWidth, qreal aHeight);

public:
	virtual ~SpringHandle();

	/// overridden to allow setting a custom ZValue
	virtual DrawObject* createDrawObject();

	/// overridden from RectObject to allow for the special joints
	/// and because this class wants to register for callbacks
	void createPhysicsObject(void);

	/// overridden from BaseObject to allow for the special joints
	virtual void deletePhysicsObject(void);

	/// get the actual displacement of the prismatic joint
	/// where zero means no compression/extension
	qreal getDistance(void) const;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	friend class Spring;

private:
	/// implemented from SimStepCallbackInterface
	virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

	Spring* theDBoxPtr;
	b2PrismaticJoint* theJointPtr;

private:
	// disable copy constructor / assignment operator
	SpringHandle(const SpringHandle& aBORefToCopy);
	SpringHandle& operator = (const SpringHandle& aBORefToCopy);
};



#endif // SPRING_H
