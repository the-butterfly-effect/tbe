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

#ifndef STARTSTOPWATCH_H
#define STARTSTOPWATCH_H

#include <QtGui>
#include "ui_MainWindow.h"


// forward declarations
class QGraphicsSvgItem;
class QSvgRenderer;

/** This is the new view/controller class for start/stop.
  * It uses an animated representation of a stopwatch
  * to make sure that it is better visible the simulation is still running
  */
class StartStopWatch : public QGraphicsScene
{
	Q_OBJECT

public:
	/// constructor
	StartStopWatch();
	virtual ~StartStopWatch();

	enum TheStates
	{
		NOTSTARTED,	// stopwatch is stationary at zero
		STOPPED,	// stopwatch is stationary at nonzero (also show reset button)
		RUNNING,	// stopwatch is running
		FAST,		// stopwatch is running in fast forward
		BROKEN,		// stopwatch is broken - probably because "butterfly has died"
	};

	/// the base of the state machine
	void goToState(TheStates aNewState);

	/// set tooltip to display or remove tooltip to display
	/// @param isToSet true if is to display tooltip, false if not to show it
	void displayTooltip(bool isToSet);

public slots:
	void clicked_on_watch();
	void clicked_on_reset();
	void clicked_on_fastforward();

	void keypress_go();
	void keypress_pause();
	void keypress_reset();

signals:
	/// signal: start (or continue) simulation
	void startSim();

	/// signal: stop simulation
	void stopSim();

	/// signal: when World needs to reset to its original position
	void resetSim(void);

	/// signal: speed up
	void goFast();

	/// signal: slow down
	void goSlow();

protected:
	/// overridden to capture mouse clicks on the stopwatch
	virtual void mousePressEvent (QGraphicsSceneMouseEvent * aMouseEvent );

private slots:
	/// the actual member that rotates the seconds hand
	void progressHand();

private:
	void removeFastForwardButton();
	void removeResetButton();
	void showFastForwardButton();
	void showResetButton();
	void showWatch(bool isBroken);

	void startStopwatch();
	void stopStopwatch();
	void resetStopwatch();

	QTimer theTimer;
	QGraphicsSvgItem* theStopWatchSvgPtr;
	QGraphicsSvgItem* theStopWatchHandSvgPtr;
	QTransform	theRotation;
	QTransform	theFastRotation;
	QGraphicsSvgItem* theResetSvgPtr;
	QGraphicsSvgItem* theFastForwardSvgPtr;

	QSvgRenderer* theBrokenStopwatchRendererPtr;
	QSvgRenderer* theStopwatchRendererPtr;

	/** the variable behind the state machine
	 *  The only member allowed to touch theState is goToState()!!!
	 */
	TheStates theState;
};

#endif // STARTSTOPWATCH_H
