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

#ifndef DRAWRAMP_H
#define DRAWRAMP_H

#include "ode/ode.h"
#include "DrawObject.h"
#include "Ramp.h"

// forward declarations
class BaseObject;


/** class DrawObject
  *
  * This class abstracts the actual drawing of objects
  * 
  */

class DrawRamp : public DrawObject
{
public:
	
	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	DrawRamp (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawRamp ( );

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

    /// overriden from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

	/// overriden from QGraphicsItem
    virtual void advance(int step);

protected:
    // Protected attribute accessor methods
	//  

private:
	virtual void initAttributes ( ) ;
};

#endif // DRAWRAMP_H
