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

#include <QGraphicsItem>

// forward declarations
class BaseObject;
class QUndoStack;
class Anchors;
class QSvgRenderer;
class DrawWorld;

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

	/// Simple Constructor
	DrawObject (BaseObject* aBaseObjectPtr);

	/// Svg Constructor
	DrawObject (BaseObject* aBaseObjectPtr, const QString& anImageName);


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

	BaseObject* getBaseObjectPtr(void) const
		{ return theBaseObjectPtr; }

	/** removes itself from the DrawWorld
	 */
	bool deregister(void);

	/** adds itself again to the DrawWorld
	 *  Note: this function is not suitable for first registration!!!
	 */
	bool reregister();


protected:

	static QUndoStack* getUndoStackPtr(void);

	/** overridden from QGraphicsItem
	 *  if called, setup a context menu - a straight Qt Menu for now :-(
	 *
	 *  @param event the event to handle
	 */
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	/** overridden from QGraphicsItem
	 *  we want to know when the user selects the object
	 *    - so we can have theEditState take action
	 *  @param event the even to handle
	 */
	virtual void focusInEvent ( QFocusEvent * event );

	/** overridden from QGraphicsItem
	 *  we want to know when the user deselects the object
	 *    - so we can have theEditState take action
	 *  @param event the even to handle
	 */
	virtual void focusOutEvent ( QFocusEvent * event );

	/** overridden from QGraphicsItem
	 *  if called, setup a hover icon (indicating current action)
	 *  or not (if object is immovable)
	 *
	 *  @param event the event to handle
	 */
	virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

	/** overridden from QGraphicsItem
	 *  if called, remove the hover icon (indicating current action)
	 *
	 *  @param event the event to handle
	 */
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

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

	void applyPosition(void);
	
	virtual void paintHighlighted(QPainter* myPainter);

protected:
	// Private attributes
	//  

	BaseObject* theBaseObjectPtr;

	static const qreal theScale = 100.0;
	
	// current angle of the object in radians!
	qreal	theOldAngle;
	
	Anchors* theAnchorsPtr;
	QSvgRenderer*	theRenderer;

	bool isHighlighted;

	/// pointer for undeleting this object
	///   - only usable *after* a deregister() !!!
	DrawWorld* theUndeleteDrawWorldPtr;

private:
	virtual void initAttributes ( ) ;

	friend class PieMenu;
};

#endif // DRAWOBJECT_H
