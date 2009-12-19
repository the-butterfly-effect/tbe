/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

	// the following strings are used within e.g. RectObject as part
	// of the above RESIZABLE_STRING:

	static const char* NONE_STRING;
	static const char* HORIZONTAL_STRING;
	static const char* VERTICAL_STRING;
	static const char* TOTALRESIZE_STRING;
};

#endif // PROPERTY_H
