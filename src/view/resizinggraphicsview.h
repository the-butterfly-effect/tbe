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

// This headerfile is a bit different from other headerfiles
// because it uses only lowercase - that's to work around a
// Qt limitation.


#ifndef RESIZINGGRAPHICSVIEW_H_
#define RESIZINGGRAPHICSVIEW_H_

#include <QGraphicsView>
#include <QTimer>

/** a customized QGraphicsView that does not suffer from fitting problems
 */
class ResizingGraphicsView : public QGraphicsView
{
	Q_OBJECT
	
public:
	/** the constructor
	 *  it sets a lot of custom settings that we need for our GraphicsView
	 */
	ResizingGraphicsView (QWidget* aParent)
		: QGraphicsView(aParent)
	{
		setAlignment(Qt::AlignCenter);

		setInteractive(true);
		setDragMode(QGraphicsView::RubberBandDrag);
		setRubberBandSelectionMode(Qt::ContainsItemShape);

		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	    QSizePolicy vpol;
	    vpol.setHorizontalPolicy(QSizePolicy::Expanding);
	    vpol.setVerticalPolicy(QSizePolicy::Expanding);
		setSizePolicy(vpol);
	}

	virtual ~ResizingGraphicsView() {;}

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
	}
protected slots:
	void on_timerTick(void)
		{ if (scene()) 
			QGraphicsView::fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio); }
};

#endif /* RESIZINGGRAPHICSVIEW_H_ */
