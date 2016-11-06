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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "AbstractObject.h"
#include "PieMenu.h"
#include "Popup.h"
#include "Position.h"
#include "resizinggraphicsview.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "World.h"

#include <QAction>
#include <QGraphicsSceneMouseEvent>

#include "tbe_global.h"

static bool isSimRunning = false;

const int ViewWorld::MAX_FPS = 60;

ViewWorld::ViewWorld (ResizingGraphicsView *aGraphicsViewPtr, World *aWorldPtr)
    : QGraphicsScene(0, -THESCALE * aWorldPtr->getTheWorldHeight(),
                     THESCALE * aWorldPtr->getTheWorldWidth(), THESCALE * aWorldPtr->getTheWorldHeight()),
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

ViewWorld::~ViewWorld()
{
    DEBUG3ENTRY;
}


qreal ViewWorld::getHeight(void) const
{
    return THESCALE * theWorldPtr->getTheWorldHeight();
}


bool ViewWorld::getIsSimRunning()
{
    return isSimRunning;
}


qreal ViewWorld::getWidth(void) const
{
    return THESCALE * theWorldPtr->getTheWorldWidth();
}


void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent *mouseEvent )
{
    // If the simulation is running (or at least is not in reset mode)
    // players cannot change anything.
    if (isSimRunning) {
        // we go at least to pause mode now - update the simcontrols
        emit needPause();
        if (Popup::YesNoQuestion(
                    tr("You cannot make changes now, the simulation is ongoing.\nReset the simulation?")) == true)
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
    // whatever happens, draw every 25 frames
    for (int i = 0; i < 25 && theSimulationTime < myCurrentTime; i++) {
        theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * 2 * theSimSpeed);
    }

    // iterate through all known objects to update the graphics part
    theWorldPtr->updateViewWorld(true);
    theFramesPerSecond++;
}

void ViewWorld::on_framerateTimerTick()
{
// Updating the framerate on MacOSX results in no fps on screen, yet we get
// "QMenu: No OSMenuRef created for popup menu" warnings on the console.
// That's why we do not even try to put it on screen on Mac.
#ifndef Q_WS_MAC
    theFrameRateViewPtr->setText(tr("    %1 fps; %2 s").arg(theFramesPerSecond).arg(
                                     theGameStopwatch.elapsed() / 1000));
#endif
    theFramesPerSecond = 0;
}


void ViewWorld::on_sizeAdjust(void)
{
    setSceneRect(0, -getHeight(), getWidth(), getHeight());
}


void ViewWorld::setupBackground(void)
{
    if (theWorldPtr->theBackground.theBackgroundGradient.count() == 0 &&
            theWorldPtr->theBackground.theImageName.isEmpty()) {
        // the default if nothing else specified: a blue gradient background
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
    }
    setBackgroundBrush(Qt::blue);
    QLinearGradient myBackground(0, 0, 0, -getHeight());
    foreach (Background::GradientStop myGS, theWorldPtr->theBackground.theBackgroundGradient)
        myBackground.setColorAt(myGS.thePosition, QColor(
                                    static_cast<int>(myGS.theR * 255),
                                    static_cast<int>(myGS.theG * 255),
                                    static_cast<int>(myGS.theB * 255),
                                    static_cast<int>(myGS.theAlpha * 255)));
    setBackgroundBrush(myBackground);

    // Set a very light 1-pixel pen line on the left and bottom borders.
    // This will prevent the player to move objects outside the borders.
    QPen myPen(QColor(255, 255, 255, 1), 0);
    addLine(0, 0, 0, -4 * getHeight(), myPen);
    addLine(0, 0, 4 * getWidth(), 0, myPen);
}

void ViewWorld::slot_signalFF()
{
    if (isSimRunning == false)
        slot_signalPlay();
    theSimSpeed = 250;
    emit theTimer.start();
}


void ViewWorld::slot_signal4F()
{
    if (isSimRunning == false)
        slot_signalPlay();
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
//    AnimatedDialog::makeAllAnimatedDialogsDisappear();
    PieMenuSingleton::clearPieMenu();

    if (isSimRunning == false)
        theWorldPtr->createPhysicsWorld();
    isSimRunning = true;
    theSimulationTime = QTime::currentTime();
    theSimSpeed = 1000;
    theFramesPerSecond = 0;
    emit theTimer.start(1000 / MAX_FPS);

    if (theDisplayFramerate) {
        // update framerate every second
        theFramesPerSecond = 0;
        theFramerateTimer.start(1000);
        theGameStopwatch.start();
    } else
        theFrameRateViewPtr->setText("");
}


void ViewWorld::slot_signalReset()
{
    isSimRunning = false;
    emit theTimer.stop();
    emit theFramerateTimer.stop();
    if (theDrawDebug)
        clearGraphicsList(0);
    theWorldPtr->deletePhysicsWorld();
    theWorldPtr->updateViewWorld(false);
}


void ViewWorld::slot_signalSlow()
{
    if (isSimRunning == false)
        slot_signalPlay();
    theSimSpeed = 3000;
    emit theTimer.start();
}

// ---------------------------------------------------------------------------
//                Below is the b2DebugDraw implementation
// ---------------------------------------------------------------------------

const int ViewWorld::theMaxNumberOfGraphicsListElements = 200;

// Draw a closed polygon provided in CCW order.
void ViewWorld::DrawPolygon(UNUSED_ARG const b2Vec2 *vertices, UNUSED_ARG int32 vertexCount,
                            UNUSED_ARG const b2Color &color)
{
    DEBUG5ENTRY;
}

// Draw a solid closed polygon provided in CCW order.
void ViewWorld::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount,
                                 UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::green, 1, Qt::SolidLine);
    QBrush brush(Qt::NoBrush);
    QPolygonF myPoly;
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].x < -0.5 || vertices[i].y < -0.5)
            return;
        myPoly << Vector(vertices[i]).toQPointF();
    }
    addDebugDrawToList(addPolygon(myPoly, pen, brush));
}

// Draw a circle.
void ViewWorld::DrawCircle(UNUSED_ARG const b2Vec2 &center,
                           UNUSED_ARG float32 radius,
                           UNUSED_ARG const b2Color &color)
{
    DEBUG5ENTRY;
}

// Draw a solid circle.
void ViewWorld::DrawSolidCircle(const b2Vec2 &center, float32 radius,
                                UNUSED_ARG const b2Vec2 &axis,
                                UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::green, 1, Qt::SolidLine);
    QBrush brush(Qt::NoBrush);

    Vector myPos = Vector(center) - Vector(radius, -radius);
    QPointF myPosQ = myPos.toQPointF();

    addDebugDrawToList(addEllipse(myPosQ.x(), myPosQ.y(),
                                  2.0 * THESCALE * radius, 2.0 * THESCALE * radius, pen, brush));
}

// Draw a line segment.
void ViewWorld::DrawSegment(UNUSED_ARG const b2Vec2 &p1,
                            UNUSED_ARG const b2Vec2 &p2,
                            UNUSED_ARG const b2Color &color)
{
    QPen pen(Qt::cyan, 1, Qt::SolidLine);

    QPointF myP1 = Vector(p1).toQPointF();
    QPointF myP2 = Vector(p2).toQPointF();

    addDebugDrawToList(addLine(myP1.x(), myP1.y(),
                               myP2.x(), myP2.y(), pen));
}

// Draw a transform. Choose your own length scale.
void ViewWorld::DrawTransform(UNUSED_ARG const b2Transform &xf)
{
    DEBUG5ENTRY;
}

void ViewWorld::addDebugDrawToList(QGraphicsItem *anItem)
{
    anItem->setZValue(150);
    theGraphicsList.push_back(anItem);
    clearGraphicsList(theMaxNumberOfGraphicsListElements);
}

void ViewWorld::clearGraphicsList(int aCount)
{
    while (theGraphicsList.count() > aCount) {
        QGraphicsItem *myItemPtr = theGraphicsList.first();
        theGraphicsList.pop_front();
        delete myItemPtr;
    }
}
