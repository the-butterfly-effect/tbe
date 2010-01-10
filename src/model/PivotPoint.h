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

#ifndef PIVOTPOINT_H
#define PIVOTPOINT_H

#include "BaseObject.h"

// forward declarations
class b2BodyPtr;
class b2RevoluteJoint;


/// class to implement rotational joints
class PivotPoint : public BaseObject
{
public:
	PivotPoint();

	virtual ~PivotPoint() {};

	virtual DrawObject* createDrawObject();

	/// overridden from BaseObject
	/// (this class does not have a body, only a joint)
	virtual void createPhysicsObject(void);

	/// overridden from BaseObject
	/// (this class does not have a body, only a joint)
	virtual void deletePhysicsObject(void);

	/// overridden from BaseObject
	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return QObject::tr("PivotPoint");	}

	/**
	 * Get the current Position of the object.
	 * This is the current center, i.e. where the object is now.
	 *
	 * @return the value of theCenter
	 * overridden from BaseObject
	 */
	virtual Position getTempCenter ( ) const;

	/// overridden from BaseObject
	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Objects rotate around this point"); }

	/// overridden from BaseObject
	/// @returns true if the Joint is created
	virtual bool isPhysicsObjectCreated(void) const
	{ return theJointPtr!=NULL; }

	/// overridden from BaseObject
	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/** "resets the object into the start position/situation"
	  * in this case, the *first* reset will find the object(s) and setup the joint
	  * any subsequent calls don't need help.
	  * (overridden from BaseObject)
	  */
	virtual void reset(void);

	/// called by the World class to set the Ground Body
	static void setGroundBodyPtr(b2Body* aPtr);

	/// called by World when the joint was "implicitly destructed"
	void jointWasDeleted(void)
	{	theJointPtr = NULL;	}

protected:
	/** returns true if property aPropertyName exists *and*
	  * its value is the ID of an existing BaseObject instance
	  * @param aWorldPtr
	  * @param aPropertyName
	  * @param aBOPtrPtr	  OUTPUT upon success contains pointer to BaseObject
	  * @returns true if success. if no success, value of aBOPtrPtr is undefined
	  */
	bool propertyToObjectPtr(World* aWorldPtr,
							 const QString& aPropertyName,
							 BaseObject** aBOPtrPtr);

private:
	BaseObject* theFirstPtr;
	BaseObject* theSecondPtr;

	b2RevoluteJoint* theJointPtr;
};

#endif // PIVOTPOINT_H
