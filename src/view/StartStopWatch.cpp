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

#include "StartStopWatch.h"
#include "ImageStore.h"

#include <QGraphicsSvgItem>


StartStopWatch::StartStopWatch(Ui::MainWindow* aMainWindowUIPtr)
{
	// set scene to view
	QGraphicsView* myViewPtr = aMainWindowUIPtr->StartStopView;
	myViewPtr->setScene(this);
	myViewPtr->setAlignment(Qt::AlignCenter);
	myViewPtr->setInteractive(true);
	myViewPtr->setDragMode(QGraphicsView::NoDrag);

	myViewPtr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	myViewPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QSizePolicy myPol;
	myPol.setHorizontalPolicy(QSizePolicy::Expanding);
	myPol.setVerticalPolicy(QSizePolicy::Expanding);
	myViewPtr->setSizePolicy(myPol);


	// get our SVG renderers
	QSvgRenderer* myRenderer = ImageStore::getRenderer("StartStopWatch");
	theStopWatchSvgPtr = new QGraphicsSvgItem(NULL);
	theStopWatchSvgPtr->setSharedRenderer(myRenderer);
	theStopWatchSvgPtr->setZValue(1.0);
	addItem(theStopWatchSvgPtr);

	myRenderer = ImageStore::getRenderer("StartStopWatchHand");
	theStopWatchHandSvgPtr = new QGraphicsSvgItem(NULL);
	theStopWatchHandSvgPtr->setSharedRenderer(myRenderer);
	theStopWatchHandSvgPtr->setZValue(2.0);
	addItem(theStopWatchHandSvgPtr);

	myViewPtr->fitInView(itemsBoundingRect(), Qt::KeepAspectRatio);

	// the stopwatch is 290x290 (pixels)
	// the Hand is x (pixels)
	// the center of the stopwatch is at 163.4 x 127.7
	// the center of the Hand is at  65.7 x 36.0
	// (all measured from the top-left corner)

	const qreal myHandRx = 65.7;
	const qreal myHandRy = 36.0;
	const qreal myHandH  = 102.9;
	theStopWatchHandSvgPtr->setPos( 163.4-myHandRx, 127.7-myHandRy);

	// let's move 2 degrees (1/3 second) per progress step
	theRotation = QTransform().translate(myHandRx, myHandH-myHandRy).rotate(2).translate(-myHandRx, -myHandH+myHandRy);

	progressHand();

	// I want the hand to progress 10 times per second
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(progressHand()));
	theTimer.start(1000/10);

}

StartStopWatch::~StartStopWatch()
{
	;
}




void StartStopWatch::clicked_on_watch()
{
}



void StartStopWatch::clicked_on_reset()
{
}


void StartStopWatch::goToState(TheStates aNewState)
{
}


void StartStopWatch::progressHand(void)
{

	theStopWatchHandSvgPtr->setTransform(theRotation, true);
}

