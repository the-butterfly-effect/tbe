/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H

#include <QtGui/QGraphicsPixmapItem>
#include "Position.h"

// forward declarations
class AbstractObject;
#include "ViewObjectActionDectorator.h"

/** class ViewObject
  *
  * This class abstracts the actual drawing of objects
  *
  */
class ViewObject : public QGraphicsPixmapItem
{
public:
	/// simple constructor
	explicit ViewObject(AbstractObject* anAbstractObjectPtr);

	/// image name constructor
	ViewObject(AbstractObject* anAbstractObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewObject ( );

	AbstractObject* getAbstractObjectPtr(void) const
		{ return theAbstractObjectPtr; }

	/// @returns Aspect ratio (width/height) of the (first) image,
	///          before it was scaled to width and height dimensions.
	qreal getOrigImageAspectRatio(void)
	{ return thePixmapWidth / thePixmapWidth; }

	/// Sets new geometry for this object.
	/// @note This member should only be called by UndoCommands
	/// @note It will set the geometry in the AbstractObject and
	/// then update itself.
	/// @param aNewPosition
	/// @param aNewWidth
	/// @param aNewHeight
	void setNewGeometry(const Position& aNewPosition,
						qreal aNewWidth,
						qreal aNewHeight);
	void setNewGeometry(const Position& aNewPosition);

    /// Sets the image to be displayed to the image pointed to by anIndex.
    /// @note Supposed to be only used by AbstractObject::updateViewObject().
    /// @note New images are only loaded upon creation of the object.
    void setNewImageIndex(unsigned int anIndex);

    /// based on changes in the underlying AbstractObject, adjust the image
    /// by resizing and/or rotation. This is overkill for just moving...
    void adjustObjectDrawing(qreal aWidth, qreal aHeight, const Position& aCenter);
protected:
    void adjustObjectDrawing(void);

    /// load an image (SVG/PNG/JPG) into a qpixmap and
    /// attach it to this object
    /// @param anImageName name of the image to load (without path or extension)
    /// @returns true if loading was successful
    /// @note if not successful, display a placeholder image
    bool loadImage(const QString& anImageName);

    /// overridden to allow detection of mouse button presses
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event);
    /// overridden to allow object highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent* event );
    /// overridden to allow highlighting
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent* event );

    void realHoverEnterEvent(void);
    void realMousePressEvent(QGraphicsSceneMouseEvent* anEvent);

protected:
	// Protected attributes
	//

	AbstractObject* theAbstractObjectPtr;

	typedef QList<QPixmap> ImageList;
	ImageList thePixmapList;

	friend class AbstractUndoCommand;
	ViewObjectActionDecorator theDecorator;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( ViewObject )

private:
	void initViewObjectAttributes(void);

	qreal thePixmapWidth;
	qreal thePixmapHeight;

	qreal theOldWidth;
	qreal theOldHeight;
	Position theOldPos;
};

#endif // VIEWOBJECT_H
