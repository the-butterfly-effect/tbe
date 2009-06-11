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

#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "ode/ode.h"
#include <QGraphicsItem>

// forward declarations
class BaseObject;
class QUndoStack;

/** class DrawObject
  *
  * This class abstracts the actual drawing of objects
  * 
  */

class DrawObject : public QGraphicsItem
{
public:

	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	DrawObject (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawObject ( );

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

    /// overriden from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    static void setUndoStackPtr(QUndoStack* aPtr);

	/// overriden from QGraphicsItem
    virtual void advance(int step);

protected:
	static QUndoStack* getUndoStackPtr(void);
	
	/** overridden from QGraphicsItem
	 *  if the user drags the object around, this even will be called for each pixel.
	 *  let's actually adjust the coordinates!!!
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

	/** overridden from QGraphicsItem
	 *  we want to know when the user *releases* the left mouse button 
	 *    - so we can take action if it is a drag 
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	
protected:
    // Protected attribute accessor methods
	//  

	/**
	 * Set the value of theBodyID
	 * @param new_var the new value of theBodyID
	 */
	void setTheBodyID ( dBodyID new_var )
	{	theBodyID = new_var; }

	/**
	 * Get the value of theBodyID
	 * @return the value of theBodyID
	 */
	dBodyID getTheBodyID ( )
	{	return theBodyID; }

	void applyPosition(void);
	
protected:
	// Private attributes
	//  

	dBodyID theBodyID;
	BaseObject* theBaseObjectPtr;

	static const qreal theScale = 100.0;
	
	// current angle of the object in radians!
	qreal	theOldAngle;
	
private:
	virtual void initAttributes ( ) ;
};

#endif // DRAWOBJECT_H
