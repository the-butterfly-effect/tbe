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

#include "PieMenu.h"
#include "resizinggraphicsview.h"
#include "ViewWorld.h"
#include "World.h"

#include "Position.h"

#include <QtGui/QGraphicsSceneMouseEvent>

ViewWorld::ViewWorld (ResizingGraphicsView* aGraphicsViewPtr, World* aWorldPtr)
	: QGraphicsScene(0, -THESCALE*aWorldPtr->getTheWorldHeight(),
					 THESCALE*aWorldPtr->getTheWorldWidth(), THESCALE*aWorldPtr->getTheWorldHeight()),
	  theWorldPtr(aWorldPtr),
	  theSimSpeed(1000),
	  isSimRunning(false)
{
	aGraphicsViewPtr->setViewWorld(this, theWorldPtr->getName());

	addRect(0, 0, getWidth(), -getHeight());

	if (theWorldPtr->theBackground.theBackgroundGradient.count()==0 &&
		theWorldPtr->theBackground.theImageName.isEmpty())
	{
		// the default if nothing else specified: a blue gradient background
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
	}
	setBackgroundBrush(Qt::blue);
	QLinearGradient myBackground(0,0, 0,-getHeight());
	foreach(Background::GradientStop myGS, theWorldPtr->theBackground.theBackgroundGradient)
		myBackground.setColorAt(myGS.thePosition, QColor(
									static_cast<int>(myGS.theR*255),
									static_cast<int>(myGS.theG*255),
									static_cast<int>(myGS.theB*255),
									static_cast<int>(myGS.theAlpha*255)));
	setBackgroundBrush(myBackground);

//	connect(&theFramerateTimer, SIGNAL(timeout()), this, SLOT(on_framerateTimerTick()));
}


qreal ViewWorld::getHeight()
{
	return THESCALE*theWorldPtr->getTheWorldHeight();
}


qreal ViewWorld::getWidth()
{
	return THESCALE*theWorldPtr->getTheWorldWidth();
}


void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
    QGraphicsItem* myItemPtr = itemAt(mouseEvent->scenePos());
    if (myItemPtr!=NULL)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }
    // nothing clicked: clear pie menu
    PieMenuSingleton::clearPieMenu();
}


void ViewWorld::on_timerTick()
{
	while(theSimulationTime < QTime::currentTime())
	{
		theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * theSimSpeed);
	}
	advance();
	theFramesPerSecond++;
}

//void ViewWorld::on_framerateTimerTick()
//{
//	theMainWindowPtr->statusBar()->showMessage(tr("Framerate: %1 fps").arg(theFramesPerSecond), 3500);
//	theFramesPerSecond = 0;
//}



/// public slot: reset the scene
void ViewWorld::slot_resetSim()
{
	emit theTimer.stop();
	isSimRunning = false;
}


void ViewWorld::slot_signalFF()
{
	theSimSpeed = 250;
	emit theTimer.start();
}

void ViewWorld::slot_signalPause()
{
	theSimSpeed = 0;
	emit theTimer.stop();
}

void ViewWorld::slot_signalPlay()
{
	if (isSimRunning==false)
		slot_startSim();
	isSimRunning=true;
	theSimSpeed = 1000;
	emit theTimer.start();
}

void ViewWorld::slot_signalReset()
{
	isSimRunning=false;
	emit theTimer.stop();
}

void ViewWorld::slot_startSim()
{
	DEBUG1ENTRY;

	// TODO: upon reset, the old object gets focus again - thanks, Qt!
//	DrawObject myTemp(NULL);
//	setFocusItem(&myTemp);
//	clearFocus();
//	Anchors::clearEditObjectDialogPtr();

	theWorldPtr->createPhysicsWorld();
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));
	theTimer.start(1000/25);
	theSimulationTime = QTime::currentTime();
	isSimRunning = true;

//	if (theDisplayFramerate)
//	{
//		// update framerate every second
//		theFramerateTimer.start(1000);
//		theFramesPerSecond = 0;
//	}
}
