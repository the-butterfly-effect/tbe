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
#include "PieMenu.h"
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
static const qreal CENTER_RADIUS = 40.0;

static const qreal SMALL_ICON_HALFWIDTH = 0.5*0.667*CENTER_RADIUS;
static const qreal LARGE_ICON_HALFWIDTH = 2.0*SMALL_ICON_HALFWIDTH;

/// Multiply CENTER_RADIUS with the below number to get the CENTER_RADIUS
/// in scene coordinates
static qreal theCenterRadiusScale = 1.0;

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
    : QGraphicsSvgItem(aFileName, aParentPtr),
      theActionType(anActionType)
{
	// we *must* have a parent (PieMenu!)
	Q_ASSERT(aParentPtr!=NULL);

	// we want to scale ourselfs so we are 2/3 of myRadius as width
	// (or height - square!) that allows the centerpiece to be twice
	// as big and everything just touches :-)
	qreal myCurWidth = boundingRect().width();
	qreal mySmallWidth = SMALL_ICON_HALFWIDTH;
	qreal mySmallScale = 2*mySmallWidth/myCurWidth;
	qreal myLargeWidth = LARGE_ICON_HALFWIDTH;
	qreal myLargeScale = 2*myLargeWidth/myCurWidth;
	theCenterRadiusScale = mySmallWidth/myCurWidth;
	QPointF myOuterPos(CENTER_RADIUS*cos(anActionType*45.0/180.0*PI) - mySmallWidth,
					   -CENTER_RADIUS*sin(anActionType*45.0/180.0*PI) + mySmallWidth);
	QPointF mySInnerPos(-mySmallWidth, 0);
	QPointF myLInnerPos(-myLargeWidth, 0);

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

	QPropertyAnimation* mySizAnim = new QPropertyAnimation(this, "scale");
	mySizAnim->setDuration(500);
	mySizAnim->setEasingCurve(QEasingCurve::InOutBack);

	// where to go from start...
	QAbstractTransition* myTransPtr;
	if (anActionType != ACTION_MOVE)
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myOuterState);
	else
		myTransPtr=myStartState->addTransition(aParentPtr, SIGNAL(moveToPositions()), myInnerState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);

	// and the other state transitions
	if (isEnabled==true)
	{
		myTransPtr=myOuterState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myInnerState);
		connect(this, SIGNAL(clicked(ActionIcon*)), aParentPtr, SLOT(iconClicked(ActionIcon*)));
	}
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(clicked(ActionIcon*)), myActivState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);
	myTransPtr=myInnerState->addTransition(this, SIGNAL(moveBack()), myOuterState);
	myTransPtr->addAnimation(myPosAnim);
	myTransPtr->addAnimation(mySizAnim);


	// set the positions for the various states
	myStartState->assignProperty(this, "pos", mySInnerPos);
	myStartState->assignProperty(this, "scale", mySmallScale);

	myOuterState->assignProperty(this, "pos", myOuterPos);
	myOuterState->assignProperty(this, "scale", mySmallScale);

	myInnerState->assignProperty(this, "pos", myLInnerPos);
	myInnerState->assignProperty(this, "scale", myLargeScale);

	theIconStateMachine.start();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


PieMenu::PieMenu(ViewObject* aParentPtr)
	: QGraphicsWidget(aParentPtr), theVOPtr(aParentPtr)
{
	theAOPtr = aParentPtr->getAbstractObjectPtr();

    setFlags(QGraphicsItem::ItemIgnoresTransformations |
                 QGraphicsItem::ItemIgnoresParentOpacity |
                 QGraphicsItem::ItemDoesntPropagateOpacityToChildren |
                 QGraphicsItem::ItemHasNoContents );
}

void PieMenu::iconClicked(ActionIcon* anIconPtr)
{
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
								"images/ActionResize.svg", myBool, this);

	myBool = (theAOPtr->isRotatable()) || theIsLevelEditor;
	new ActionIcon(ActionIcon::ACTION_ROTATE,
								"images/ActionRotate.svg", myBool, this);

	myBool = (theAOPtr->isMovable());
	ActionIcon* myMoveIcon = new ActionIcon(ActionIcon::ACTION_MOVE,
								"images/ActionMove.svg", myBool, this);

	new ActionIcon(ActionIcon::ACTION_DELETE,
				   "images/ActionDelete.svg", myBool, this);


	new ActionIcon(ActionIcon::ACTION_SETPHONE,
				   "images/ActionSetNumber.svg",
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

		// Problem: we have a position the mouse clicked in Scene coordinates.
		// We need to move half a radius up in order to display the center
		// icon in the middle of the mouse click.
		// Secondly, we need to move the pie menu in case otherwise icons
		// would fall outside of the view.

		qreal PIE_RADIUS = theCenterRadiusScale * (SMALL_ICON_HALFWIDTH+CENTER_RADIUS);

		if (aPositionInSceneCoord.x() < theViewRect.left()+PIE_RADIUS)
			aPositionInSceneCoord.setX(theViewRect.left()+PIE_RADIUS);
		if (aPositionInSceneCoord.x()> theViewRect.right()-PIE_RADIUS)
			aPositionInSceneCoord.setX(theViewRect.right()-PIE_RADIUS);

		if (aPositionInSceneCoord.y()<theViewRect.top()+PIE_RADIUS)
			aPositionInSceneCoord.setY(theViewRect.top()+PIE_RADIUS);
		if (aPositionInSceneCoord.y()>theViewRect.bottom()-PIE_RADIUS)
			aPositionInSceneCoord.setY(theViewRect.bottom()-PIE_RADIUS);
		aPositionInSceneCoord.setY(aPositionInSceneCoord.y()- theCenterRadiusScale*LARGE_ICON_HALFWIDTH);

		me()->theCurrentPieMenuPtr->setPos(
					me()->theCurrentPieMenuPtr->mapFromScene(aPositionInSceneCoord));

	}
	else
		me()->theCurrentPieMenuPtr = NULL;
}


void PieMenuSingleton::setViewInSceneCoords(const QPolygonF& aViewRect)
{
	theViewRect = aViewRect.boundingRect();
}
