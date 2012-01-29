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

#ifndef TRANSLATIONGUIDE_H
#define TRANSLATIONGUIDE_H

#include "AbstractJoint.h"


/// This class implements translational joints - for now only between one
/// object and "the world". 
/// Translational joints can have an engine (implementing force and/or
/// translation speed).
///
/// Example usages:
/// * Limits a rectobject to horizontal movement
///    <object type="RectObject" X="1" Y="1" width="1" height="1" angle="0">
///      <property key="TranslationGuide">0</property>
///    </object>
///
/// * Limits the "Elevator" to vertical movement, moves it upwards with a
///   target speed of 1 m/s; the force to make that happen is capped at 5 N
///    <object type="TranslationGuide" angle=1.57>
///        <property key="object">Elevator</property>
///        <property key="Speed">1.0</property>
///        <property key="Force">5.0</property>
///    </object>
/// See also http://sourceforge.net/apps/trac/tbe/wiki/TranslationGuide
class TranslationGuide : public AbstractJoint
{
public:
	/// empty constructor
	TranslationGuide(void);

	/** constructor to add translation joint for an object to world
	  * @param aAbstractObject
	  * @param aDirection	 the allowed direction of translation
	  */
	TranslationGuide(AbstractObject* anAbstractObject, qreal aDirection);

	virtual ~TranslationGuide() {}

	/// overridden from AbstractObject
	/// (this class does not have a body, only a joint)
	virtual void createPhysicsObject(void);

	/// overridden from AbstractObject
	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return QObject::tr("TranslationGuide");	}

	/// overridden from AbstractObject
	/// returns the Tooltip of the object.
	virtual const QString getToolTip ( ) const
	{	return QObject::tr("Objects are limited to only move along one axis"); }

	/// overridden from AbstractObject
	/// returns whether the object can be resized by the user
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}

	/// overridden from AbstractObject
	/// returns true if the object can be rotated by the user
	virtual bool isRotatable ( ) const
	{	return false;	}

	/// overridden from AbstractObject
	/// parses all properties that TranslationGuide understands
	virtual void  parseProperties(void);

	/// implemented from AbstractJoint
	virtual void updateOrigCenter(void);

protected:

private:
	void initTG_Attributes ( );

	AbstractObject* theObjectPtr;

	/** use property 'collide' to set this.
	  * true means that objects can collide
	  */
	bool areObjectsColliding;

	qreal	theDirection;
};

#endif // TRANSLATIONGUIDE_H
