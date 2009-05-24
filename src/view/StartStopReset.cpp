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

#include "StartStopReset.h"
#include "tbe_global.h"

#include "MainWindow.h"
#include "DrawWorld.h"

#include <cassert>

// Constructors/Destructors
//  

StartStopReset::StartStopReset (Ui::MainWindow* aMainWindowUIPtr)
	: theMainWindowUIPtr(aMainWindowUIPtr), theState(StartStopReset::NOTSTARTED)
{
	DEBUG5("StartStopReset::StartStopReset\n");
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));
}

StartStopReset::~StartStopReset ( ) 
{
	// nothing to do here
}

//  
// Methods
//  


// Slots
//  

void StartStopReset::on_pushButton_Reset_clicked(void)
{
	DEBUG4("StartStopReset::on_pushButton_Reset_clicked(void) whilst in state %d\n", theState);
	if (theState != STOPPED)
	{
		DEBUG2("Reset activated while not in STOPPED mode\n");
		// TODO: remove this assert once made sure it cannot be called
		assert(false);
		return;
	}
	goToState(NOTSTARTED);
}

void StartStopReset::on_pushButton_Toggle_clicked(void)
{
	DEBUG4("StartStopReset::on_pushButton_Toggle_clicked(void) whilst in state %d\n", theState);
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
	}
	
}

// Accessor methods
//  


// Other methods
//  

void StartStopReset::goToState(TheStates aNewState)
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
				startSim();
				theState = aNewState;
				break;
			}
			break;
		}
	case STOPPED:
		{
			switch (aNewState)
			{
			case NOTSTARTED:	// reset button clicked
				resetSim();
				theState = aNewState;
				break;
			case STOPPED:		// no need for action
				break;
			case RUNNING:		// continue button clicked
				startSim();
				theState = aNewState;
				break;
			}
			break;
		}
	case RUNNING: 
		{
			switch (aNewState)
			{
			case NOTSTARTED:	// should not happen - reset is disabled during running
				assert(false);
				break;
			case STOPPED:		// stop button clicked
				stopSim();
				theState = aNewState;
				break;
			case RUNNING:		// no need for action
				break;
			}
			break;
		}
	} // end-of-switch

	setPushButtonState();
	setMenuState();
	setResetButtonState(theState==STOPPED);
}

void StartStopReset::on_timerTick()
{
	// TODO: make this real time.
	// this is probably done easiest by linking up the Level/World immediately, so
	// we do not need to go through DrawWorld.
	// (and we probably need to run multiple time steps per timer Tick)
	emit reinterpret_cast<DrawWorld*>(theMainWindowUIPtr->graphicsView->scene())->timeStep();
}

void StartStopReset::resetSim(void)
{
	// TODO: implement this
}

void StartStopReset::setMenuState(void)
{
	// TODO: implement this
}

void StartStopReset::setPushButtonState(void)
{
	QString myButtonText;
	switch(theState)
	{
	case NOTSTARTED:
		myButtonText = tr("START");
		break;
	case STOPPED:
		myButtonText = tr("CONTINUE");
		break;
	case RUNNING: 
		myButtonText = tr("STOP");
		break;
	}
	theMainWindowUIPtr->pushButton_StartStopContinue->setText(myButtonText);
}

void StartStopReset::setResetButtonState(bool isOn)
{
	theMainWindowUIPtr->pushButton_Reset->setEnabled(isOn);
}

void StartStopReset::startSim(void)
{
	DEBUG5("StartStopReset::startSim(void)\n");
	theTimer.start(1000/25);
}

void StartStopReset::stopSim(void)
{
	DEBUG5("StartStopReset::stopSim(void)\n");
	theTimer.stop();
}

