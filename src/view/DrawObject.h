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
#include <QCursor>

// forward declarations
class BaseObject;
class QUndoStack;
class QUndoCommand;
class UndoMoveCommand;
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

	enum ImageType
	{
		IMAGE_PNG,
		IMAGE_SVG,
		IMAGE_ANY
	};

	/// Svg/Png Constructor
	DrawObject (BaseObject* aBaseObjectPtr, const QString& anImageName, ImageType anImageType = IMAGE_ANY);


	/**
	 * Empty Destructor
	 */
	virtual ~DrawObject ( );

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

	/** Generate a bitmap (QPixmap) of size aWidth*aHeight
	  * and draw the object within the bitmap.
	  * Make sure its aspect ratio keeps correct.
	  * if aWidth or aHeight is zero, the bitmap will be scaled to fit the
	  * current view (i.e. the resizinggraphicsview).
	  * @param aWidth   width of the bitmap in screen pixels or 0
	  * @param aHeight  height of the bitmap in screen pixels or 0
	  * @returns QPixmap*, caller is responsible for destruction.
	  *
	  * TODO/FIXME: Future improvement: use QPixmapCache classes
	  */
	QPixmap* createBitmap(int aWidth=0, int aHeight=0);

	/// overriden from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    static void setUndoStackPtr(QUndoStack* aPtr);

	/// overriden from QGraphicsItem
    virtual void advance(int step);

	BaseObject* getBaseObjectPtr(void) const
		{ return theBaseObjectPtr; }

	bool pushUndo(QUndoCommand* anUndo);

	/** removes highlighting from this object and anchors - if needed
	 *  @param alsoDeleteAnchors use true if also to delete Anchors, supply
	 *			false if called from Anchors itself
	 */
	void focusRemove(bool alsoDeleteAnchors=true);

	/// setup caching for this object
	void setupCache(void);

	/** Set the Z-ordering (which object is drawn on top of which).
	  * the highest number is drawn on top.
	  *  - default for objects is 2
	  *  - default for Scenery is 0.1
	  * the ZValue is actually set in setupCache()
	  * @param aZValue the Z Value to set drawing to
	  * OVERRIDDEN FROM QGraphicsItem
	  */
	virtual void setZValue(float aZValue)
	{ theZValue = aZValue; }


protected:
	float theZValue;

	/** collision detection, sets isCollidingDuringDrag if true
	  * @returns true if collision detected
	  */
	bool checkForCollision(void);

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
	 *  @param event the even to handle
	 */
//	virtual void focusOutEvent ( QFocusEvent * event );

	/** overridden from QGraphicsItem
	 *  if the user drags the object around, this even will be called for each pixel.
	 *  let's actually adjust the coordinates!!!
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent ( const QPointF& aPos, const QPointF& bPos = QPointF(0,0) );

friend class UndoInsertCommand;
friend class UndoMoveCommand;

	/** overridden from QGraphicsItem
	 *  we want to know when the user *releases* the left mouse button 
	 *    - so we can take action if it is a drag 
	 *  
	 *  @param event the even to handle
	 */
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/** overridden from QGraphicsItem
	 *  if this item is selectable, we want to have a pointing hand cursor
	 */
	void hoverEnterEvent ( QGraphicsSceneHoverEvent *)
	{	setCursor(QCursor(Qt::PointingHandCursor));	}

	/** overridden from QGraphicsItem
	 *  if this item is selectable, we want to have a pointing hand cursor
	 */
	void hoverLeaveEvent ( QGraphicsSceneHoverEvent *)
	{	unsetCursor(); }

protected:
	// Protected members
	//

	virtual void applyPosition(void);

	/// removes the collision cross - if there is one
	void removeCollisionCross(void);
	
protected:
	// Protected attributes
	//  

	BaseObject* theBaseObjectPtr;

	static const qreal theScale = 100.0;
	
	// current angle of the object in radians!
	qreal	theOldAngle;
	
	/** Pointer to an Achors class
	 *  This pointer is shared across all DrawObjects,
	 *  as such you shouldn't assume it's yours
	 */
	static Anchors* theAnchorsPtr;

	QSvgRenderer*	theRenderer;
	QPixmap*		thePixmapPtr;

	/// pointer for undeleting this object
	///   - only usable *after* a deregister() !!!
	DrawWorld* theUndeleteDrawWorldPtr;

	/// pointer to QUndoCommand for move
	UndoMoveCommand* theUndoMovePtr;

private:
	virtual void initAttributes ( ) ;

	// all below friends are needed for Undoing actions
	friend class UndoResizeCommand;    // resize
	friend class UndoDeleteCommand;    // delete


	/** this class-in-class displays the Cross over the
	  * item in case of a collision
	  */
	class Cross : public QGraphicsItem
	{
	public:
		Cross(DrawObject* aParent);
		virtual ~Cross();

		/** overriden from QGraphicsItem
		  * we return a boundingRect far away from any other object
		  * so there's definitely no collission with this one :-)
		  */
		virtual QRectF boundingRect() const
		{ return QRectF(-10.0,-10.0, 0.1, 0.1); }

		/// overriden from QGraphicsItem
		virtual void paint(QPainter *painter,
						   const QStyleOptionGraphicsItem *option,
						   QWidget *widget);
	private:
		/// we only need one Cross Renderer for all DrawObjects...
		static QSvgRenderer*	theCrossRendererPtr;

		BaseObject* theBaseObjectPtr;
	};

	Cross* theCrossPtr;
	friend class Cross;

private:
	// kill the default constructor
	DrawObject();
};

#endif // DRAWOBJECT_H
