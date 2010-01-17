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


StartStopWatch::StartStopWatch()
		: theResetSvgPtr(NULL)
{

	// I don't want the View to be different from the background.
	// FIXME/TODO: this is not entirely correct now, but we're close.
	setBackgroundBrush(QApplication::palette().window());

	// get our SVG renderers
	theStopwatchRendererPtr = ImageStore::getRenderer("StartStopWatch");
	theBrokenStopwatchRendererPtr = ImageStore::getRenderer("StartStopWatchBroken");
	theStopWatchSvgPtr = new QGraphicsSvgItem(NULL);
	theStopWatchSvgPtr->setSharedRenderer(theStopwatchRendererPtr);
	theStopWatchSvgPtr->setZValue(1.0);
	addItem(theStopWatchSvgPtr);

	QSvgRenderer* myRenderer = ImageStore::getRenderer("StartStopWatchHand");
	theStopWatchHandSvgPtr = new QGraphicsSvgItem(NULL);
	theStopWatchHandSvgPtr->setSharedRenderer(myRenderer);
	theStopWatchHandSvgPtr->setZValue(2.0);
	addItem(theStopWatchHandSvgPtr);

	// the stopwatch is 255x306 (pixels)
	// the Hand is 15x141 (pixels)
	// the center of the stopwatch is at 128 x (306-127=179)
	// the center of the Hand is at  7 x (141-48=93)
	// (all measured from the top-left corner)

	const qreal myHandRx = 7;
	const qreal myHandRy = 93;
	theStopWatchHandSvgPtr->setPos(128-myHandRx, 179-myHandRy);

	// let's move 2 degrees (1/3 second) per progress step
	theRotation = QTransform().translate(myHandRx, myHandRy).rotate(2).translate(-myHandRx, -myHandRy);

	theState = NOTSTARTED;
}

StartStopWatch::~StartStopWatch()
{
	;
}




void StartStopWatch::clicked_on_watch()
{
	DEBUG5("StartStopWatch::clicked_on_watch(void) whilst in state %d\n", theState);
	switch(theState)
	{
	case NOTSTARTED:
		goToState(RUNNING);
		break;
	case STOPPED:
		goToState(RUNNING);
		break;
	case RUNNING:
		goToState(STOPPED);
		break;
	case BROKEN:
		goToState(NOTSTARTED);
		break;
	}
}



void StartStopWatch::clicked_on_reset()
{
	// this should only be possible to call when the reset button is visible
	goToState(NOTSTARTED);
}


void StartStopWatch::goToState(TheStates aNewState)
{
	// let's implement the whole 3x3 matrix of possibilities
	// this should be the only member in the class that modifies theState!!!
	switch(theState)
	{
	case NOTSTARTED:
		{
			switch (aNewState)
			{
			case NOTSTARTED:	// no need for action
				break;
			case STOPPED:		// should not happen
				assert(false);
			case RUNNING:		// start button clicked
				startStopwatch();
				theState = aNewState;
				break;
			case BROKEN:		// should not happen
				assert(false);
				break;
			}
			break;
		}
	case STOPPED:
		{
			switch (aNewState)
			{
			case NOTSTARTED:	// reset button clicked
				removeResetButton();
				resetStopwatch();
				theState = aNewState;
				break;
			case STOPPED:		// no need for action
				break;
			case RUNNING:		// continue button clicked
				removeResetButton();
				startStopwatch();
				theState = aNewState;
				break;
			case BROKEN:		// should not happen
				assert(false);
				break;
			}
			break;
		}
	case RUNNING:
		{
			switch (aNewState)
			{
			case NOTSTARTED:
				// should not happen in normal use
				// so let's go to stopped first
				goToState(STOPPED);
				goToState(NOTSTARTED);
				break;
			case STOPPED:		// stop button clicked
				stopStopwatch();
				showResetButton();
				theState = aNewState;
				break;
			case RUNNING:		// no need for action
				break;
			case BROKEN:
				stopStopwatch();
				showResetButton();
				// show broken watch
				showWatch(true);
				theState = aNewState;
				break;
			}
			break;
		}
	case BROKEN:
		{
			switch (aNewState)
			{
			case NOTSTARTED:
				// no longer show broken watch
				showWatch(false);
				resetStopwatch();
				removeResetButton();
				theState = aNewState;
				break;
			case BROKEN:
				break;
			case STOPPED:
			case RUNNING:
				assert(false);
				break;
			}
			break;
		}
	} // end-of-switch
	DEBUG4("StartStopWatch: the new state is %d\n", theState);
}

void StartStopWatch::progressHand(void)
{
	// make the transform incremental: rotate some more
	theStopWatchHandSvgPtr->setTransform(theRotation, true);
}

void StartStopWatch::mousePressEvent (QGraphicsSceneMouseEvent * aMouseEvent )
{
	QGraphicsItem* myItemPtr = itemAt(aMouseEvent->scenePos());
	if (myItemPtr==theStopWatchSvgPtr || myItemPtr==theStopWatchHandSvgPtr)
		clicked_on_watch();
	if (myItemPtr==theResetSvgPtr)
		clicked_on_reset();
}

void StartStopWatch::removeResetButton(void)
{
	assert(theResetSvgPtr != NULL);
	delete theResetSvgPtr;
	theResetSvgPtr=NULL;
}

void StartStopWatch::resetStopwatch()
{
	// we need to reset the hand
	// i.e. we use the absolute transform - that will go back to (almost) start
	theStopWatchHandSvgPtr->setTransform(theRotation, false);
	emit resetSim();
}

void StartStopWatch::showResetButton(void)
{
	assert(theResetSvgPtr == NULL);

	// and initialise the reset button
	QSvgRenderer* myRenderer = ImageStore::getRenderer("ActionUndo");
	theResetSvgPtr = new QGraphicsSvgItem(NULL);
	theResetSvgPtr->setSharedRenderer(myRenderer);
	theResetSvgPtr->setZValue(5.0);
	addItem(theResetSvgPtr);
}

void StartStopWatch::showWatch(bool isBroken)
{
	if (isBroken)
		theStopWatchSvgPtr->setSharedRenderer(theBrokenStopwatchRendererPtr);
	else
		theStopWatchSvgPtr->setSharedRenderer(theStopwatchRendererPtr);
	// only display the hand when not broken
	theStopWatchHandSvgPtr->setVisible(isBroken==false);
}


void StartStopWatch::startStopwatch()
{
	// I want the hand to progress 10 times per second
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(progressHand()));
	theTimer.start(1000/10);

	emit startSim();
}

void StartStopWatch::stopStopwatch()
{
	theTimer.stop();
	emit stopSim();
}
