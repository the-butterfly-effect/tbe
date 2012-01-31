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
	void setScene ( QGraphicsScene * aScene )
	{
		QGraphicsView::setScene(aScene);
		QTimer::singleShot(200, this, SLOT(on_timerTick()));
	}
	void updatePixelsPerUnit();

protected:
	virtual void resizeEvent(QResizeEvent * anEvent)
	{
		QGraphicsView::resizeEvent(anEvent);
		emit on_timerTick();
	}

	/** OVERRIDDEN from QGraphicsView
	  * remove the ability to use pageup/pagedown
	  */
	virtual void keyPressEvent(QKeyEvent* anEvent);

	/** OVERRIDDEN from QGraphicsView
	  * if mouse is near end of the view, start a drag&drop
	  */
	virtual void mouseMoveEvent(QMouseEvent* event);

	virtual void wheelEvent ( QWheelEvent * event );

	/** the actual creation of the mime data and corresponding UndoDelete
	  * @param aBOPtr pointer to the BaseObject to move to the toolbox
	  * @returns true if success or false if not (dnd didn't happen)
	  */
	bool handleDnD(BaseObject* aBOPtr);

public slots:
	void on_timerTick(void);
};

#endif /* RESIZINGGRAPHICSVIEW_H_ */
