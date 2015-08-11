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

// Strings identifying elements/nodes in the XML file
// (the ones without static are also used in BaseObjectSerializer.cpp)
//
static const char* theRootNodeString= "tbe-level";
static const char* theLevelInfoString = "levelinfo";
	static const char* theLevelAuthorString       = "author";
	static const char* theLevelDateString         = "date";
	static const char* theLevelDescriptionString  = "description";
	static const char* theLevelLicenseString      = "license";
	static const char* theLevelNameString         = "title";
static const char* theSceneString = "scene";
	static const char* theSceneSizeString  = "scenesize";
	static const char* theViewString       = "view";
	static const char* thePredefinedString = "predefined";
		   const char* theObjectString     = "object";
		   const char* thePropertyString   = "property";
static const char* theToolboxString = "toolbox";
static const char* theGoalsString = "goals";
		   const char* theGoalString = "goal";

const char* theWidthAttributeString     = "width";
const char* theHeightAttributeString    = "height";
const char* theXAttributeString         = "X";
const char* theYAttributeString         = "Y";
const char* theAngleAttributeString     = "angle";
const char* theTypeAttributeString      = "type";
const char* theIDAttributeString        = "ID";
