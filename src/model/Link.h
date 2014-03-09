/* The Butterfly Effect
 * This file copyright (C) 2010,2012  Klaas van Gend
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

#ifndef LINK_H
#define LINK_H

#include "AbstractObject.h"
#include "AbstractJoint.h"
#include "Position.h"

/// The Link class is a joint, it links two objects (bodies) together
/// whilst keeping the connection points at a constant distance
///
/// <!-- the x/y/angle in the object tag is ignored and/or recalculated when needed -->
/// <object type="Link" X="2.74" Y="3.44" angle="2.12">
///    <property key="object1">Skyhook@(1,2)</property>
///    <property key="object2">Chain3@(-1,2)</property>
///    <property key="ImageName">flat-chain</property>
/// </object>

class Link : public AbstractJoint
{
public:
	Link(void);

	virtual ~Link();

	/** (overridden from AbstractJoint to fixup aspect ratio and overlap)
	  * @returns pointer to AbstractObject
	  */
	virtual ViewObject* createViewObject(float aDefaultDepth = 2.0);

	/// overridden from AbstractObject
	/// (this class does not have a body, only a joint)
	virtual void createPhysicsObject(void);

	/// overridden from AbstractObject
	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return QObject::tr("Link");	}

	/** Get the current center position of the object.
	 * @return the value of theCenter
	 */
	virtual Position getTempCenter ( ) const;

	/// overridden from AbstractObject
	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("A Link is a massless, bodyless connection between two objects."); }

	/// overridden from AbstractObject
	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from AbstractObject
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// overridden from AbstractObject
	/// parses all properties that Link understands
	virtual void  parseProperties(void);

	/// implemented from BaseJoint
	virtual void updateOrigCenter(void);

        /// updates the ViewLink to still have its line between the underlying b2bodys
        /// (it won't update if the object is asleep if sim is running)
        /// @param isSimRunning  set to true if you want to use position/size from sim
        virtual void updateViewObject(bool isSimRunning) const;

private:
	AbstractObjectPtr theFirstPtr;
	AbstractObjectPtr theSecondPtr;
	Vector*     theFirstLocalPosPtr;
        Vector*     theSecondLocalPosPtr;

};

#endif // LINK_H
