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

#ifndef DRAWBUTTERFLY_H
#define DRAWBUTTERFLY_H

#include "DrawObject.h"
#include "Butterfly.h"

// forward declarations
class BaseObject;


/** class DrawButterfly
  *
  * This class draws the Butterfly class, which has several layers
  * of drawing - the flapping of the wings
  */

class DrawButterfly : public QObject, public DrawObject
{
	
	Q_OBJECT

public:
	// Constructors/Destructors
	//  

	/**
	 * SVG Constructor
	 */
	DrawButterfly (BaseObject* aBaseObjectPtr, const QString& anImageName, ImageType anImageType = IMAGE_ANY);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawButterfly ( );

	/// overriden from DrawObject
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					   QWidget *widget);


private:
	virtual void initAttributes ( ) ;

};

#endif // DrawButterfly_H
