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

#ifndef SIMULATIONCONTROLS_H
#define SIMULATIONCONTROLS_H

#include <QtGui>
#include "tbe_global.h"

class Overlay;

/// This direct QState derivative was created to have meaningful
/// debug messages when SimulationControls switches state
class SimState : public QState
{
	Q_OBJECT
public:
	explicit SimState(QState* parent = 0, const QString& aStateName = "")
		: QState(parent), theName(aStateName) {}

	virtual ~SimState()
	{}

protected:
	virtual void onEntry ( QEvent * event );

private:
	QString theName;
};



/** This state machine controls how the simulation runs
  * inputs are the QActions that it creates in the menu bar and slots on its interface
  * outputs are the signals that it sends when states change
  */
class SimulationControls : public QObject
{
    Q_OBJECT

public:
	explicit SimulationControls(QObject *parent = 0);

	void setup(QMenu* aMenuPtr, Overlay* anOverlayPtr);

signals:
	/// this signal is emitted when our slot onFailed is called
	void failed(void);

public slots:
	/// call this slot if the simulation has failed, the internal
	/// state machine will go to the Failed state.
	void onFailed(void)
	{ emit failed(); }

private:
	QStateMachine theSimStateMachine;

	QIcon thePlayIcon;
	QIcon thePauseIcon;
	QIcon theStopIcon;
	QIcon theResetIcon;
	QIcon theFwdIcon;
};

#endif // SIMULATIONCONTROLS_H
