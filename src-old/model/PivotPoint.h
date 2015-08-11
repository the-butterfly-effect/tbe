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

#ifndef PIVOTPOINT_H
#define PIVOTPOINT_H

#include "BaseJoint.h"



/// This class implements rotational joints - between objects or between one
/// object and "the world". 
/// Rotational joints can have an engine (implementing torque and/or 
/// rotational speed).
///
/// Example usage:
///    <object type="PivotPoint" X="3.2" Y="2.0">
///       <property key="object">Lever</property>
///    </object>
///
///    <object type="PivotPoint" X="3.2" Y="1.8">
///        <property key="object">SteamWheel</property>
///        <property key="Speed">1.0</property>
///        <property key="Torque">50.0</property>
///    </object>
///
///	   <object type="PivotPoint" X="3.95" Y="0.95">
///       <property key="object1">Bar</property>
///       <property key="object2">Handle</property>
///       <property key="collide">false</property>
///    </object>
/// See also http://sourceforge.net/apps/trac/tbe/wiki/PivotPoint
class PivotPoint : public BaseJoint
{
public:
	/// empty constructor
	PivotPoint(void);

	/** constructor to add pivot for an object to world
	  * @param aBaseObject
	  * @param aRelativePosition the (relative!) position of this pivot point
	  *                          relative to the center of the BaseObject
	  */
	PivotPoint(BaseObject* aBaseObject, const Vector& aRelativePosition);

	virtual ~PivotPoint();

	/// overridden from BaseObject
	/// (this class does not have a body, only a joint)
	virtual void createPhysicsObject(void);

	/// overridden from BaseObject
	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return QObject::tr("PivotPoint");	}

	/// overridden from BaseObject
	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Objects rotate around this point"); }

	/// overridden from BaseObject
	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// overridden from BaseObject
	/// parses all properties that PivotPoint understands
	virtual void  parseProperties(void);

	/// implemented from BaseJoint
	virtual void updateOrigCenter(void);

protected:

private:
	virtual void initAttributes ( );

	BaseObject* theFirstPtr;
	BaseObject* theSecondPtr;

	/** use property 'collide' to set this.
	  * true means that objects can collide - useful for true hinges
	  */
	bool areObjectsColliding;

	Vector thePosRelativeToFirst;
};

#endif // PIVOTPOINT_H
