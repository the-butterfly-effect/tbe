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

RegressionTest::RegressionTest(MainWindow *parent) :
	QObject(parent),
	theIsWon(false),
	theIsFail(false),
	theLevelIndex(0),
	theMainWindowPtr(parent),
	theState(RegressionTest::START)
{
	theLevels = theStartFileName.split(",");
	DEBUG1("levels: %s", ASCII(theStartFileName));
}

void RegressionTest::startRegressionRun(void)
{
	Q_ASSERT(theIsRunAsRegression);
	if(theIsRunAsRegression==false)
		return;
	theLevelIndex = 0;
	theState = LOADLEVEL;
	theRegressionTimer.singleShot(200, this, SLOT(slotRegressionProgress()));
}

void RegressionTest::slot_Fail()
{
	theIsFail = true;
	slotRegressionProgress();
}

void RegressionTest::slot_Won()
{
	theIsWon = true;
	slotRegressionProgress();
}

void RegressionTest::slotRegressionProgress(void)
{

	int myNextDelay = 0;

	QStringList myLevelParams = theLevels[theLevelIndex].split(':');
	QString myLevelName = myLevelParams[0];
	int myLevelDurationSeconds = myLevelParams[1].toInt();
	States myNextState = START;

	DEBUG1("AUTOMATED TESTING, LEVEL %d STATE %d--------------------------", theLevelIndex, theState);
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
		// Registering for fail event (and success event!) was already done in resizinggraphicsview
		myNextState = LEVELFAILED;
		break;
	}
	case LEVELFAILED: // Check for "not success" (i.e. death or timeout)
		theRegressionTimer.stop();
		Q_ASSERT(theIsWon==false);
		myNextDelay= 400;
		myNextState= RESETLEVEL;
		break;
	case RESETLEVEL: // Reset
	{
		// FIXME: This is not i18n proof: We're just pressing Alt-R here...
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_R , Qt::AltModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_R , Qt::AltModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		myNextDelay= 800;
		myNextState= STARTLEVELTOFAIL;
		break;
	}
	case ADDHINTS: // Setup all hints
		// TODO: implement
		break;
	case STARTLEVELTOWIN: // Start Level, expect success
		// TODO: implement
		break;
	case LEVELWON: // Check for success
		theRegressionTimer.stop();
		// TODO: implement
		break;
	case NEXTLEVEL: // Continue with next item in test
		// TODO: implement
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
	theRegressionTimer.singleShot(myNextDelay, this, SLOT(slotRegressionProgress()));
}
