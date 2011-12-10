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
	theMenuBarPtr(NULL)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	theSimControlsPtr = new SimulationControls(this);
	hideSimControls();

}

ResizingGraphicsView::~ResizingGraphicsView()
{
	delete theSimControlsPtr;
}


void ResizingGraphicsView::clearGameResourcesDialog()
{
	delete theGameResourcesPtr;
	theGameResourcesPtr = NULL;
}

void ResizingGraphicsView::createGameResourcesDialog()
{
	Q_ASSERT(theGameResourcesPtr == NULL);
	Q_ASSERT(theMenuBarPtr != NULL);
	theGameResourcesPtr = new GameResources(this);
	theGameResourcesPtr->setup(theMenuBarPtr);
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
	// TODO/FIXME: adjust position of GameResources dialog
}


void ResizingGraphicsView::setup(QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr)
{
	theSimControlsPtr->setup(anMenuControlsPtr);
	theMenuBarPtr = aMenuBarPtr;
}


void ResizingGraphicsView::showSimControls(void)
{
	theSimControlsPtr->show();
}
