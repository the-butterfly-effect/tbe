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

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <qstring.h>
#include "ode/ode.h"
#include "tbe_global.h"

#include "Position.h"

// FIXME: hack hack hack
// defined and initialized in main.cpp
extern dWorldID theGlobalWorldID;
extern dSpaceID theGlobalSpaceID;

/**
  * class BaseObject
  * 
  */

class BaseObject
{
public:

				// Constructors/Destructors
				//  


				/**
				 * Empty Constructor
				 */
				BaseObject ( );

				/**
				 * Empty Destructor
				 */
				virtual ~BaseObject ( );

				// Public attribute accessor methods
				//  

				/// returns the Name of the object.
				virtual const QString getName ( ) const = 0;

				/// returns the Tooltip of the object.
				virtual const QString getToolTip ( ) const = 0;

				/// returns true if the object can be rotated by the user
				virtual bool isRotatable ( ) const = 0;

				/// returns true if the object can be resized by the user
				virtual bool isResizable ( ) const = 0;

				
protected:

				// Static Protected attributes
				//  

				// Protected attributes
				//  

public:


				// Protected attribute accessor methods
				//  

protected:

public:


				// Protected attribute accessor methods
				//  

protected:


private:
	// Private attributes
	
	Position theCenter;
	dReal theWidth;
	dReal theHeight;
	dReal theScale;

protected:
	dBodyID theBodyID;
	dGeomID theGeomID;

public:
				// Private attribute accessor methods

				/**
				 * Set the value of theCenter
				 * @param new_var the new value of theCenter
				 */
				void setTheCenter ( Position new_var )				 {
												theCenter = new_var;
				}

				/**
				 * Get the value of theCenter
				 * @return the value of theCenter
				 */
				Position getTheCenter ( )				 {
								return theCenter;
				}

				/**
				 * Set the value of theWidth
				 * @param new_var the new value of theWidth
				 */
				void setTheWidth ( dReal new_var )				 {
												theWidth = new_var;
				}

				/**
				 * Get the value of theWidth
				 * @return the value of theWidth
				 */
				dReal getTheWidth ( )				 {
								return theWidth;
				}

				/**
				 * Set the value of theHeight
				 * @param new_var the new value of theHeight
				 */
				void setTheHeight ( dReal new_var )				 {
												theHeight = new_var;
				}

				/**
				 * Get the value of theHeight
				 * @return the value of theHeight
				 */
				dReal getTheHeight ( )				 {
								return theHeight;
				}

				/**
				 * Set the value of theBodyID
				 * @param new_var the new value of theBodyID
				 */
				void setTheBodyID ( dBodyID new_var )				 {
												theBodyID = new_var;
				}

				/**
				 * Get the value of theBodyID
				 * @return the value of theBodyID
				 */
				dBodyID getTheBodyID ( )				 {
								return theBodyID;
				}

				/**
				 * Set the value of theGeomID
				 * @param new_var the new value of theGeomID
				 */
				void setTheGeomID ( dGeomID new_var )				 {
												theGeomID = new_var;
				}

				/**
				 * Get the value of theGeomID
				 * @return the value of theGeomID
				 */
				dGeomID getTheGeomID ( )				 {
								return theGeomID;
				}

				/**
				 * Set the value of theScale
				 * @param new_var the new value of theScale
				 */
				void setTheScale ( dReal new_var )				 {
												theScale = new_var;
				}

				/**
				 * Get the value of theScale
				 * @return the value of theScale
				 */
				dReal getTheScale ( )				 {
								return theScale;
				}
private:


				void initAttributes ( ) ;

};

#endif // BASEOBJECT_H
