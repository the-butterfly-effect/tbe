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

#include "resizinggraphicsview.h"
#include "Overlay.h"
#include "SimulationControls.h"

ResizingGraphicsView::ResizingGraphicsView(QWidget *parent) :
	QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setDragMode(QGraphicsView::NoDrag);
	overlay = new Overlay(this);
	hideSimControls();

}

ResizingGraphicsView::~ResizingGraphicsView()
{
	delete overlay;
}

void ResizingGraphicsView::hideSimControls(void)
{
	overlay->hide();
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	overlay->parentResize(frameSize());
}

void ResizingGraphicsView::setup(QMenu* anMBPtr)
{
	SimulationControls* myControls = new SimulationControls;
	myControls->setup(anMBPtr, overlay);
}


void ResizingGraphicsView::showSimControls(void)
{
	overlay->show();
}
