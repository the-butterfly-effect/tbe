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
#include "ImageStore.h"

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


const int DrawWorld::theMaxNumberOfGraphicsListElements = 200;


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
		if (isUserInteractionAllowed == false)
		{
			if (true == Popup::YesNoQuestion(
					tr("You cannot make changes now. Reset the stopwatch?"),
					views().first()))
			{
				emit theSimStateMachine->clicked_on_reset();
			}
			event->ignore();
			return;
		}

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
				theWorldPtr->addObject(myObjectPtr);

				theInsertUndoPtr = new UndoInsertCommand(myObjectPtr);

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
	DEBUG5("DrawWorld::dragMoveEvent ()\n");
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

void DrawWorld::drawBGImage( QPainter* p, const QRectF & rect )
{
	QSize myVPSize = views()[0]->size();
	if (myVPSize.width() != theBackgroundPixmap.width() ||
		myVPSize.height() != theBackgroundPixmap.height())
	{
		redrawBGPixmap();
	}

	QBrush myBrush(theBackgroundPixmap);
	float myPixPerMeter = ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
	myBrush.setTransform(QTransform::fromScale(1/myPixPerMeter,1/myPixPerMeter));

	p->save();
	p->setBrush(myBrush);
	p->drawRect(rect);
	p->restore();
}

void DrawWorld::drawBackground ( QPainter* p, const QRectF & rect )
{
	// commented out to save CPU power and improve framerates
	// see https://sourceforge.net/apps/trac/tbe/ticket/278
	drawBGImage(p,rect);

	// TODO: speed up: move this one to redrawBGPixmap()
	QGraphicsScene::drawBackground(p, rect);
}

void DrawWorld::redrawBGPixmap(void)
{
	//  figure out new width and height
	QSize myVPSize = views()[0]->size();

	//	replace existing QPixmap
	theBackgroundPixmap = QPixmap(myVPSize);
	QPainter p(&theBackgroundPixmap);

	// draw the background image - if we have one,
	// then let the QGraphicsScene worry about the gradient...
	if (theWorldPtr->theBackground.theImageName.isEmpty() == false)
	{
		QPixmap* myPtr = ImageRendererStore::getPixmap(theWorldPtr->theBackground.theImageName);
		if (myPtr != NULL)
		{
			p.save();

			// if no repeat: image is drawn to fit within the world
			float myHRepeat = getWidth();
			float myVRepeat = getHeight();
			if (theWorldPtr->theBackground.theImageHRepeat > 0.01)
				myHRepeat = theWorldPtr->theBackground.theImageHRepeat;
			if (theWorldPtr->theBackground.theImageVRepeat > 0.01)
				myVRepeat = theWorldPtr->theBackground.theImageVRepeat;

			// Calculate the scaling. The QPainter transform is set to deal in
			// pixels as coordinates - we're going to scale that as we see fit
			float myPixPerMeter = ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
			float myHScale = myHRepeat * myPixPerMeter / myPtr->width();
			float myVScale = myVRepeat * myPixPerMeter / myPtr->height();
			QTransform myTransform(myHScale,0,0,myVScale,
						0, p.viewport().height()-getHeight()*myPixPerMeter);

			// Now the transform is into action, the new unit is pixels of the
			// image. We want to see *horizontally* repeats outside the scene
			// but *vertically*, we stop at the top of the level
			QRectF myScaledRect(0,0, (getWidth()/myHRepeat+1.0) * myPtr->width(),
								getHeight()/myVRepeat * myPtr->height());
			p.setTransform(myTransform);

			// with all those calculations done, the actual draw command is
			// almost trivial :-)
			p.drawTiledPixmap(myScaledRect, *myPtr, QPointF());
			p.restore();
		}
	}

	// TODO: why waste more CPU time?
	// TODO: let's also draw the gradient!
//	QGraphicsScene::drawBackground(&p, p.viewport());
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

	if (theWorldPtr->theBackground.theBackgroundGradient.count()==0 &&
		theWorldPtr->theBackground.theImageName.isEmpty())
	{
		// the default if nothing else specified: a blue gradient background
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
	}
	setBackgroundBrush(Qt::blue);
	QLinearGradient myBackground(0,0, 0,-getHeight());
	foreach(Background::GradientStop myGS, theWorldPtr->theBackground.theBackgroundGradient)
		myBackground.setColorAt(myGS.thePosition, QColor(myGS.theR*255, myGS.theG*255, myGS.theB*255, myGS.theAlpha*255));
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
	if (theCongratDeathBoxPtr!=NULL)
	{
		delete theCongratDeathBoxPtr;
		theCongratDeathBoxPtr=NULL;
	}

	theWorldPtr->deletePhysicsWorld();
	// and redraw
	advance();
	isUserInteractionAllowed = true;
	if (theDrawDebug)
		clearGraphicsList(0);

	emit theSimStateMachine->clicked_on_reset();
}

void DrawWorld::startTimer(void)
{
	// note: this member is always called when the timer starts,
	// so even when the timer after a stop continues...
	// don't do anything here that will mess up a continue!
	// (the createPhysicsWorld() will return if a world still exists)

	DEBUG5("DrawWorld::startTimer(void)\n");

	// TODO: upon reset, the old object gets focus again - thanks, Qt!
	DrawObject myTemp(NULL);
	setFocusItem(&myTemp);
	clearFocus();
	Anchors::clearEditObjectDialogPtr();

	isUserInteractionAllowed = false;
	DrawObject::setIsSimRunning(true);

	theWorldPtr->createPhysicsWorld();
	clearGraphicsList(0);
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
void DrawWorld::DrawTransform(UNUSED_ARG const b2Transform& xf)
{
	DEBUG5("DrawWorld::DrawTransform\n");
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
			myMessage = tr("Fail - retry?");
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
	theTextPtr->setTransform(QTransform::fromScale(myResize, myResize),true);
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
