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

#include "GameResources.h"
#include "resizinggraphicsview.h"
#include "SimulationControls.h"


ResizingGraphicsView::ResizingGraphicsView(QWidget *parent) :
	QGraphicsView(parent),
	theGameResourcesPtr(NULL),
	theGRAnimationPtr(NULL)

{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	theSimControlsPtr = new SimulationControls(this);
	hideSimControls();

}

ResizingGraphicsView::~ResizingGraphicsView()
{
	delete theSimControlsPtr;
	delete theGRAnimationPtr;
}


void ResizingGraphicsView::clearGameResourcesDialog()
{
	delete theGameResourcesPtr;
	theGameResourcesPtr = NULL;
}


void ResizingGraphicsView::createGameResourcesDialog()
{
	Q_ASSERT(theGameResourcesPtr == NULL);
	theGameResourcesPtr = new GameResources(this);
}


GameResources* ResizingGraphicsView::getGameResourcesDialogPtr() const
{
	Q_ASSERT(theGameResourcesPtr != NULL);
	return theGameResourcesPtr;
}


void ResizingGraphicsView::hideSimControls(void)
{
	theSimControlsPtr->hide();
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	theSimControlsPtr->parentResize(frameSize());

	// only resize GameResources if down
	if (theGRUpActionPtr->isEnabled())
		theGameResourcesPtr->parentResize(transform());
}


void ResizingGraphicsView::setup(QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr)
{
	theSimControlsPtr->setup(anMenuControlsPtr);

	// hook up the buttons for the GameResources dialog
	theGRDownActionPtr = aMenuBarPtr->addAction(tr("&Down"));
	theGRUpActionPtr   = aMenuBarPtr->addAction(tr("&Up"));

	// and set the QActions disabled for now
	theGRDownActionPtr->setEnabled(false);
	theGRUpActionPtr->setEnabled(false);
}

void ResizingGraphicsView::enableGameResourcesDialog(void)
{
	theGameResourcesPtr->updateToolbox();
	theGRDownActionPtr->setEnabled(false);
	theGRUpActionPtr->setEnabled(true);

	connect(theGRDownActionPtr, SIGNAL(triggered()), this, SLOT(showGRDialog()));
	connect(theGRUpActionPtr, SIGNAL(triggered()), this, SLOT(hideGRDialog()));
	connect(theGameResourcesPtr, SIGNAL(hideMe()), this, SLOT(hideGRDialog()));
}


void ResizingGraphicsView::showGRDialog()
{
	delete theGRAnimationPtr;
	theGameResourcesPtr->parentResize(transform());
	theGRAnimationPtr = new QPropertyAnimation(theGameResourcesPtr, "geometry");
	theGRAnimationPtr->setStartValue(QRectF(
									  QPointF((width()-theGameResourcesPtr->width())/2,
											  -theGameResourcesPtr->height()),
									  theGameResourcesPtr->size()));
	theGRAnimationPtr->setEndValue(QRectF(
									QPointF((width()-theGameResourcesPtr->width())/2,
											(height()-theGameResourcesPtr->height())/2),
									theGameResourcesPtr->size()));
	theGRAnimationPtr->setDuration(1000);
	theGRAnimationPtr->setEasingCurve(QEasingCurve::OutBounce);
	theGRAnimationPtr->start();
	theGRDownActionPtr->setEnabled(false);
	theGRUpActionPtr->setEnabled(true);
}

void ResizingGraphicsView::hideGRDialog()
{
	delete theGRAnimationPtr;
	theGRAnimationPtr = new QPropertyAnimation(theGameResourcesPtr, "geometry");
	theGRAnimationPtr->setEndValue(QRectF(
									  QPointF((width()-theGameResourcesPtr->width())/2,
											  -theGameResourcesPtr->height()),
									  theGameResourcesPtr->size()));
	theGRAnimationPtr->setStartValue(QRectF(
									QPointF((width()-theGameResourcesPtr->width())/2,
											(height()-theGameResourcesPtr->height())/2),
									theGameResourcesPtr->size()));
	theGRAnimationPtr->setDuration(1000);
	theGRAnimationPtr->setEasingCurve(QEasingCurve::OutQuad);
	theGRAnimationPtr->start();
	theGRDownActionPtr->setEnabled(true);
	theGRUpActionPtr->setEnabled(false);
}


void ResizingGraphicsView::showSimControls(void)
{
	theSimControlsPtr->show();
}
