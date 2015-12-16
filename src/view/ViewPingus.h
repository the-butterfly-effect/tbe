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

#include "Pingus.h"
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
	explicit ViewPingus (AbstractObjectPtr aAbstractObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewPingus ( ) override;

	/// Sets the image to be displayed to the image pointed to by anIndex.
	/// @note Supposed to be only used by Pingus object
	/// @note New images are only loaded upon creation of the object.
	virtual void setNewImageIndex(unsigned int) override
	{ }

	/// Sets the state of the pingus (walking left/right, etc).
	/// Replaces setNewImageIndex that cannot work for Pingus.
	/// @param
	/// @param
	void setNewAnimationFrame(unsigned int aState, unsigned int aFrameIndex);

	/// overriden from ViewObject because we want to keep the pingus upright
	virtual void adjustObjectDrawing(qreal aWidth, qreal aHeight,
									 const Position& aCenter) override;

private:
	// prevent copy constructor / assignment operator
	ViewPingus(const ViewPingus&);
	const ViewPingus& operator= (const ViewPingus&);

	QPixmap theBigBitmap;

	int theIndexInImageList[Pingus::DEAD+1];
};

#endif // VIEWPINGUS_H
