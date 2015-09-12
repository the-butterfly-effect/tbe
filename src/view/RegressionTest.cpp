/* The Butterfly Effect
 * This file copyright (C) 2015 Klaas van Gend
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "RegressionTest.h"

#include <QCoreApplication>
#include <QEvent>
#include <QKeyEvent>

#if 0
/// for use with automated testing within Valgrid or Pareon Verify
const qreal theTestMultiplier = 3.0;
const qreal theTestAddition   = 0.2;
#else
/// for use without any tools
const qreal theTestMultiplier = 1.0;
const qreal theTestAddition   = 0.0;
#endif

RegressionTest::RegressionTest(MainWindow *parent) :
	QObject(parent),
	theIsWon(false),
	theIsFail(false),
	theHintIndex(-1),
	theLevelIndex(0),
	theMainWindowPtr(parent),
	theState(RegressionTest::START)
{
	theLevels = theStartFileName.split(",");
	DEBUG1("levels: %s", ASCII(theStartFileName));
	connect(&theRegressionTimer, SIGNAL(timeout()), this, SLOT(slotRegressionProgress()));

	// set up the state names. Note: you should keep the numbers from low to high...
	theStateNames.insert(START,             "Start");
	theStateNames.insert(LOADLEVEL,         "Load Level");
	theStateNames.insert(STARTLEVELTOFAIL,  "Start Level to Fail");
	theStateNames.insert(LEVELFAILED,       "Level Failed (expected)");
	theStateNames.insert(STOPANDRESETLEVEL, "Stop and Reset Level");
	theStateNames.insert(RESETLEVEL,        "Reset Level");
	theStateNames.insert(ADDHINTS,          "Add All Hints");
	theStateNames.insert(STARTLEVELTOWIN,   "Start Level to Win");
	theStateNames.insert(LEVELWON,          "Level Won (expected)");
	theStateNames.insert(NEXTLEVEL,         "Next Level");
}

void RegressionTest::startRegressionRun(void)
{
	Q_ASSERT(theIsRunAsRegression);
	if(theIsRunAsRegression==false)
		return;
	theLevelIndex = 0;
	theState = LOADLEVEL;
	theRegressionTimer.start(200);
}

void RegressionTest::slot_Fail()
{
	DEBUG1("AUTOMATED TESTING, slot_Fail--------------------------");
	theIsFail = true;
	if (theWantWonFail)
	{
		theWantWonFail = false;
		slotRegressionProgress();
	}
}

void RegressionTest::slot_Won()
{
	DEBUG1("AUTOMATED TESTING, slot_Won--------------------------");
	theIsWon = true;
	if (theWantWonFail)
	{
		theWantWonFail = false;
		slotRegressionProgress();
	}
}

void RegressionTest::slotRegressionProgress(void)
{

	int myNextDelay = 0;
	QStringList myLevelParams = theLevels[theLevelIndex].split(':');
	QString myLevelName = myLevelParams[0];
	int myLevelDurationSeconds = myLevelParams[1].toInt();
	States myNextState = START;

	DEBUG1("AUTOMATED TESTING, LEVEL %d STATE %d '%s'--------------------------",
		   theLevelIndex, theState, ASCII(theStateNames[theState]));
	switch (theState)
	{
	case START:
		Q_ASSERT(false);
		break;
	case LOADLEVEL: // Load Level
		DEBUG1("AUTOMATED TESTING OF LEVEL %s", ASCII(myLevelName));
		theMainWindowPtr->loadLevel(myLevelName);
		myNextDelay= 1500;
		myNextState = STARTLEVELTOFAIL;
		break;
	case STARTLEVELTOFAIL: // Start Level, expect failure - either death or timeout
	{
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		myNextDelay= myLevelDurationSeconds*1000;
		theIsWon = false;
		theIsFail = false;
		theWantWonFail = true;
		// Registering for fail event (and success event!) was already done in resizinggraphicsview
		myNextState = LEVELFAILED;
		break;
	}
	case LEVELFAILED: // Check for "not success" (i.e. death or timeout)
		theWantWonFail = false;
		theRegressionTimer.stop();
		Q_ASSERT(theIsWon==false);
		myNextDelay= 800;
		if (theIsFail)
			myNextState= RESETLEVEL;
		else
			myNextState= STOPANDRESETLEVEL;
		break;
	case STOPANDRESETLEVEL: // Reset after timeout
	{
		theWantWonFail = false;
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		// FIXME: This is not i18n proof: We're just pressing Alt-R here...
		QKeyEvent* myEvent3Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_R , Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent3Ptr);
		QKeyEvent* myEvent4Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_R , Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent4Ptr);
		myNextDelay= 800;
		myNextState= ADDHINTS;
		break;
	}
	case RESETLEVEL: // Reset
	{
		// FIXME: This is not i18n proof: We're just pressing Alt-R here...
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_R , Qt::AltModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_R , Qt::AltModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		myNextDelay= 800;
		myNextState= ADDHINTS;
		break;
	}
	case ADDHINTS: // Setup all hints
	{
		int i=0;
		for(; i<100; i++)
		{
			if (!theMainWindowPtr->slot_insertHint(i))
				break;
		}
		DEBUG2("Added %d hints", i);
		myNextDelay= 800;
		myNextState= STARTLEVELTOWIN;
		break;
	}
	case STARTLEVELTOWIN: // Start Level, expect success
	{
		theWantWonFail = true;
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		myNextDelay= myLevelDurationSeconds*1000;
		theIsWon = false;
		theIsFail = false;
		// Registering for success event (and fail event!) was already done in resizinggraphicsview
		myNextState = LEVELWON;
		break;
	}
	case LEVELWON: // Check for success
		theWantWonFail = false;
		theRegressionTimer.stop();
		Q_ASSERT(theIsWon==true);
		myNextState = NEXTLEVEL;
		myNextDelay= 4000;
		break;
	case NEXTLEVEL: // Continue with next item in test
		theWantWonFail = false;
		theLevelIndex++;
		// are we done?
		if (theLevelIndex >= theLevels.count())
			exit(0);
		myNextState = LOADLEVEL;
		myNextDelay= 1;
		break;
	default:
		// if we get here, something went terribly wrong...
		DEBUG1("Hit catch-all state - something went wrong");
		exit (5);
		break;
	}
	Q_ASSERT(myNextState != START);
	theState = myNextState;
	Q_ASSERT(myNextDelay!=0);
	theRegressionTimer.start(myNextDelay*theTestMultiplier+theTestAddition);
}
