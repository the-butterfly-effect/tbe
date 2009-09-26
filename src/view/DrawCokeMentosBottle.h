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

#ifndef DRAWCOKEMENTOSBOTTLE_H
#define DRAWCOKEMENTOSBOTTLE_H

#include "DrawObject.h"
#include "CokeMentosBottle.h"

// forward declarations
class BaseObject;


/** class DrawCokeMentosBottle
  *
  * This class draws the CokeMentosBottle class, which has several layers
  * of drawing, depending on whether the bottle is full, triggered or empty
  */

class DrawCokeMentosBottle : public QObject, public DrawObject
{
	
	Q_OBJECT

public:
	// Constructors/Destructors
	//  

	/**
	 * SVG Constructor
	 */
	DrawCokeMentosBottle (BaseObject* aBaseObjectPtr, const QString& anImageName, ImageType anImageType = IMAGE_ANY);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawCokeMentosBottle ( );

	/// overriden from DrawObject
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
					   QWidget *widget);


private:
	virtual void initAttributes ( ) ;

};

#endif // DRAWCOKEMENTOSBOTTLE_H
