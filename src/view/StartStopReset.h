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

#ifndef STARTSTOPRESET_H
#define STARTSTOPRESET_H

#include <QtGui>

#include "ui_MainWindow.h"

/** class StartStopReset
  *
  * This class implements the state machine around start/stop/continue/reset.
  */

class StartStopReset : public QObject
{
public:

	Q_OBJECT

	enum TheStates
	{
		NOTSTARTED,	// pushbutton should show START
		STOPPED,	// pushbutton should show CONTINUE
		RUNNING		// pushbutton should show STOP 
	};

public:
	// Constructors/Destructors
	//  

	/**
	 * Empty Constructor
	 */
	StartStopReset (Ui::MainWindow* aMainWindowUIPtr );

	/**
	 * Empty Destructor
	 */
	virtual ~StartStopReset ( );


	void goToState(TheStates aNewState);

signals:
	/// this signal is emitted on each timer step
	void runSimStep(void);
	
	/// this signal is emitted when the world needs to reset to its original position
	void resetSim(void);
	
public slots:
	/** public slot for Reset UI actions
	 *  if simulation is not running, it will tell the world
	 *  to its starting point and reset theState to NOTSTARTED 
	 */
	void on_pushButton_Reset_clicked(void);
	
	/** public slot for Start/Stop/Continue actions
	 */
	void on_pushButton_Toggle_clicked(void);

private slots:
	/// called whenever a timer tick happens
	void on_timerTick(void);
	
private:
	void setMenuState(void);
	void setPushButtonState(void);
	void setResetButtonState(bool isOn);

	
	void startSim();
	void stopSim();
	
private:
	Ui::MainWindow* theMainWindowUIPtr;

	/** the variable behind the state machine
	 *  The only member allowed to touch theState is goToState()!!!
	 */
	TheStates theState;
	
	QTimer theTimer;
};

#endif // DRAWOBJECT_H
