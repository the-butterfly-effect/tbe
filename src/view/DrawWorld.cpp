/* The Butterfly Effect 
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#include "tbe_global.h"
#include "DrawWorld.h"
#include "World.h"
#include "MainWindow.h"
#include "DrawObject.h"
#include "Popup.h"
#include "toolbox.h"
#include "StartStopWatch.h"
#include "UndoInsertCommand.h"
#include "Anchors.h"

#include <QPainter>
#include <QStyleOption>
#include <QDropEvent>

/** the Dot class is a helper - it's a true QGraphicsItem, it's
 *  just invisible (nothing is drawn) and very very small
 *  However - it does affect to what size the Scene streches.
 */
class Dot : public QGraphicsItem
{
public:
	Dot(qreal x, qreal y)
	{
		setPos(x,y);
		setFlags(0);
	}

	/// overriden from QGraphicsItem
    virtual QRectF boundingRect() const
    {    return QRectF(0,0, 0.001, 0.001); }
	
	/// overriden from QGraphicsItem
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget *)
	{ /* nothing! */ }
};


const int DrawWorld::theMaxNumberOfGraphicsListElements = 125;


// Constructors/Destructors
//  

DrawWorld::DrawWorld (MainWindow* aMainWindowPtr, World* aWorldPtr)
	: theMainWindowPtr(aMainWindowPtr), theWorldPtr(aWorldPtr), theSimStateMachine(NULL)
{
	initAttributes();
	
	aMainWindowPtr->setScene(this, theWorldPtr->getName());
	
	connect(&theTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));
	connect(&theFramerateTimer, SIGNAL(timeout()), this, SLOT(on_framerateTimerTick()));

	drawOutlineAndBackground();

	// announce my UndoStack to all future DrawObjects:
	DrawObject::setUndoStackPtr(&theUndoStack);
	setDrawDebug();
	setAcceptDrops(true);
}

DrawWorld::~DrawWorld ( ) 
{
	DEBUG5("DrawWorld::~DrawWorld ( )\n");
	QObject::disconnect(theSimStateMachine, SIGNAL(startSim()), this, SLOT(startTimer()));
	QObject::disconnect(theSimStateMachine, SIGNAL(stopSim()),  this, SLOT(stopTimer()));
	QObject::disconnect(theSimStateMachine, SIGNAL(resetSim()), this, SLOT(resetWorld()));
	QObject::disconnect(theSimStateMachine, SIGNAL(goFast()),   this, SLOT(goFast()));
	QObject::disconnect(theSimStateMachine, SIGNAL(goSlow()),   this, SLOT(goSlow()));
	delete theSimStateMachine;
}

//  
// Methods
//  


// Accessor methods
//
qreal DrawWorld::getHeight()
{
	return theWorldPtr->getTheWorldHeight(); 
}

QGraphicsScene* DrawWorld::getStartStopWatchPtr()
{
	if (theSimStateMachine)
		theSimStateMachine->displayTooltip(true);
	return theSimStateMachine;
}

qreal DrawWorld::getWidth()
{
	return theWorldPtr->getTheWorldWidth();
}


// Other methods
//  


void DrawWorld::deleteOutline(void)
{
	while (theBasicDrawWorldItems.count()>0)
	{
		QGraphicsItem* myItem = theBasicDrawWorldItems.last();
		theBasicDrawWorldItems.pop_back();
		delete myItem;
	}
	drawDots();
}

void DrawWorld::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	DEBUG4("DrawWorld::dragEnterEvent ()\n");
	if (event->mimeData()->hasFormat(TBItem::ToolboxMimeType))
	{
		QByteArray pieceData = event->mimeData()->data(TBItem::ToolboxMimeType);
		QDataStream stream(&pieceData, QIODevice::ReadOnly);
		QString myObjectName;
		stream >> myObjectName;

		DEBUG4("  the object is: '%s' @ %f,%f\n", ASCII(myObjectName),
			   event->scenePos().x(), event->scenePos().y());

		// now we know the ID of the object, let's retrieve a copy from the Toolbox
		if (event->source() != NULL)
		{
			BaseObject* myObjectPtr = dynamic_cast<ToolBox*>(event->source())->getMeACopyOf(myObjectName);
			if (myObjectPtr != NULL)
			{
				myObjectPtr->setOrigCenter(Position(event->scenePos()));
				myObjectPtr->createPhysicsObject();
				theWorldPtr->addObject(myObjectPtr);

				theInsertUndoPtr = new UndoInsertCommand(myObjectPtr);

				myObjectPtr->parseProperties();

				event->accept();
				return;
			}
		}
	}
	event->ignore();
}

void DrawWorld::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	DEBUG4("DrawWorld::dragLeaveEvent()\n");
	if (theInsertUndoPtr!=NULL)
	{
		theInsertUndoPtr->cancel();
		delete theInsertUndoPtr;
		theInsertUndoPtr = NULL;
	}
	event->accept();
}

void DrawWorld::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	DEBUG4("DrawWorld::dragMoveEvent ()\n");
	if (theInsertUndoPtr==NULL)
	{
		DEBUG3("DrawWorld::dragMoveEvent IGNORED()\n");
		event->ignore();
		return;
	}

	// do not use the QPointF constructor because that one will negate y
	Position myPosition(event->scenePos().x(), event->scenePos().y());
	theInsertUndoPtr->setNewPosition(myPosition);
	event->accept();
}

void DrawWorld::drawDots(void)
{
	// make sure to never shrink smaller than the specified size
	QGraphicsItem* myDot = new Dot(getWidth(), -getHeight());
	theBasicDrawWorldItems.push_back(myDot);
	addItem(myDot);
	myDot = new Dot(0,0);
	theBasicDrawWorldItems.push_back(myDot);
	addItem(myDot);
}

void DrawWorld::drawOutlineAndBackground(void)
{
	DEBUG1("void DrawWorld::drawOutlineAndBackground(void)\n");

	deleteOutline();

	if (theIsLevelEditor)
	{
		// draw a box as the outline of the World
		theBasicDrawWorldItems.push_back(addLine(0,0,                     getWidth(),0));
		theBasicDrawWorldItems.push_back(addLine(getWidth(),0,            getWidth(),-getHeight()));
		theBasicDrawWorldItems.push_back(addLine(getWidth(),-getHeight(), 0,-getHeight()));
		theBasicDrawWorldItems.push_back(addLine(0,-getHeight(),          0,0));
	}

	// a dot on top-right to make sure that the Scene
	// is rendered in total, with the bottom-left corner at the bottomleft
	// and without any room for the view to start scrolling during drag & drop
	int myViewWidthPixels=views()[0]->width();
	int myViewHeightPixels=views()[0]->height();
	qreal myViewWidthReal = myViewWidthPixels/ ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
	qreal myViewHeightReal = myViewHeightPixels/ ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
	QGraphicsItem* myDot =                new Dot(myViewWidthReal, -myViewHeightReal);
	theBasicDrawWorldItems.push_back(myDot);
	addItem(myDot);
	drawDots();

	// a blue gradient background
	// TODO/FIXME: We're going to make this flexible later (see ticket:58)
	setBackgroundBrush(Qt::blue);
	QLinearGradient myBackground(0,0, 0,-getHeight());
	myBackground.setColorAt(0, QColor(212,212,255,255));
	myBackground.setColorAt(1, QColor(121,121,235,255));
	setBackgroundBrush(myBackground);
}


void DrawWorld::dropEvent ( QGraphicsSceneDragDropEvent * event)
{
	DEBUG4("void DrawWorld::dropEvent\n");
	if (theInsertUndoPtr!=NULL)
	{
		if (theInsertUndoPtr->checkForValidPositionOrRevert()==false)
		{
			// we have never been in a good position
			theInsertUndoPtr->cancel();
			delete theInsertUndoPtr;
		}
		else
			theUndoStack.push(theInsertUndoPtr);

		theInsertUndoPtr = NULL;
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->ignore();
}

void DrawWorld::goFast()
{
	DEBUG4("void DrawWorld::goFast()\n");
	theSimSpeed /= 4;
}

void DrawWorld::goSlow()
{
	DEBUG4("void DrawWorld::goSlow()\n");
	theSimSpeed *= 4;
}

void DrawWorld::initAttributes ( ) 
{
	DEBUG4("void DrawWorld::initAttributes\n");
	theSimSpeed = 1000.0;
	theInsertUndoPtr = NULL;
	theCongratDeathBoxPtr = NULL;

	isUserInteractionAllowed = true;
	DrawObject::setIsSimRunning(false);

	theSimStateMachine = new StartStopWatch();
	QObject::connect(theSimStateMachine, SIGNAL(startSim()), this, SLOT(startTimer()));
	QObject::connect(theSimStateMachine, SIGNAL(stopSim()),  this, SLOT(stopTimer()));
	QObject::connect(theSimStateMachine, SIGNAL(resetSim()), this, SLOT(resetWorld()));
	QObject::connect(theSimStateMachine, SIGNAL(goFast()),   this, SLOT(goFast()));
	QObject::connect(theSimStateMachine, SIGNAL(goSlow()),   this, SLOT(goSlow()));
}

void DrawWorld::invalidateCaching(void)
{
	QList<QGraphicsItem *> myItemList = items();
	QList<QGraphicsItem *>::iterator i = myItemList.begin();
	while(i!= myItemList.end())
	{
		DrawObject* myDOPtr = dynamic_cast<DrawObject*>(*i);
		if (myDOPtr)
			myDOPtr->setupCache();
		++i;
	}
}

void DrawWorld::makeAllObjectsSelectable()
{
	QList<QGraphicsItem *> myItems = items ();
	foreach(QGraphicsItem* i, myItems)
	{
		DrawObject* myDOPtr = dynamic_cast<DrawObject*>(i);
		if (myDOPtr!=NULL)
			myDOPtr->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	}
}

void DrawWorld::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	if (isUserInteractionAllowed)
	{
		QGraphicsScene::mousePressEvent(mouseEvent);
	}
	else
	{
		if (true == Popup::YesNoQuestion(
				tr("You cannot make changes now. Reset the stopwatch?"),
				views().first()))
		{
			emit theSimStateMachine->clicked_on_reset();
		}
	}
}

void DrawWorld::on_death(void)
{
	// only need to display the graphicsitem once...
	if (theCongratDeathBoxPtr!=NULL)
		return;
	else
		theCongratDeathBoxPtr = new CongratDeathMessage(CongratDeathMessage::DEATH, this, theMainWindowPtr);

	theSimStateMachine->goToState(StartStopWatch::BROKEN);
}


void DrawWorld::on_OneSecondAfterWinning(void)
{
	theSimStateMachine->goToState(StartStopWatch::STOPPED);
	emit levelWon();
}


void DrawWorld::on_timerTick()
{
	while(theSimulationTime < QTime::currentTime())
	{
		theSimulationTime = theSimulationTime.addMSecs(theWorldPtr->simStep() * theSimSpeed);
	}
	advance();
	theFramesPerSecond++;
}

void DrawWorld::on_framerateTimerTick()
{
	theMainWindowPtr->statusBar()->showMessage(tr("Framerate: %1 fps").arg(theFramesPerSecond), 3500);
	theFramesPerSecond = 0;
}


void DrawWorld::on_winning(void)
{
	// only need to display the graphicsitem once...
	if (theCongratDeathBoxPtr!=NULL)
		return;
	else
		theCongratDeathBoxPtr = new CongratDeathMessage(
				CongratDeathMessage::CONGRATS, this, theMainWindowPtr);

	QTimer::singleShot(1500, this, SLOT(on_OneSecondAfterWinning()));
}



void DrawWorld::resetWorld( )
{
	DEBUG5("RESET WORLD\n");
	theWorldPtr->reset();
	// and redraw
	advance();
	isUserInteractionAllowed = true;
	setAcceptDrops(true);
	if (theDrawDebug)
		clearGraphicsList(0);

	if (theCongratDeathBoxPtr!=NULL)
	{
		delete theCongratDeathBoxPtr;
		theCongratDeathBoxPtr=NULL;
	}
	emit theSimStateMachine->clicked_on_reset();
}

void DrawWorld::setAcceptDrops(bool isOn)
{
	QGraphicsView* myView = views()[0];
	DEBUG5("DrawWorld::setAcceptDrops(%d) for view %p\n", isOn, myView);
	if (myView != NULL)
	{
		myView->setAcceptDrops(isOn);
		DEBUG5("new view drop state: %d\n", myView->acceptDrops());
	}
}

void DrawWorld::setDrawDebug()
{
	if (theDrawDebug)
	{
		SetFlags (e_shapeBit | e_prismaticJoint | e_revoluteJoint);
		clearGraphicsList(0);
	}
	else
		ClearFlags (e_shapeBit | e_prismaticJoint | e_revoluteJoint);
}

void DrawWorld::startTimer(void)
{
	DEBUG5("DrawWorld::startTimer(void)\n");

	// TODO: upon reset, the old object gets focus again - thanks, Qt!
	DrawObject myTemp(NULL);
	setFocusItem(&myTemp);
	myTemp.focusRemove(true);
	Anchors::clearEditObjectDialogPtr();

	setAcceptDrops(false);
	isUserInteractionAllowed = false;
	DrawObject::setIsSimRunning(true);


	theTimer.start(1000/25);
	theSimulationTime = QTime::currentTime();

	if (theDisplayFramerate)
	{
		// update framerate every second
		theFramerateTimer.start(1000);
		theFramesPerSecond = 0;
	}
}

void DrawWorld::stopTimer(void)
{
	DEBUG5("DrawWorld::stopTimer(void)\n");
	theFramerateTimer.stop();
	theTimer.stop();
	DrawObject::setIsSimRunning(false);
}





/// Draw a closed polygon provided in CCW order.
void DrawWorld::DrawPolygon(UNUSED_ARG const b2Vec2* vertices, UNUSED_ARG int32 vertexCount, UNUSED_ARG const b2Color& color)
{
	DEBUG5("DrawWorld::DrawPolygon\n");
}
/// Draw a solid closed polygon provided in CCW order.
void DrawWorld::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
								 UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 0.01, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);
	QPolygonF myPoly;
	for (int i=0; i<vertexCount; i++)
	{
		if (vertices[i].x < -0.5 || vertices[i].y < -0.5)
			return;
		myPoly << QPointF(vertices[i].x, -vertices[i].y);
	}
	addDebugDrawToList(addPolygon(myPoly, pen, brush));
}
/// Draw a circle.
void DrawWorld::DrawCircle(UNUSED_ARG const b2Vec2& center,
						   UNUSED_ARG float32 radius,
						   UNUSED_ARG const b2Color& color)
{
	DEBUG5("DrawWorld::DrawCircle\n");
}
/// Draw a solid circle.
void DrawWorld::DrawSolidCircle(const b2Vec2& center, float32 radius,
								UNUSED_ARG const b2Vec2& axis,
								UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::green, 0.01, Qt::SolidLine);
	QBrush brush(Qt::NoBrush);
	addDebugDrawToList(addEllipse(center.x-radius,-center.y-radius, 2.0*radius,2.0*radius, pen, brush));
}
/// Draw a line segment.
void DrawWorld::DrawSegment(UNUSED_ARG const b2Vec2& p1,
							UNUSED_ARG const b2Vec2& p2,
							UNUSED_ARG const b2Color& color)
{
	QPen pen(Qt::cyan, 0.01, Qt::SolidLine);
	addDebugDrawToList(addLine(p1.x,-p1.y, p2.x, -p2.y, pen));
}
/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void DrawWorld::DrawXForm(UNUSED_ARG const b2XForm& xf)
{
	DEBUG5("DrawWorld::DrawXForm\n");
}

void DrawWorld::addDebugDrawToList(QGraphicsItem* anItem)
{
	theGraphicsList.push_back(anItem);
	clearGraphicsList(theMaxNumberOfGraphicsListElements);
}

void DrawWorld::clearGraphicsList(int aCount)
{
	while (theGraphicsList.count() > aCount)
	{
		QGraphicsItem* myItemPtr = theGraphicsList.first();
		theGraphicsList.pop_front();
		delete myItemPtr;
	}
}

DrawWorld::CongratDeathMessage::CongratDeathMessage(
		MessageType aType,
		DrawWorld* aScenePtr,
		MainWindow* aMainWindowPtr)
		: QGraphicsSvgItem(IMAGES_DIRECTORY + "/congrat-death-border.svg"), theScenePtr(aScenePtr)
{
	QString myMessage;
	switch(aType)
	{
		case CONGRATS:
			myMessage = tr("Congratulations");
			break;
		case DEATH:
			myMessage = tr("Death - retry?");
			break;
	}

	// the image is put in scene coordinates
	QRectF myImageBounds = boundingRect();
	QRectF mySceneBounds(0,-aScenePtr->getHeight(),
						 aScenePtr->getWidth(), aScenePtr->getHeight());

	qreal myResize = 0.8 * mySceneBounds.width() / myImageBounds.width() ;
	scale(myResize, myResize);
	setPos(mySceneBounds.center()-mapToScene(boundingRect().center()));
	setZValue(10.0);
	aScenePtr->addItem(this);

	theTextPtr = new QGraphicsSimpleTextItem(myMessage, this);
	QRectF myTextBounds = theTextPtr->boundingRect();
	myResize = 0.9 * boundingRect().width() / myTextBounds.width() ;
	theTextPtr->setBrush(Qt::white);
	theTextPtr->scale(myResize, myResize);
	myTextBounds = theTextPtr->mapToParent( theTextPtr->boundingRect() ).boundingRect();
	theTextPtr->setPos(boundingRect().center() - myTextBounds.center());

	QGraphicsView* myView = aScenePtr->views()[0];
	theButtons[0] = new QPushButton(tr("&Replay"), myView );
	theButtons[0]->show();
	theButtons[1] = new QPushButton(tr("&Choose..."), myView );
	theButtons[1]->show();
	if (aType==CONGRATS)
	{
		theButtons[2] = new QPushButton(tr("&Next>"), myView );
		theButtons[2]->show();
	}
	else
	{
		theButtons[2]=NULL;
	}
	moveButtons();

	connect(theButtons[0], SIGNAL(clicked()), theScenePtr, SLOT(resetWorld()));
	connect(theButtons[1], SIGNAL(clicked()), aMainWindowPtr, SLOT(on_actionOpen_level_activated()));
	if (theButtons[2])
		connect(theButtons[2], SIGNAL(clicked()), aMainWindowPtr, SLOT(slot_next_level()));
}

DrawWorld::CongratDeathMessage::~CongratDeathMessage()
{
	delete theButtons[0];
	delete theButtons[1];
	delete theButtons[2];
}

void DrawWorld::CongratDeathMessage::moveButtons()
{
	QGraphicsView* myView = theScenePtr->views()[0];
	QPointF myButtonLPos = theTextPtr->mapToScene(QPointF(theTextPtr->boundingRect().left(),
								  theTextPtr->boundingRect().bottom()));

	theButtons[0]->move(myView->mapFromScene(myButtonLPos));

	theButtons[1]->move(theButtons[0]->x() + theButtons[0]->width() + 20,
						theButtons[0]->y());

	if (theButtons[2])
		theButtons[2]->move(theButtons[1]->x() + theButtons[1]->width() + 20,
						theButtons[1]->y());
}

void DrawWorld::CongratDeathMessage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget)
{
	QGraphicsSvgItem::paint(painter, option, widget);
	moveButtons();
}
