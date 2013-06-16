/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012 Klaas van Gend
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

#include "AbstractObject.h"
#include "animateddialog.h"
#include "PieMenu.h"
#include "Popup.h"
#include "Position.h"
#include "resizinggraphicsview.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "World.h"

#include <QtGui/QAction>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "tbe_global.h"

static bool isSimRunning = false;

ViewWorld::ViewWorld (ResizingGraphicsView* aGraphicsViewPtr, World* aWorldPtr)
	: QGraphicsScene(0, -THESCALE*aWorldPtr->getTheWorldHeight(),
					 THESCALE*aWorldPtr->getTheWorldWidth(), THESCALE*aWorldPtr->getTheWorldHeight()),
	  theWorldPtr(aWorldPtr),
	  theSimSpeed(1000)
{
	aGraphicsViewPtr->setViewWorld(this, theWorldPtr->getName());
	theFrameRateViewPtr = aGraphicsViewPtr->getFrameRateViewPtr();

    setupBackground();

    connect(&theFramerateTimer, SIGNAL(timeout()), this, SLOT(on_framerateTimerTick()));
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));
	isSimRunning = false;
}


qreal ViewWorld::getHeight(void) const
{
	return THESCALE*theWorldPtr->getTheWorldHeight();
}


bool ViewWorld::getIsSimRunning()
{
	return isSimRunning;
}


qreal ViewWorld::getWidth(void) const
{
	return THESCALE*theWorldPtr->getTheWorldWidth();
}


void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
    // If the simulation is running (or at least is not in reset mode)
    // players cannot change anything.
    if (isSimRunning)
    {
        emit slot_signalPause();
        if (Popup::YesNoQuestion(tr("You cannot make changes now, the simulation is ongoing.\nReset the simulation?"))==true)
            emit needReset();
        return;
    }
    PieMenuSingleton::startClickCheck();
    QGraphicsScene::mousePressEvent(mouseEvent);
    PieMenuSingleton::endClickCheck();
}


void ViewWorld::on_timerTick()
{
	QTime myCurrentTime = QTime::currentTime();
	while(theSimulationTime < myCurrentTime)
	{
		theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * 2 * theSimSpeed);
	}

	// iterate through all known objects to update the graphics part
	theWorldPtr->updateViewWorld(true);
	theFramesPerSecond++;
}

void ViewWorld::on_framerateTimerTick()
{
	theFrameRateViewPtr->setText(tr("%1 fps").arg(theFramesPerSecond));
	theFramesPerSecond = 0;
}

void ViewWorld::setupBackground(void)
{
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
}

void ViewWorld::slot_signalFF()
{
    theSimSpeed = 250;
    emit theTimer.start();
}


void ViewWorld::slot_signal4F()
{
    theSimSpeed = 60;
    emit theTimer.start();
}


void ViewWorld::slot_signalPause()
{
	emit theTimer.stop();
	emit theFramerateTimer.stop();
}


void ViewWorld::slot_signalPlay()
{
	// remove any dialogs when user starts playing
	AnimatedDialog::makeAllAnimatedDialogsDisappear();
	PieMenuSingleton::clearPieMenu();

	if (isSimRunning==false)
		theWorldPtr->createPhysicsWorld();
	isSimRunning=true;
	theSimulationTime = QTime::currentTime();
	theSimSpeed = 1000;
	theFramesPerSecond = 0;
	emit theTimer.start(1000/50);

	if (theDisplayFramerate)
	{
		// update framerate every second
		theFramesPerSecond = 0;
		theFramerateTimer.start(1000);
	}
	else
		theFrameRateViewPtr->setText("");
}


void ViewWorld::slot_signalReset()
{
	isSimRunning=false;
	emit theTimer.stop();
	emit theFramerateTimer.stop();
	if (theDrawDebug)
		clearGraphicsList(0);
	theWorldPtr->deletePhysicsWorld();
	theWorldPtr->updateViewWorld(false);
}


// ---------------------------------------------------------------------------
//                Below is the b2DebugDraw implementation
// ---------------------------------------------------------------------------

const int ViewWorld::theMaxNumberOfGraphicsListElements = 200;

// Draw a closed polygon provided in CCW order.
void ViewWorld::DrawPolygon(UNUSED_ARG const b2Vec2* vertices, UNUSED_ARG int32 vertexCount, UNUSED_ARG const b2Color& color)
{
	DEBUG5("ViewWorld::DrawPolygon\n");
}

// Draw a solid closed polygon provided in CCW order.
void ViewWorld::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
								 UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 1, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);
	QPolygonF myPoly;
	for (int i=0; i<vertexCount; i++)
	{
		if (vertices[i].x < -0.5 || vertices[i].y < -0.5)
			return;
		myPoly << Vector(vertices[i]).toQPointF();
	}
	addDebugDrawToList(addPolygon(myPoly, pen, brush));
}

// Draw a circle.
void ViewWorld::DrawCircle(UNUSED_ARG const b2Vec2& center,
						   UNUSED_ARG float32 radius,
						   UNUSED_ARG const b2Color& color)
{
	DEBUG5("ViewWorld::DrawCircle\n");
}

// Draw a solid circle.
void ViewWorld::DrawSolidCircle(const b2Vec2& center, float32 radius,
								UNUSED_ARG const b2Vec2& axis,
								UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 1, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);

	Vector myPos = Vector(center) - Vector(radius,-radius);
	QPointF myPosQ = myPos.toQPointF();

	addDebugDrawToList(addEllipse(myPosQ.x(),myPosQ.y(),
								  2.0*THESCALE*radius,2.0*THESCALE*radius, pen, brush));
}

// Draw a line segment.
void ViewWorld::DrawSegment(UNUSED_ARG const b2Vec2& p1,
							UNUSED_ARG const b2Vec2& p2,
							UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::cyan, 1, Qt::SolidLine);

	QPointF myP1 = Vector(p1).toQPointF();
	QPointF myP2 = Vector(p2).toQPointF();

	addDebugDrawToList(addLine(myP1.x(),myP1.y(),
							   myP2.x(),myP2.y(), pen));
}

// Draw a transform. Choose your own length scale.
void ViewWorld::DrawTransform(UNUSED_ARG const b2Transform& xf)
{
	DEBUG5("ViewWorld::DrawTransform\n");
}

void ViewWorld::addDebugDrawToList(QGraphicsItem* anItem)
{
	anItem->setZValue(150);
	theGraphicsList.push_back(anItem);
	clearGraphicsList(theMaxNumberOfGraphicsListElements);
}

void ViewWorld::clearGraphicsList(int aCount)
{
	while (theGraphicsList.count() > aCount)
	{
		QGraphicsItem* myItemPtr = theGraphicsList.first();
		theGraphicsList.pop_front();
		delete myItemPtr;
	}
}
