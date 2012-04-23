/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#include <QtCore/QState>
#include <QtCore/QTimer>
#include <QtCore/QSignalTransition>
#include <QtCore/QPropertyAnimation>

#include "AbstractObject.h"
#include "ImageCache.h"
#include "PieMenu.h"
#include "resizinggraphicsview.h"
#include "TriggerExplosion.h"   // for DetonatorBox*
#include "UndoSingleton.h"
#include "ViewObject.h"

#include <cmath>
#include "tbe_global.h"

// this source file contains 4 classes:
// * NamedState
// * ActionItem
// * PieMenu
// * PieMenuSingleton


/// Distance from the pie menu center to the center of the outside icons.
/// Note: this is in ActionIcon units
static const qreal CENTER_RADIUS = 20.0;
static const qreal OUTER_DISTANCE = 2*CENTER_RADIUS*1.36;
static const qreal TOTAL_RADIUS = OUTER_DISTANCE + 0.5 * CENTER_RADIUS;

static qreal theActionIconScaling = 1.0;

void NamedState::onEntry ( QEvent * event )
{
//	DEBUG4("SimulationControls-SimState %s onEntry!\n", ASCII(theName));
    QState::onEntry(event);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


ActionIcon::ActionIcon(ActionType anActionType,
                       const QString & aFileName,
                       bool isEnabled,
                       QGraphicsWidget* aParentPtr)
    : QGraphicsPixmapItem(aFileName, aParentPtr),
      theActionType(anActionType)
{
	// we *must* have a parent (PieMenu!)
	Q_ASSERT(aParentPtr!=NULL);

	QPixmap myTempPixmap;
	ImageCache::getPixmap(aFileName, QSize(2*CENTER_RADIUS,2*CENTER_RADIUS), &myTempPixmap);
	setPixmap(myTempPixmap);

	QPointF myOuterPos(OUTER_DISTANCE*cos(anActionType*45.0/180.0*PI) -CENTER_RADIUS,
					   -OUTER_DISTANCE*sin(anActionType*45.0/180.0*PI) - CENTER_RADIUS);
	QPointF mySInnerPos(-CENTER_RADIUS, -CENTER_RADIUS);
	QPointF myLInnerPos(-CENTER_RADIUS,-CENTER_RADIUS);

	// Quite a convoluted way to figure out the scaling transformation that the QGraphicsWidget parent of ours does
	// TODO/FIXME: I have no clue where that 0.5 comes from, but it appears to work reasonably well...
	theActionIconScaling = 0.5*mapFromScene(QRectF(0,0,CENTER_RADIUS,CENTER_RADIUS)).boundingRect().width() / CENTER_RADIUS;

	if (isEnabled==false)
	{
		QGraphicsColorizeEffect* myEffect = new QGraphicsColorizeEffect();
		myEffect->setColor(Qt::darkBlue);
		setGraphicsEffect(myEffect);
	}

	// no need to keep pointers around - will be cleaned up by the state
	// machine upon destruction.
	QState* myStartState = new NamedState(&theIconStateMachine, "Start "+aFileName);
	QState* myOuterState = new NamedState(&theIconStateMachine, "Outer "+aFileName);
	QState* myInnerState = new NamedState(&theIconStateMachine, "Inner "+aFileName);
	QState* myActivState = new NamedState(&theIconStateMachine, "Activ "+aFileName);

	theIconStateMachine.setInitialState(myStartState);

	// setup the transition animation
	QPropertyAnimation* myPosAnim = new QPropertyAnimation(this, "pos");
	myPosAnim->setDuration(500);
	myPosAnim->setEasingCurve(QEasingCurve::InOutBack);

	// where to go from start...
	QAbstractTransition* myTransPtr;
	if (anActionType != ACTION_MOVE)
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myOuterState);
	else
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myInnerState);
	myTransPtr->addAnimation(myPosAnim);

	// and the other state transitions
	if (isEnabled==true)
	{
		myTransPtr=myOuterState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myInnerState);
		connect(this, SIGNAL(clicked(ActionIcon*)), aParentPtr, SLOT(iconClicked(ActionIcon*)));
	}
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myActivState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(moveBack()), myOuterState);
	myTransPtr->addAnimation(myPosAnim);


	// set the positions for the various states
	myStartState->assignProperty(this, "pos", mySInnerPos);

	myOuterState->assignProperty(this, "pos", myOuterPos);

	myInnerState->assignProperty(this, "pos", myLInnerPos);

	theIconStateMachine.start();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


PieMenu::PieMenu(ViewObject* aParentPtr)
	: QGraphicsWidget(aParentPtr), theVOPtr(aParentPtr), wasIconClicked(false)
{
	theAOPtr = aParentPtr->getAbstractObjectPtr();

    setFlags(QGraphicsItem::ItemIgnoresTransformations |
                 QGraphicsItem::ItemIgnoresParentOpacity |
                 QGraphicsItem::ItemDoesntPropagateOpacityToChildren |
                 QGraphicsItem::ItemHasNoContents );
}

void PieMenu::iconClicked(ActionIcon* anIconPtr)
{
	wasIconClicked = true;
	emit theCurrentInnerIconPtr->moveBack();

	if (theCurrentInnerIconPtr == anIconPtr)
	{
		UndoSingleton::createUndoCommand(theVOPtr, anIconPtr->getActionType());
		PieMenuSingleton::clearPieMenu();
	}
	else
		theCurrentInnerIconPtr = anIconPtr;
}

void PieMenu::setup()
{

	bool myBool = (theAOPtr->isResizable()&
				   (AbstractObject::HORIZONTALRESIZE|AbstractObject::VERTICALRESIZE))
				   || theIsLevelEditor;
	new ActionIcon(ActionIcon::ACTION_RESIZE,
								"ActionResize", myBool, this);

	myBool = (theAOPtr->isRotatable()) || theIsLevelEditor;
	new ActionIcon(ActionIcon::ACTION_ROTATE,
								"ActionRotate", myBool, this);

	myBool = (theAOPtr->isMovable());
	ActionIcon* myMoveIcon = new ActionIcon(ActionIcon::ACTION_MOVE,
								"ActionMove", myBool, this);

	new ActionIcon(ActionIcon::ACTION_DELETE,
				   "ActionDelete", myBool, this);


	new ActionIcon(ActionIcon::ACTION_SETPHONE,
				   "ActionSetNumber",
				   dynamic_cast<DetonatorBox*>(theAOPtr)!=NULL, this);
	theCurrentInnerIconPtr = myMoveIcon;

	QTimer::singleShot(1, this, SLOT(startMove()));
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static PieMenuSingleton* thePMSingletonPtr = NULL;
static QRectF theViewRect;

PieMenuSingleton::PieMenuSingleton(void)
	: theCurrentPieMenuPtr(NULL)
{
	// nothing to do
}


ViewObject* PieMenuSingleton::getPieMenuParent(void)
{
	if (me()->theCurrentPieMenuPtr==NULL)
		return NULL;
		return me()->theCurrentPieMenuPtr->theVOPtr;
}


PieMenuSingleton* PieMenuSingleton::me(void)
{
	if (thePMSingletonPtr==NULL)
		thePMSingletonPtr = new PieMenuSingleton();
	return thePMSingletonPtr;
}


void PieMenuSingleton::addPieMenuToViewObject(ViewObject* aViewObjectPtr,
											  QPointF aPositionInSceneCoord )
{
	DEBUG3("PieMenuSingleton::setPieMenuParent(%p)\n", aViewObjectPtr);
	// one can always call delete on a nullpointer
	delete me()->theCurrentPieMenuPtr;

	if (aViewObjectPtr!=NULL)
	{
		me()->theCurrentPieMenuPtr = new PieMenu(aViewObjectPtr);
		me()->theCurrentPieMenuPtr->setup();

		// We need to move the pie menu if otherwise icons
		// would fall outside of the view.
		qreal PIE_RADIUS = TOTAL_RADIUS/theActionIconScaling;

		if (aPositionInSceneCoord.x() < theViewRect.left()+PIE_RADIUS)
			aPositionInSceneCoord.setX(theViewRect.left()+PIE_RADIUS);
		if (aPositionInSceneCoord.x()> theViewRect.right()-PIE_RADIUS)
			aPositionInSceneCoord.setX(theViewRect.right()-PIE_RADIUS);

		if (aPositionInSceneCoord.y()<theViewRect.top()+PIE_RADIUS)
			aPositionInSceneCoord.setY(theViewRect.top()+PIE_RADIUS);
		if (aPositionInSceneCoord.y()>theViewRect.bottom()-PIE_RADIUS)
			aPositionInSceneCoord.setY(theViewRect.bottom()-PIE_RADIUS);

		me()->theCurrentPieMenuPtr->setPos(me()->theCurrentPieMenuPtr->mapToParent(me()->theCurrentPieMenuPtr->mapFromScene(aPositionInSceneCoord)));
	}
	else
		me()->theCurrentPieMenuPtr = NULL;
}


void PieMenuSingleton::setViewInSceneCoords(const QPolygonF& aViewRect)
{
	theViewRect = aViewRect.boundingRect();
}


void PieMenuSingleton::startClickCheck()
{
	if (me()->theCurrentPieMenuPtr!=NULL)
		me()->theCurrentPieMenuPtr->wasIconClicked = false;
}

void PieMenuSingleton::endClickCheck()
{
	if (me()->theCurrentPieMenuPtr!=NULL)
	{
		if (me()->theCurrentPieMenuPtr->wasIconClicked == false)
			clearPieMenu();
	}
}

