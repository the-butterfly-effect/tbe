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


class PropertyList
{
public:
	/// set property aKey to aValue
	virtual void  setProperty(const QString& aKey, const QString& aValue)
		{ theProperties[aKey] = aValue; }

	/** @returns the value for property aKey
	  *  - or an empty string if it does not exist
	  */
	virtual QString getProperty(const QString& aKey) const
	{ return theProperties.value(aKey, ""); }

	///
	virtual void removeProperty(const QString& aKey)
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

	static const char* OBJECT_NAME_STRING;
	static const char* BOUNCINESS_STRING;		// 0.0 - inf, where 1.0=perfect bounce
	static const char* RESIZABLE_STRING;		// see below
	static const char* ROTATABLE_STRING;		// "true" or "false"
	static const char* MASS_STRING;				// 0.0 - inf, in [kg]
	static const char* FRICTION_STRING;			// 0.0 - 1.0, where 0=frictionless and 1=no slip
	static const char* IMAGE_NAME_STRING;		// don't specify the .svg or .png
	static const char* DESCRIPTION_STRING;		// tooltip  FIXME/TODO: i18n on this
	static const char* PIVOTPOINT_STRING;		// coordinate - where to put a pivot point to world

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

};

#endif // PROPERTY_H
