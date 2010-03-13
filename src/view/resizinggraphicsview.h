/* The Butterfly Effect 
 * This file copyright (C) 2009-2010  Klaas van Gend
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

// This headerfile is a bit different from other headerfiles
// because it uses only lowercase - that's to work around a
// Qt limitation.


#ifndef RESIZINGGRAPHICSVIEW_H_
#define RESIZINGGRAPHICSVIEW_H_

#include "tbe_global.h"
#include "toolbox.h"
#include "DrawWorld.h"

#include <QGraphicsView>
#include <QTimer>
#include <QDragEnterEvent>

/** a customized QGraphicsView that does not suffer from fitting problems
 */
class ResizingGraphicsView : public QGraphicsView
{
	Q_OBJECT
	
public:
	/** the constructor
	 *  it sets a lot of custom settings that we need for our GraphicsView
	 */
	ResizingGraphicsView (QWidget* aParent);

	virtual ~ResizingGraphicsView() {;}

	/** This function returns the number of pixels that the view currently
	  * displays per unit (meter) in the scene. This number is used to
	  * calculate bitmap sizes for objects.
	  * STATIC FUNCTION, ACCESSIBLE WITHOUT POINTER.
	  * @returns a floating value usually higher than 50
	  */
	static float getPixelsPerSceneUnitHorizontal(void);

	/** this override works around a quirk in QGraphicsView,
	 *  where fitInView() works - but only after adding the scene is done and rendered
	 *  so we set a singleshot timer to redo the fitInView later...
	 */
	void setScene ( QGraphicsScene * scene )
	{
		QGraphicsView::setScene(scene); 		
		QTimer::singleShot(200, this, SLOT(on_timerTick()));
	}
	
protected:
	virtual void resizeEvent(QResizeEvent * event)
	{
		QGraphicsView::resizeEvent(event);
		on_timerTick();
		updatePixelsPerUnit();
	}

	void updatePixelsPerUnit();

	/// event handler override from QGraphicsView to accept drops
	/// and forward them to our graphics scene - which knows more...
	virtual void dropEvent (QDropEvent* event)
	{
		DEBUG4("void ResizingGraphicsView::dropEvent(\"%s\")\n", ASCII(event->mimeData()->formats().join(";")));
		QPointF myPos = mapToScene(event->pos());
		if (scene()!=NULL)
			reinterpret_cast<DrawWorld*>(scene())->dropEventFromView(myPos, event);
	}

	// D&D of objects from the toolbox is completely handled in DrawWorld
	//	virtual void dragEnterEvent(QDragEnterEvent *event);
	//	virtual void dragLeaveEvent(QDragLeaveEvent*);
	//	virtual void dragMoveEvent(QDragMoveEvent*);

	virtual void mouseMoveEvent(QMouseEvent* event);

protected slots:
	void on_timerTick(void);
};

#endif /* RESIZINGGRAPHICSVIEW_H_ */
