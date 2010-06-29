/* The Butterfly Effect
 * This file copyright (C) 2009, 2010  Klaas van Gend
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

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QString>
#include <QMap>

// forward declarations
class Vector;
class World;
class BaseObject;

class PropertyList
{
public:
	/// empty virtual destructor
	virtual ~PropertyList() {;}

	/** set (or add) default properties that the object this property list
	  * belongs to, understands.
	  *
	  * format: aa:bb/cc:dd/-ee:/ff:/gg:hh
	  *   * different key:value pairs are separated by slashes
	  *   * keys and values are separated by colons
	  *   * aa, bb, ee, ff and gg are property keys (keys always start A-Z
	  *     or a-z) and bb, dd and hh are values
	  *   * if there is no value (like ff), that implies the default value is
	  *     empty
	  *   * if the key is preceded by a minus (like ee), that implies this
	  *     default property does not exist for this object (even though the
	  *     parent may know)
	  *   * if key are entered twice, they will be filtered and the last
	  *     occcuring value is used
	  */
	void setDefaultPropertiesString(const QString& aSeparableString);

	/// set property aKey to aValue
	void setProperty(const QString& aKey, const QString& aValue)
		{ theProperties[aKey] = aValue; }

	/** @returns the value for a default property aKey
	  * if the default property doesn't exist, it returns a null string
	  * this is different from "" - which is an empty string
	  */
	QString getDefaultProperty(const QString& aKey) const
	{ if (theDefaultProperties.contains(aKey)==false) return QString();
	  else return theDefaultProperties.value(aKey); }

	/** @returns a QStringList with all known default properties for
	  * the object.
	  */
	QStringList getDefaultPropertyList(void) const;

	/** @returns the value for property aKey
	  *  - or an empty string if it does not exist
	  */
	QString getProperty(const QString& aKey) const
	{ return theProperties.value(aKey, ""); }

	///
	void removeProperty(const QString& aKey)
	{ theProperties.remove(aKey); }

	bool doesPropertyExists(const QString& aKey) const;

	typedef QMap<QString,QString> PropertyMap;

	PropertyMap::const_iterator constPropertyBegin(void) const
	{ return theProperties.constBegin(); }

	PropertyMap::const_iterator constPropertyEnd(void) const
	{ return theProperties.constEnd(); }

	/** returns true if property aPropertyName exists *and*
	  * its value can be parsed to fit a bool (true/false/yes/no)
	  * @param aPropertyName
	  * @param aBool	  OUTPUT upon success contains value of property
	  * @returns true if success. if no success aBool is unchanged
	  */
	bool propertyToBool(const QString& aPropertyName, bool* aBool) const;

	/** returns true if property aPropertyName exists *and*
	  * its value can be parsed to fit aFloat
	  * @param aPropertyName
	  * @param aFloat		  OUTPUT upon success contains value of property
	  * @returns true if success. if no success aFloat is unchanged
	  */
	bool propertyToFloat(const QString& aPropertyName, float* aFloat) const;

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

	/** returns true if property aPropertyName exists *and*
	  * its value can be parsed to fit a position (x,y)
	  * because there is no angle, we use the Vector class, not Position.
	  * @param aPropertyName
	  * @param aPosition		  OUTPUT upon success contains value of property
	  * @returns true if success. if no success aPosition is unchanged
	  */
	bool propertyToVector(const QString& aPropertyName, Vector* aPosition) const;

	/** returns true if property aPropertyName exists *and*
	  * its value can be parsed to fit a string - that's always the case if not empty :-)
	  * @param aPropertyName
	  * @param aString		  OUTPUT upon success contains the value string
	  * @returns true if success (i.e. there is a string). false if empty
	  */
	bool propertyToString(const QString& aPropertyName, QString* aString) const;

	/// @returns the number of properties in this class
	int  getPropertyCount(void) const
	{ return theProperties.count(); }

private:
	PropertyMap theProperties;

	PropertyMap theDefaultProperties;
};


class Property
{
public:

	// the following strings are property names, as used in
	// the level definition XML file:
	//           <object width="0.25" height="0.8" type="RectObject" >
	//                <property key="Friction" >0.3</property>
	//                <property key="Bounciness" >0.2</property>
	//                <property key="ImageName" >styrofoam</property>
	//                <property key="Mass" >1</property>
	//           </object>

	// all below are used in BaseObject
	static const char* IMAGE_NAME_STRING;		// don't specify the .svg or .png
	static const char* BOUNCINESS_STRING;		// 0.0 - inf, where 1.0=perfect bounce
	static const char* PIVOTPOINT_STRING;		// coordinate - where to put a pivot point to world
	static const char* ZVALUE_STRING;    		// depth in image, 0.1 = default for Scenery,
												// 2.0 is default for normal objects


	// all below are usable in RectObject
	// all below (except resizable string) are usable in PolyObject

	static const char* OBJECT_NAME_STRING;
	static const char* RESIZABLE_STRING;		// see below
	static const char* ROTATABLE_STRING;		// "true" or "false"
	static const char* MASS_STRING;	    		// 0.0 - inf, in [kg]
	static const char* FRICTION_STRING; 		// 0.0 - 1.0, where 0=frictionless and 1=no slip
	static const char* DESCRIPTION_STRING;		// tooltip  FIXME/TODO: i18n on this

	// this one is unique to PolyObject
	static const char* POLYGONS_STRING;			// set(s) of coordinates

	// this one is unique to CustomBall
	static const char* RADIUS_STRING;	    	// 0.0 - inf, in [m]

	// these ones are used by the various joints
	static const char* TORQUE_STRING;			// -inf - +inf in [Nm]
	static const char* SPEED_STRING;			// -inf - +inf in either [m/s] or [rad/s]
	static const char* FORCE_STRING;			// -inf - +inf in [N]

	// the following strings are used within e.g. RectObject as part
	// of the above RESIZABLE_STRING:

	static const char* NONE_STRING;
	static const char* HORIZONTAL_STRING;
	static const char* VERTICAL_STRING;
	static const char* TOTALRESIZE_STRING;

	// the following is only used by CokeMentosBottle
	//

	static const char* THRUST_STRING;


	// the following strings are used by the various Goal classes:
	//

	// GoalDistance
	static const char* S_LESSTHAN;
	static const char* S_MORETHAN;

	// GoalPositionChange
	static const char* S_XCHANGED;
	static const char* S_XBELOW;
	static const char* S_XOVER;
	static const char* S_YCHANGED;
	static const char* S_YBELOW;
	static const char* S_YOVER;
	static const char* S_ACHANGED;
	static const char* S_ANYTHING;

	// Goal, PivotPoint:
	static const char* OBJECT_STRING;
	static const char* OBJECT1_STRING;
	static const char* OBJECT2_STRING;
	static const char* COLLIDE_STRING;

};

#endif // PROPERTY_H
