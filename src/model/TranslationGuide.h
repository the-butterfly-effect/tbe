/* The Butterfly Effect
 * This file copyright (C) 2010,2014  Klaas van Gend
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

#ifndef TRANSLATIONGUIDE_H
#define TRANSLATIONGUIDE_H

#include "AbstractJoint.h"
#include "TranslationGuidePtr.h"

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
class TranslationGuide : public AbstractJoint
{
public:
	/// empty constructor
	TranslationGuide(void);

	/** constructor to add translation joint for an object to world
	  * @param aAbstractObject
	  * @param aDirection	 the allowed direction of translation
	  */
	TranslationGuide(AbstractObjectPtr anAbstractObject, qreal aDirection);

	virtual ~TranslationGuide() {}

	/// overridden from AbstractObject
	/// (this class does not have a body, only a joint)
    void createPhysicsObject(void) override;

	/// overridden from AbstractObject
	/// returns the Name of the object.
    const QString getName ( ) const override
	{	return QObject::tr("TranslationGuide");	}

	/// overridden from AbstractObject
	/// returns true if the object can be rotated by the user
    bool isRotatable ( ) const override
	{	return false;	}

	/// overridden from AbstractObject
	/// parses all properties that TranslationGuide understands
    void  parseProperties(void) override;

	/// implemented from AbstractJoint
    void updateOrigCenter(void) override;

protected:

private:
	void initTG_Attributes ( );

	AbstractObjectPtr theObjectPtr;

	/** use property 'collide' to set this.
	  * true means that objects can collide
	  */
	bool areObjectsColliding;

	qreal	theDirection;
};

#endif // TRANSLATIONGUIDE_H
