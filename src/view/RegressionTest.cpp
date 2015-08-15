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
	theLevelIndex(0),
	theMainWindowPtr(parent),
	theState(0)
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
	theState = 0;
	theRegressionTimer.singleShot(2000, this, SLOT(slotRegressionProgress()));
}

void RegressionTest::slotRegressionProgress(void)
{

	int myNextDelay = 0;

	QStringList myLevelParams = theLevels[theLevelIndex].split(':');
	QString myLevelName = myLevelParams[0];
	int myLevelDurationSeconds = myLevelParams[1].toInt();

	DEBUG1("AUTOMATED TESTING, LEVEL %d STATE %d--------------------------", theLevelIndex, theState);
	switch (theState)
	{
	case 0: // Load Level
		DEBUG1("AUTOMATED TESTING OF LEVEL %s", ASCII(myLevelName));
		theMainWindowPtr->loadLevel(myLevelName);
		myNextDelay= 1500;
		break;
	case 1: // Start Level, expect failure - either death or timeout
	{
		QKeyEvent* myEvent1Ptr = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent1Ptr);
		QKeyEvent* myEvent2Ptr = new QKeyEvent ( QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
		QCoreApplication::postEvent (theMainWindowPtr, myEvent2Ptr);
		myNextDelay= myLevelDurationSeconds*1000;
		// Registering for fail event (and success event!) was already done in resizinggraphicsview
		break;
	}
	case 2: // Check for "not success" (i.e. death or timeout)
		theRegressionTimer.stop();
		myNextDelay= 1000;
		break;
	case 3: // NOP
		// TODO: implement
		break;
	case 4: // Reset
		// TODO: implement
		break;
	case 5: // Setup all hints
		// TODO: implement
		break;
	case 6: // Start Level, expect success
		// TODO: implement
		break;
	case 7: // Check for success
		theRegressionTimer.stop();
		// TODO: implement
		break;
	case 8: // Continue with next item in test
		// TODO: implement
		break;
	default:
		// if we get here, something went terribly wrong...
		DEBUG1("Hit catch-all state - something went wrong");
		exit (5);
		break;
	}
	theState++;
	Q_ASSERT(myNextDelay!=0);
	theRegressionTimer.singleShot(myNextDelay, this, SLOT(slotRegressionProgress()));
}
