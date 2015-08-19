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

#ifndef REGRESSIONTEST_H
#define REGRESSIONTEST_H

#include "tbe_global.h"

#include <MainWindow.h>

#include <QObject>
#include <QStringList>
#include <QTimer>

class RegressionTest : public QObject
{
	Q_OBJECT
public:

	enum States
	{
		START = 0,
		LOADLEVEL,
		STARTLEVELTOFAIL,
		LEVELFAILED,
		STOPANDRESETLEVEL,
		RESETLEVEL,
		ADDHINTS,
		STARTLEVELTOWIN,
		LEVELWON,
		NEXTLEVEL,
	};


	explicit RegressionTest(MainWindow *parent = 0);

	/// Starts the automated regression tests
	/// TODO: for now contains the entire event state machine for tests
	/// @note: can only be started if the is true
	void startRegressionRun(void);

signals:

public slots:
	void slot_Won();
	void slot_Fail();

private slots:
	///
	void slotRegressionProgress(void);

private:

	bool theIsWon;
	bool theIsFail;
	bool theWantWonFail;	/// true if slot_Won or slot_Fail should work...

	int theLevelIndex;
	QStringList theLevels;
	MainWindow* theMainWindowPtr;
	States theState;
	QTimer theRegressionTimer;
};

#endif // REGRESSIONTEST_H
