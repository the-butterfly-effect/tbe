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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include <QState>
#include <QTimer>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include <QApplication>

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
static const qreal CENTER_RADIUS = 24.0;
/// When the small outside and center radius circles touch, the proportion
/// between the two is: (1-sin(22.5))/sin(22.5) = 1.613
static const qreal SMALL_RADIUS = CENTER_RADIUS/1.613;
static const qreal OUTER_DISTANCE = CENTER_RADIUS+SMALL_RADIUS;
static const qreal TOTAL_RADIUS = OUTER_DISTANCE + SMALL_RADIUS;

static qreal theActionIconScaling = 1.0;

void NamedState::onEntry ( QEvent * event )
{
//	DEBUG4("SimulationControls-SimState %s onEntry!", ASCII(theName));
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
	Q_ASSERT(aParentPtr!=nullptr);

	ImageCache::getPixmap(aFileName, QSize(2*CENTER_RADIUS,2*CENTER_RADIUS), &theCenterPixmap);
	ImageCache::getPixmap(aFileName, QSize(2*SMALL_RADIUS,2*SMALL_RADIUS), &theSmallPixmap);

	QPointF myOuterPos(OUTER_DISTANCE*cos(anActionType*45.0/180.0*PI) -SMALL_RADIUS,
					   -OUTER_DISTANCE*sin(anActionType*45.0/180.0*PI) - SMALL_RADIUS);
	QPointF mySInnerPos(-SMALL_RADIUS, -SMALL_RADIUS);
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
	myStartState->assignProperty(this, "pixmap", theSmallPixmap);

	myOuterState->assignProperty(this, "pos", myOuterPos);
	myOuterState->assignProperty(this, "pixmap", theSmallPixmap);

	myInnerState->assignProperty(this, "pos", myLInnerPos);
	myInnerState->assignProperty(this, "pixmap", theCenterPixmap);

	theIconStateMachine.start();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


PieMenu::PieMenu(ViewObjectPtr aParentPtr)
	: QGraphicsWidget(aParentPtr), theVOPtr(aParentPtr), wasIconClicked(false)
{
	theAOPtr = aParentPtr->getAbstractObjectPtr();

    setFlags(QGraphicsItem::ItemIgnoresTransformations |
                 QGraphicsItem::ItemIgnoresParentOpacity |
                 QGraphicsItem::ItemDoesntPropagateOpacityToChildren |
                 QGraphicsItem::ItemHasNoContents );
    theVOsOriginalZValue = theVOPtr->zValue();
    theVOPtr->setZValue(1000);
}

PieMenu::~PieMenu()
{
    if (!theVOPtr.isNull())
        theVOPtr->setZValue(theVOsOriginalZValue);
    theVOPtr.clear();
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
                   || theIsLevelCreator;
	new ActionIcon(ActionIcon::ACTION_RESIZE,
								"ActionResize", myBool, this);

    myBool = (theAOPtr->isRotatable()) || theIsLevelCreator;
	new ActionIcon(ActionIcon::ACTION_ROTATE,
								"ActionRotate", myBool, this);

	myBool = (theAOPtr->isMovable());
	ActionIcon* myMoveIcon = new ActionIcon(ActionIcon::ACTION_MOVE,
								"ActionMove", myBool, this);

	new ActionIcon(ActionIcon::ACTION_DELETE,
				   "ActionDelete", myBool, this);


	new ActionIcon(ActionIcon::ACTION_SETPHONE,
				   "ActionSetNumber",
                   dynamic_cast<DetonatorBox*>(theAOPtr.get())!=nullptr, this);
	theCurrentInnerIconPtr = myMoveIcon;

	QTimer::singleShot(1, this, SLOT(startMove()));
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static PieMenuSingleton* thePMSingletonPtr = nullptr;
static QRectF theViewRect;

PieMenuSingleton::PieMenuSingleton(void)
	: theCurrentPieMenuPtr(nullptr)
{
	// nothing to do
}


ViewObjectPtr PieMenuSingleton::getPieMenuParent(void)
{
	if (me()->theCurrentPieMenuPtr==nullptr)
		return nullptr;
	return me()->theCurrentPieMenuPtr->theVOPtr;
}


PieMenuSingleton* PieMenuSingleton::me(void)
{
	if (thePMSingletonPtr==nullptr)
		thePMSingletonPtr = new PieMenuSingleton();
	return thePMSingletonPtr;
}


void PieMenuSingleton::addPieMenuToViewObject(ViewObjectPtr aViewObjectPtr,
											  QPointF aPositionInSceneCoord )
{
	DEBUG5("PieMenuSingleton::setPieMenuParent(%p)", aViewObjectPtr.data());

	// we call delete on a QPointer<>
	delete me()->theCurrentPieMenuPtr;
	QApplication::processEvents();

	if (nullptr != aViewObjectPtr)
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
}


void PieMenuSingleton::setViewInSceneCoords(const QPolygonF& aViewRect)
{
	theViewRect = aViewRect.boundingRect();
}


void PieMenuSingleton::startClickCheck()
{
	if (me()->theCurrentPieMenuPtr!=nullptr)
		me()->theCurrentPieMenuPtr->wasIconClicked = false;
}

void PieMenuSingleton::endClickCheck()
{
	if (me()->theCurrentPieMenuPtr!=nullptr)
	{
		if (me()->theCurrentPieMenuPtr->wasIconClicked == false)
			clearPieMenu();
	}
}

