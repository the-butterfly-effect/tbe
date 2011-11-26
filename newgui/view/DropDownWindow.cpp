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

#include "DropDownWindow.h"
#include <QGraphicsView>

#include "ViewObject.h"
#include "AbstractObject.h"

DropDownWindow::DropDownWindow(QGraphicsWidget* parent) :
	QGraphicsWidget(parent)
{
}

void DropDownWindow::setup(QMenuBar* aMenuBarPtr)
{
	setAutoFillBackground (true);

	// take all palette options from menu bar, so we match
	// except for the background of the window :-)
	QPalette myPal = aMenuBarPtr->palette();
	myPal.setColor(QPalette::Window, Qt::darkRed);
	setPalette(myPal);

	{
	theScenePtr = new QGraphicsScene(0, -100, 100, 100);
	theScenePtr->setBackgroundBrush(Qt::white);
	theScenePtr->addRect(0,0,100,-100);

#if 0
	AbstractObject* theAOPtr = new AbstractObject();
	theAOPtr->setTheHeight(45);
	theAOPtr->setTheWidth(45);
	theAOPtr->setOrigCenter(Position(30,30, 0));
	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr);
#endif
	}


	theToolBoxView = scene()->addWidget(new QGraphicsView(theScenePtr));
	pushButton = scene()->addWidget(new QPushButton("test"));

	theLayoutPtr = new QGraphicsLinearLayout;
	theLayoutPtr->addItem(theToolBoxView);
	theLayoutPtr->addItem(pushButton);
	setLayout(theLayoutPtr);
	setZValue(1000);


	// and the animation mumbo-jumbo
	//
	QState* myUpStatePtr = new QState(&theStateMachine);
	myUpStatePtr->assignProperty(this, "geometry", QRectF(QPointF(80, -380), QSizeF(0, 40)));
	QState* myDownStatePtr = new QState(&theStateMachine);
	myDownStatePtr->assignProperty(this, "geometry", QRectF(QPointF(80,-200), QSizeF(250,170)));
	theStateMachine.setInitialState(myUpStatePtr);

	// setup a custom animation
	QPropertyAnimation* myAnimationPtr = new QPropertyAnimation(this, "geometry");
	myAnimationPtr->setDuration(1000);
	myAnimationPtr->setEasingCurve(QEasingCurve::OutBounce);

	// setup a custom animation
	QPropertyAnimation* myUpAnimationPtr = new QPropertyAnimation(this, "geometry");
	myUpAnimationPtr->setDuration(1000);
	myUpAnimationPtr->setEasingCurve(QEasingCurve::OutQuad);

	// and trigger the state changes with the above animation
	QAction* myDownActionPtr = aMenuBarPtr->addAction(tr("&Down"));
	QAbstractTransition* myDownTransitionPtr = myUpStatePtr->addTransition(myDownActionPtr, SIGNAL(triggered()), myDownStatePtr);
	myDownTransitionPtr->addAnimation(myAnimationPtr);

	QAction* myUpActionPtr = aMenuBarPtr->addAction(tr("&Up"));
	QAbstractTransition *myUpTransitionPtr = myDownStatePtr->addTransition(myUpActionPtr, SIGNAL(triggered()), myUpStatePtr);
	myUpTransitionPtr->addAnimation(myUpAnimationPtr);

	// finally, let's get this baby rolling!!!
	theStateMachine.start();
}
