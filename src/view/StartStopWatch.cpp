/* The Butterfly Effect
 * This file copyright (C) 2009, 2010  Klaas van Gend
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
		: theResetSvgPtr(NULL), theFastForwardSvgPtr(NULL)
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
	theFastRotation = QTransform().translate(myHandRx, myHandRy).rotate(4*2).translate(-myHandRx, -myHandRy);

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
	case FAST:
		goToState(RUNNING);
		break;
	case BROKEN:
		goToState(NOTSTARTED);
		break;
	}
}



void StartStopWatch::clicked_on_reset()
{
	// this should only be possible to call when the reset button is visible
	// but it doesn't really matter - goToState will take care of that :-)
	goToState(NOTSTARTED);
}

void StartStopWatch::clicked_on_fastforward()
{
	// only possible to call when the fastforward button is visible
	goToState(FAST);
}

void StartStopWatch::displayTooltip(bool isToSet)
{
	if (views().count()>0)
	{
		if (isToSet)
			views()[0]->setToolTip(tr("Click on the stopwatch to start/stop the simulation."));
		else
			views()[0]->setToolTip(tr(""));
	}
}

void StartStopWatch::goToState(TheStates aNewState)
{
	DEBUG4("StartStopWatch::goToState from %d to %d\n", theState, aNewState);
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
				DEBUG2("state change ignored\n");
				break;
			case RUNNING:		// watch clicked
				startStopwatch();
				showFastForwardButton();
				theState = aNewState;
				break;
			case FAST:			// should not happen
			case BROKEN:		// should not happen
				DEBUG2("state change ignored\n");
				break;
			}
			break;
		}
	case STOPPED:
		{
			switch (aNewState)
			{
			case NOTSTARTED:	// reset button clicked
				theState = aNewState;
				removeResetButton();
				resetStopwatch();
				break;
			case STOPPED:		// no need for action
				break;
			case RUNNING:		// watch clicked again
				removeResetButton();
				showFastForwardButton();
				startStopwatch();
				theState = aNewState;
				break;
			case BROKEN:		// should not happen
			case FAST:			// should not happen
				DEBUG2("state change ignored\n");
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
				removeFastForwardButton();
				theState = aNewState;
				break;
			case RUNNING:		// no need for action
				break;
			case FAST:			// fast-forward button clicked
				removeFastForwardButton();
				emit goFast();
				theState = aNewState;
				break;
			case BROKEN:
				stopStopwatch();
				showResetButton();
				removeFastForwardButton();
				// show broken watch
				showWatch(true);
				theState = aNewState;
				break;
			}
			break;
		}
	case FAST:
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
				emit goSlow();
				theState = aNewState;
				break;
			case RUNNING:		// slow down
				showFastForwardButton();
				emit goSlow();
				theState = aNewState;
				break;
			case FAST:			// nothing to do
				break;
			case BROKEN:
				stopStopwatch();
				showResetButton();
				emit goSlow();
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
				theState = aNewState;
				showWatch(false);
				stopStopwatch();
				resetStopwatch();
				removeResetButton();
				break;
			case BROKEN:
				break;
			case STOPPED:
			case RUNNING:
			case FAST:
				DEBUG2("state change ignored\n");
				break;
			}
			break;
		}
	} // end-of-switch
	DEBUG4("StartStopWatch: the new state is %d\n", theState);
}

void StartStopWatch::keypress_go()
{
	// let's emulate mouse clicks
	if(theState==NOTSTARTED || theState==STOPPED)
		clicked_on_watch();
}

void StartStopWatch::keypress_pause()
{
	// let's emulate mouse clicks
	if(theState==RUNNING)
		clicked_on_watch();
}

void StartStopWatch::keypress_reset()
{
	// let's emulate mouse clicks
	if(theState==STOPPED || theState==BROKEN)
		clicked_on_reset();
}

void StartStopWatch::mousePressEvent (QGraphicsSceneMouseEvent * aMouseEvent )
{
	// handle right-mouse button as "always reset"
	if (aMouseEvent->buttons().testFlag(Qt::RightButton))
	{
		clicked_on_reset();
		return;
	}

	QGraphicsItem* myItemPtr = itemAt(aMouseEvent->scenePos());
	if (myItemPtr==theStopWatchSvgPtr || myItemPtr==theStopWatchHandSvgPtr)
		clicked_on_watch();
	if (myItemPtr==theResetSvgPtr)
		clicked_on_reset();
	if (myItemPtr==theFastForwardSvgPtr)
		clicked_on_fastforward();
}

void StartStopWatch::progressHand(void)
{
	// make the transform incremental: rotate some more
	// in fast forward, let's rotate some more more
	if (theFastForwardSvgPtr == NULL)
		theStopWatchHandSvgPtr->setTransform(theFastRotation, true);
	else
		theStopWatchHandSvgPtr->setTransform(theRotation, true);
}

void StartStopWatch::removeFastForwardButton(void)
{
	delete theFastForwardSvgPtr;
	theFastForwardSvgPtr=NULL;
}

void StartStopWatch::removeResetButton(void)
{
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

void StartStopWatch::showFastForwardButton(void)
{
	assert(theFastForwardSvgPtr == NULL);

	// and initialise the reset button
	QSvgRenderer* myRenderer = ImageStore::getRenderer("ActionFastForward");
	theFastForwardSvgPtr = new QGraphicsSvgItem(NULL);
	theFastForwardSvgPtr->setSharedRenderer(myRenderer);
	theFastForwardSvgPtr->setZValue(5.0);
	addItem(theFastForwardSvgPtr);
	theFastForwardSvgPtr->setPos(255-65, 306-65);
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
	displayTooltip(false);
}

void StartStopWatch::stopStopwatch()
{
	theTimer.stop();
	disconnect(&theTimer, SIGNAL(timeout()), this, SLOT(progressHand()));
	emit stopSim();
	displayTooltip(true);
}
