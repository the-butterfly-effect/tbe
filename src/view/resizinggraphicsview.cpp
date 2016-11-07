/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2013 Klaas van Gend
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

#include "EditObjectDialog.h"
#include "GameQControls.h"
#include "GameFlow.h"
#include "Level.h"
#include "LevelCreator.h"
#include "MainWindow.h"
#include "Popup.h"
#include "RegressionTest.h"
#include "resizinggraphicsview.h"
#include "Translator.h"
#include "ViewObjectActionDectorator.h"
#include "ViewObject.h"
#include "ViewWorld.h"
#include "World.h"

#include <QMenuBar>
#include <QQuickItem>

static ResizingGraphicsView *theRSGVPtr = nullptr;


ResizingGraphicsView::ResizingGraphicsView(QWidget *aParentPtr) :
    QGraphicsView(aParentPtr),
    theMainWindowPtr(nullptr),
    theObjectEditorPtr(nullptr),
    theGameStateMachinePtr(nullptr),
    theFrameRateViewPtr(nullptr)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setDragMode(QGraphicsView::NoDrag);
    setFrameStyle(QFrame::Plain + QFrame::NoFrame);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    theRSGVPtr = this;
}


ResizingGraphicsView::~ResizingGraphicsView()
{
    theRSGVPtr = nullptr;
}


void ResizingGraphicsView::clearViewWorld(void)
{
    DEBUG3ENTRY;
    // disconnect & delete the Scene//DrawWorld
    // keep in mind that we have a view that's not happy now!
    setScene(nullptr);
    QMatrix myMatrix;
    setMatrix(myMatrix);

    emit theGameFlowPtr->slot_clearDialog();
    delete theScenePtr;
    theScenePtr = nullptr;
}


ResizingGraphicsView *ResizingGraphicsView::me(void)
{
    Q_ASSERT(theRSGVPtr != nullptr);
    return theRSGVPtr;
}


void ResizingGraphicsView::resizeEvent(QResizeEvent *event)
{
    if (event != nullptr)
        QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void ResizingGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event != nullptr) {
        QGraphicsView::mouseMoveEvent(event);

        QPointF mousePos = this->mapToScene(event->pos());
        Position p = Position(mousePos, 0);
        emit theMainWindowPtr->slot_mouse_move(p.x, p.y);
    }
}

void ResizingGraphicsView::setup(MainWindow *aMWPtr, GameFlow *aGFPtr, GameStateMachine *aGSMPtr,
                                 QMenuBar *aMenuBarPtr, QMenu *anMenuControlsPtr)
{
    theGameFlowPtr = aGFPtr;
    theMainWindowPtr = aMWPtr;
    GameQControls::me()->setup(anMenuControlsPtr);
    connect(CrossRegisterSingleton::me(), SIGNAL(signalNumberCrossesChanged(int)), aGSMPtr,
            SLOT(slot_NumberOfCrossesChanged(int)));

    connect (aGSMPtr, SIGNAL(signal_State_Changed(GameStateMachine::States)),
             GameQControls::me(), SLOT(slot_updateIcon(GameStateMachine::States)));

    theGameStateMachinePtr = aGSMPtr;
    connect (GameQControls::me(), SIGNAL(signal_Forward_triggered()),  aGSMPtr,
             SIGNAL(signal_Forward_triggered()));
    connect (GameQControls::me(), SIGNAL(signal_Pause_triggered()),    aGSMPtr,
             SIGNAL(signal_Pause_triggered()));
    connect (GameQControls::me(), SIGNAL(signal_Play_triggered()),     aGSMPtr,
             SIGNAL(signal_Play_triggered()));
    connect (GameQControls::me(), SIGNAL(signal_RealFast_triggered()), aGSMPtr,
             SIGNAL(signal_RealFast_triggered()));
    connect (GameQControls::me(), SIGNAL(signal_Reset_triggered()),    aGSMPtr,
             SIGNAL(signal_Reset_triggered()));
    connect (GameQControls::me(), SIGNAL(signal_Slow_triggered()),     aGSMPtr,
             SIGNAL(signal_Slow_triggered()));

    // this one displays the frame rate counter if active
    theFrameRateViewPtr = aMenuBarPtr->addAction("");
}


void ResizingGraphicsView::setViewWorld(ViewWorld *aScenePtr,
                                        const QString &aLevelName)
{
    DEBUG1("ResizingGraphicsView::setViewWorld(%p, \"%s\")", aScenePtr,
           ASCII(aLevelName));
    theScenePtr = aScenePtr;

    setScene(aScenePtr);
    fitInView(0, -aScenePtr->getHeight(),
              aScenePtr->getWidth(), aScenePtr->getHeight());
    resizeEvent(nullptr);
    theMainWindowPtr->setWindowTitle(APPNAME + " - " + TheGetText(aLevelName));

    connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()),   theGameStateMachinePtr,
            SIGNAL(signal_Won_happened()));
    connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), theGameStateMachinePtr,
            SIGNAL(signal_Fail_happened()));
    connect(aScenePtr, SIGNAL(needPause()),   theGameStateMachinePtr, SIGNAL(signal_Pause_triggered()));
    connect(aScenePtr, SIGNAL(needReset()),   theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));

    if (theIsRunAsRegression) {
        connect(aScenePtr->getWorldPtr(), SIGNAL(signalWon()), theMainWindowPtr->theRegressionTest,
                SLOT(slot_Won()));
        connect(aScenePtr->getWorldPtr(), SIGNAL(signalDeath()), theMainWindowPtr->theRegressionTest,
                SLOT(slot_Fail()));
    }

    connect(theGameStateMachinePtr, SIGNAL(signal_Forward_triggered()),  aScenePtr,
            SLOT(slot_signalFF()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Play_triggered()),     aScenePtr,
            SLOT(slot_signalPlay()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Pause_triggered()),    aScenePtr,
            SLOT(slot_signalPause()));
    connect(theGameStateMachinePtr, SIGNAL(signal_RealFast_triggered()), aScenePtr,
            SLOT(slot_signal4F()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Reset_triggered()),    aScenePtr,
            SLOT(slot_signalReset()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Slow_triggered()),     aScenePtr,
            SLOT(slot_signalSlow()));
    connect(theGameStateMachinePtr, SIGNAL(signal_Stop_Gameplay()),      aScenePtr,
            SLOT(slot_signalPause()));

    theGameFlowPtr->slot_showLevelInfoDialog();
}
