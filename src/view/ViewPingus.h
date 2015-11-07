/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
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

#ifndef VIEWPINGUS_H
#define VIEWPINGUS_H

#include "ViewObject.h"

// forward declarations
class AbstractObject;

/** class ViewPingus
  *
  * This class draws the Pingus object, an object with a huge bitmap with
  * various animation frames
  */

class ViewPingus : public ViewObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/// Image Constructor
	ViewPingus (AbstractObjectPtr aAbstractObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewPingus ( ) override;

	/// Sets the image to be displayed to the image pointed to by anIndex.
	/// @note Supposed to be only used by Pingus object
	/// @note New images are only loaded upon creation of the object.
	virtual void setNewImageIndex(unsigned int anIndex) override;

	/// Sets the state of the pingus (walking left/right, etc).
	/// Obviously works together with setNewImageIndex to find the exact
	/// frame to display.
	void setNewAnimationState(unsigned int anIndex);

private:
	// prevent copy constructor / assignment operator
	ViewPingus(const ViewPingus&);
	const ViewPingus& operator= (const ViewPingus&);
};

#endif // VIEWPINGUS_H
