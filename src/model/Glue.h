/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef GLUE_H
#define GLUE_H

#include "BaseObject.h"
#include "BaseJoint.h"
#include "Position.h"

// forward declarations
class b2DistanceJoint;


/// The Glue class is a joint, it links two objects (bodies) together.
/// Due to using a distant joint and a revolute joint, both objects are really
/// locked together - unmovably.
///
/// <!-- the x/y/angle in the object tag are ignored and recalculated,
///      the width/height are used to display the image and distance the two joints-->
/// <object type="Glue" X="2.74" Y="3.44" width="0.1" height="0.1" angle="2.12">
///    <property key="object1">Bar@(-0.75,0)</property>
///    <property key="object2">Stop</property>
///    <property key="ImageName">GlueBlob</property>
/// </object>
class Glue : public BaseJoint
{
public:
	Glue();

	virtual ~Glue() {}

	/** (overridden from BaseJoint to fixup aspect ratio and overlap)
	  * @returns pointer to DrawObject
	  */
//	virtual DrawObject* createDrawObject();

	/// overridden from BaseObject
	/// (this class does not have a body, only two joints)
	virtual void createPhysicsObject(void);

	/// overridden from BaseObject
	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return QObject::tr("Glue");	}

	/** Get the current center position of the object.
	 * @return the value of theCenter
	 */
	virtual Position getTempCenter ( ) const;

	/// overridden from BaseObject
	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Glue links two objects immovably together."); }

	/// overridden from BaseObject
	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from BaseObject
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// overridden from BaseObject
	/// parses all properties that Glue understands
	/// - only "object1" and "object2"
	virtual void  parseProperties(void);

	/// implemented from BaseJoint
	virtual void updateOrigCenter(void);

private:
	BaseObject* theFirstPtr;
	BaseObject* theSecondPtr;
	Vector*     theFirstLocalPosPtr;
	Vector*     theSecondLocalPosPtr;

	b2DistanceJoint* theLinkPtr;
};

#endif // GLUE_H
