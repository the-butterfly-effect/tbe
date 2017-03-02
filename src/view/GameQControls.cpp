/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2013,2016 Klaas van Gend
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

#include "tbe_global.h"
#include "GameQControls.h"
#include "ImageProvider.h"
#include "MainWindow.h"

#include <cstdio>
#include <cassert>

#include <QMenu>

/// singleton-like pointer: there's only one.
static GameQControls* theGQCPtr = nullptr;


GameQControls::GameQControls(QQuickItem *parent)
    : QQuickItem(parent)
    , theGameButtonGroup(this)
    , theForwardAction(nullptr)
{
    const QSize myIconSize(16, 16);
    theForwardIcon = ImageProvider::getQIcon("ActionFastForward", myIconSize);
    thePauseIcon  = ImageProvider::getQIcon("ActionMenuPause", myIconSize);
    thePlayIcon   = ImageProvider::getQIcon("ActionMenuPlay", myIconSize);
    theResetIcon  = ImageProvider::getQIcon("ActionUndo", myIconSize);

    updateIcon(0);
    assert(nullptr == theGQCPtr);
    theGQCPtr = this;
}


GameQControls::~GameQControls()
{
}


void GameQControls::setup(QMenu *aMenuPtr, GameStateMachine* aGameStateMachinePtr)
{
    // add actions and their quick keys
    theForwardAction = new QAction(theForwardIcon, tr("&Forward"), nullptr);
    //: translators: 'f' is for (fast) forward
    theForwardAction->setShortcut(QKeySequence(tr("f")));
    theForwardAction->setCheckable(true);

    // add actions and their quick keys
    thePauseAction = new QAction(thePauseIcon, tr("P&ause"), nullptr);
    thePauseAction->setCheckable(true);

    // add actions and their quick keys
    thePlayAction = new QAction(thePlayIcon, tr("&Play"), nullptr);
    thePlayAction->setCheckable(true);

    // add actions and their quick keys
    theResetAction = new QAction(theResetIcon, tr("&Reset"), nullptr);
    //: translators: 'r' is for reset
    theResetAction->setShortcut(QKeySequence(tr("r")));

    //: translators: really-fast-forward is only available as a key shortcut
    //: it should be shift-<normal fast-forward>...
    QKeySequence myRealFastKey(tr("Shift+f"));
    theRealFastAction = new QAction(nullptr);
    theRealFastAction->setShortcut(myRealFastKey);
//TODO    this->addAction(theRealFastAction);

    //: translators: slow is only available as a key shortcut
    //: it should be shift-S...
    QKeySequence mySlowKey(tr("Shift+s"));
    theSlowAction = new QAction(nullptr);
    theSlowAction->setShortcut(mySlowKey);
//TODO    this->addAction(theSlowAction);

    // do not add Slow and RealFast - they're keyboard shortcuts only
    aMenuPtr->addAction(thePauseAction);
    aMenuPtr->addAction(thePlayAction);
    aMenuPtr->addAction(theResetAction);
    aMenuPtr->addAction(theForwardAction);

    // add everything to the QActionGroup to make them mutually exclusive
    theGameButtonGroup.addAction(theForwardAction);
    theGameButtonGroup.addAction(thePauseAction);
    theGameButtonGroup.addAction(thePlayAction);
    theGameButtonGroup.addAction(theRealFastAction);
    //theGameButtonGroup.addAction(theResetAction); reset is not mutually exclusive
    theGameButtonGroup.addAction(theSlowAction);
    theGameButtonGroup.setExclusive(true);

    // hook the triggered() signals to this class' signals
    connect (theForwardAction, SIGNAL(triggered()), this,  SIGNAL(signal_Forward_triggered()));
    connect (thePauseAction, SIGNAL(triggered()), this,    SIGNAL(signal_Pause_triggered()));
    connect (thePlayAction, SIGNAL(triggered()), this,     SIGNAL(signal_Play_triggered()));
    connect (theRealFastAction, SIGNAL(triggered()), this, SIGNAL(signal_RealFast_triggered()));
    connect (theResetAction, SIGNAL(triggered()), this,    SIGNAL(signal_Reset_triggered()));
    connect (theSlowAction, SIGNAL(triggered()), this,     SIGNAL(signal_Slow_triggered()));

    connect (aGameStateMachinePtr, SIGNAL(signal_State_Changed(GameStateMachine::States)),
             this, SLOT(slot_updateIcon(GameStateMachine::States)));

    connect (this, SIGNAL(signal_Forward_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_Forward_triggered()));
    connect (this, SIGNAL(signal_Pause_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_Pause_triggered()));
    connect (this, SIGNAL(signal_Play_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_Play_triggered()));
    connect (this, SIGNAL(signal_RealFast_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_RealFast_triggered()));
    connect (this, SIGNAL(signal_Reset_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_Reset_triggered()));
    connect (this, SIGNAL(signal_Slow_triggered()),
             aGameStateMachinePtr, SIGNAL(signal_Slow_triggered()));

}


GameQControls *GameQControls::me()
{
    assert(nullptr != theGQCPtr);
    return theGQCPtr;
}


void GameQControls::slot_updateIcon(GameStateMachine::States aStatus)
{
    DEBUG1ENTRY;
    Q_ASSERT(aStatus >= GameStateMachine::FailedStatus);
    Q_ASSERT(aStatus <= GameStateMachine::WonStatus);
    updateIcon(aStatus);

    QKeySequence mySpaceKey(tr("Space", "key for start/pause the simulation"));
    QKeySequence myEmptyKey("");
    switch (aStatus) {
    case GameStateMachine::FailedStatus:
    case GameStateMachine::ProblemStatus:
    case GameStateMachine::WonStatus:
        // disable the game controls incl. reset
        theForwardAction->setEnabled(false);
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setShortcut(myEmptyKey);
        thePlayAction->setEnabled(false);
        theResetAction->setEnabled(true);
        theGameButtonGroup.setEnabled(false);
        setProperty("buttonForwardEnabled",false);
        setProperty("buttonPauseEnabled",  false);
        setProperty("buttonPlayEnabled",   false);
        setProperty("buttonResetEnabled",  true);
        break;
    case GameStateMachine::StoppedStatus:
        // enable the game controls incl. reset
        theForwardAction->setEnabled(true);
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setShortcut(mySpaceKey);
        thePlayAction->setEnabled(true);
        theResetAction->setEnabled(false);
        theGameButtonGroup.setEnabled(true);
        setProperty("buttonForwardEnabled",true);
        setProperty("buttonPauseEnabled",  false);
        setProperty("buttonPlayEnabled",   true);
        setProperty("buttonResetEnabled",  false);
        break;
    case GameStateMachine::NormalStatus:
        // don't allow reset while running (even though the state machine can handle it)
        theForwardAction->setEnabled(true);
        thePauseAction->setEnabled(true);
        thePauseAction->setShortcut(mySpaceKey);
        thePlayAction->setEnabled(false);
        thePlayAction->setShortcut(myEmptyKey);
        theResetAction->setEnabled(true);
        setProperty("buttonForwardEnabled",true);
        setProperty("buttonPauseEnabled",  true);
        setProperty("buttonPlayEnabled",   false);
        setProperty("buttonResetEnabled",  true);
        break;
    case GameStateMachine::ForwardStatus:
    case GameStateMachine::RealFastStatus:
    case GameStateMachine::SlowStatus:
        // don't allow reset while running (even though the state machine can handle it)
        theForwardAction->setEnabled(false);
        thePauseAction->setEnabled(true);
        thePauseAction->setShortcut(mySpaceKey);
        thePlayAction->setEnabled(true);
        thePlayAction->setShortcut(myEmptyKey);
        theResetAction->setEnabled(true);
        setProperty("buttonForwardEnabled",true);
        setProperty("buttonPauseEnabled",  true);
        setProperty("buttonPlayEnabled",   true);
        setProperty("buttonResetEnabled",  true);
        break;
    case GameStateMachine::PausedStatus:
        // enable the reset - only makes sense in this state :-)
        theForwardAction->setEnabled(true);
        thePauseAction->setEnabled(false);
        thePauseAction->setShortcut(myEmptyKey);
        thePlayAction->setEnabled(true);
        thePlayAction->setShortcut(mySpaceKey);
        theResetAction->setEnabled(true);
        setProperty("buttonForwardEnabled",true);
        setProperty("buttonPauseEnabled",  false);
        setProperty("buttonPlayEnabled",   true);
        setProperty("buttonResetEnabled",  true);
        break;
    }
}

void GameQControls::button_forward_clicked()
{
    theForwardAction->trigger();
}

void GameQControls::button_pause_clicked()
{
    thePauseAction->trigger();
}

void GameQControls::button_play_clicked()
{
    thePlayAction->trigger();
}

void GameQControls::button_reset_clicked()
{
    theResetAction->trigger();
}

void GameQControls::updateIcon(int aNumber)
{
    // No need to call the javascript setIcon() if it doesn't yet exist.
    // While we cannot reliably detect its existence, we know that our actions
    // will be valid at the time. So let's check for that.
    if(!theForwardAction)
        return;
    QMetaObject::invokeMethod(this, "setIcon", Q_ARG(QVariant, aNumber));
}
