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

#include <QState>
#include <QTimer>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include "PieMenu.h"
#include "ViewObject.h"

#include <cmath>
#include "tbe_global.h"



void NamedState::onEntry ( QEvent * event )
{
	DEBUG4("SimulationControls-SimState %s onEntry!\n", ASCII(theName));
	QState::onEntry(event);
}




ActionIcon::ActionIcon(ActionType anActionType,
					   const QString & aFileName,
					   bool isEnabled,
					   QGraphicsWidget* aParentPtr)
	: QGraphicsSvgItem(aFileName, aParentPtr)
{
	// we *must* have a parent (PieMenu!)
	Q_ASSERT(aParentPtr!=NULL);

	// we want to set our coords in parent's coord system
	const qreal theRadius = 100.0;
	QPointF myOuterPos(theRadius*cos(anActionType*45.0/180.0*PI), -theRadius*sin(anActionType*45.0/180.0*PI));

	// TODO/FIXME: still have to change the origin of the transformations

	// we want to scale ourselfs so we are 2/3 of theRadius as width
	// (or height - square!) that allows the centerpiece to be twice
	// as big and everything just touches :-)
	qreal myCurwidth = boundingRect().width();
	qreal myScale = (0.667*theRadius)/myCurwidth;
	scale (myScale,myScale);

	QState* myStartState = new NamedState(&theIconStateMachine, "Start "+aFileName);
	QState* myOuterState = new NamedState(&theIconStateMachine, "Outer "+aFileName);
	QState* myInnerState = new NamedState(&theIconStateMachine, "Inner "+aFileName);
	QState* myActivState = new NamedState(&theIconStateMachine, "Activ "+aFileName);

	theIconStateMachine.setInitialState(myStartState);

	// setup the transition animation
	QPropertyAnimation* myAnim = new QPropertyAnimation(this, "pos");
	myAnim->setDuration(500);
	myAnim->setEasingCurve(QEasingCurve::InOutBack);
	QAbstractTransition* myTransPtr;

	// where to go from start...
	if (anActionType != ACTION_MOVE)
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myOuterState);
	else
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myInnerState);
	myTransPtr->addAnimation(myAnim);

	// and the other state transitions
	myTransPtr=myOuterState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myInnerState);
	myTransPtr->addAnimation(myAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myActivState);
	myTransPtr->addAnimation(myAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(moveBack()), myOuterState);
	myTransPtr->addAnimation(myAnim);

	connect(this, SIGNAL(clicked(ActionIcon*)), aParentPtr, SLOT(iconClicked(ActionIcon*)));

	// set the positions for the various states
	myStartState->assignProperty(this, "pos", QPoint(0,0));
	myOuterState->assignProperty(this, "pos", myOuterPos);
	myInnerState->assignProperty(this, "pos", QPoint(0,0));

	theIconStateMachine.start();
}


PieMenu::PieMenu(ViewObject* aParentPtr)
	: QGraphicsWidget(aParentPtr)
{
	// nothing to do?
}

void PieMenu::iconClicked(ActionIcon* anIconPtr)
{
	emit theCurrentInnerIconPtr->moveBack();
	theCurrentInnerIconPtr = anIconPtr;
}

void PieMenu::setup()
{
	ActionIcon* myHRezIcon = new ActionIcon(ActionIcon::ACTION_HRESIZE,
											"../images/ActionResizeHori.svg", true, this);
	ActionIcon* myRotIcon  = new ActionIcon(ActionIcon::ACTION_ROTATE,
											"../images/ActionRotate.svg", false, this);
	ActionIcon* myMoveIcon = new ActionIcon(ActionIcon::ACTION_MOVE,
											"../images/ActionMove.svg", true, this);
	theCurrentInnerIconPtr = myMoveIcon;

	DEBUG1("start\n");
	QTimer::singleShot(1, this, SLOT(startMove()));
	DEBUG1("final\n");
}
