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
#include "World.h"

#include <QMenuBar>
#include <QQuickItem>

static ResizingGraphicsView *theRSGVPtr = nullptr;


ResizingGraphicsView::ResizingGraphicsView(QWidget *aParentPtr) :
    QGraphicsView(aParentPtr),
    theMainWindowPtr(nullptr),
    theObjectEditorPtr(nullptr),
    theGameStateMachinePtr(nullptr)
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


ResizingGraphicsView *ResizingGraphicsView::me(void)
{
    Q_ASSERT(theRSGVPtr != nullptr);
    return theRSGVPtr;
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
                                 QMenu *anMenuControlsPtr)
{
    theGameFlowPtr = aGFPtr;
    theMainWindowPtr = aMWPtr;
    GameQControls::me()->setup(anMenuControlsPtr);

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


}
