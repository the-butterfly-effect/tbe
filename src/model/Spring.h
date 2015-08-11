/* The Butterfly Effect
 * This file copyright (C) 2010,2011  Klaas van Gend
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

#ifndef SPRING_H
#define SPRING_H

#include "RectObject.h"
#include "World.h"

#include <QStringList>

class SpringEnd;
typedef std::shared_ptr<SpringEnd> SpringEndPtr;

class ExplosionSplatter;
typedef std::shared_ptr<ExplosionSplatter> ExplosionSplatterPtr;


/** this class implements a Spring - a mechanical device that stores energy
 *
 *  The mechanics follow "Hooke's Law":
 *    the force with which the spring pushes back is linearly proportional to
 *    the distance from its equilibrium length.
 *    F = -k*x
 *
 *  Where "k" is measured in [N/m] or [kg/s/s] - it is adjustable through
 *  property "SpringConstant".
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

	/// overridden from AbstractObject to allow for the handle
	virtual void deletePhysicsObject(void);

	/// @returns Pointer to the B2Body for the relative position asked for.
	///          Might return NULL if no body or if outside body (see warning)
	/// @param   Relative position (to the center of the object) to look for
	/// @warning Because the default AbstractObject just returns its pointer
	///          without any checking, don't expect this member to fail if
	///          aRelPosition points outside the object's body.
	/// @note    depending on the position, will either return pointer to
	///          Spring's or to SpringEnd's b2Body
	virtual b2Body* getB2BodyPtrForPosition(UNUSED_ARG const Position& aRelPosition);

	/**
	 * Get the current Position of the object.
	 * This is the current center, i.e. where the object is now.
	 * For Spring, this is right inbetween the centers of both physobjects
	 *
	 * @return the value of theCenter
	 */
	virtual Position getTempCenter ( ) const;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

    /// overridden from AbstractObject in order to also move the SpringEnd
    virtual void setOrigCenter ( const Position& aNewPos ) override;

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
    AbstractObjectPtr theOtherEndPtr;

    /// the real value of the total spring object
    qreal theSpringWidth;

private:
	// disable copy constructor / assignment operator
	Spring(const Spring& aBORefToCopy);
	Spring& operator = (const Spring& aBORefToCopy);

	friend class SpringEnd;
};


/** specific class to handle the nocollision (towards Spring itself)
  * and no serialization
  * (because it is part of Spring and shouldn't exist by itself)
  */
class SpringEnd : public RectObject, public SimStepCallbackInterface
{
public:
    /// @param Spring pointer to a Spring, the only object allowed to create a SpringEnd
    SpringEnd(Spring* aDBox, const Position& aPos, qreal aWidth, qreal aHeight);
    virtual ~SpringEnd();

	/// overridden to allow setting a custom ZValue
	virtual ViewObject* createViewObject(float aDefaultDepth);

	/// overridden from RectObject to allow for the special joints
	/// and because this class wants to register for callbacks
	void createPhysicsObject();

	/// overridden from AbstractObject to allow for the special joints
	virtual void deletePhysicsObject(void);

	/// get the actual displacement of the prismatic joint
	/// where zero means no compression/extension
	qreal getDistance(void) const;

	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// updates the ViewObject to the position of the underlying b2body
	/// OVERRIDDEN from AbstractObject to not draw this object
	virtual void updateViewObject(bool) const
	{ ; }


	friend class Spring;

private:
    /// implemented from SimStepCallbackInterface
    virtual void callbackStep (qreal aTimeStep, qreal aTotalTime);

    Spring* theOtherEndPtr;
    b2PrismaticJoint* theJointPtr;

    /// 'k' from Hooke's law
    float theSpringConstant;

private:
	// disable copy constructor / assignment operator
	SpringEnd(const SpringEnd& aBORefToCopy);
	SpringEnd& operator = (const SpringEnd& aBORefToCopy);
};



#endif // SPRING_H
