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

#include "DrawObject.h"
#include "BaseObject.h"
#include "Anchors.h"
#include "ImageStore.h"
#include "DrawWorld.h"
#include "resizinggraphicsview.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>
#include <QUndoStack>

#include "UndoObjectChange.h"
#include <cmath>

static const int EXTRA_WHITESPACE = 2;

const qreal DrawObject::theScale = 100.0;


// set/get using static setter/getter in DrawObject
// as such it is available to DrawObject and derived classes
static QUndoStack* theUndoStackPtr = NULL;

static bool isSimRunning = false;

Anchors* DrawObject::theAnchorsPtr = NULL;

QSvgRenderer* DrawObject::Cross::theCrossRendererPtr = NULL;

// Constructors/Destructors
//  

DrawObject::DrawObject (BaseObject* aBaseObjectPtr)
	: theBaseObjectPtr(aBaseObjectPtr),
	isCaching(false), theLastImageIndex(0), theUndeleteDrawWorldPtr(NULL),
	theUndoMovePtr(NULL)
{
	DEBUG6("DrawObject::DrawObject(%p)\n", aBaseObjectPtr);
	if (theBaseObjectPtr!=NULL)
		initAttributes();
}

DrawObject::DrawObject (BaseObject* aBaseObjectPtr,
						const QString& anImageName,
						UNUSED_ARG DrawObject::ImageType anImageType)
	: theBaseObjectPtr(aBaseObjectPtr), isCaching(false),
	  theLastImageIndex(0), theUndeleteDrawWorldPtr(NULL), theUndoMovePtr(NULL)
{
	DEBUG6("DrawObject::DrawObject(%p,%s)\n", aBaseObjectPtr, ASCII(anImageName));
	initAttributes();

	QStringList myImageNames = anImageName.split(";");
	for (int i=0; i< myImageNames.count(); i++)
	{
		if (anImageType==IMAGE_PNG || anImageType==IMAGE_ANY)
		{
			QPixmap* myPtr = ImageStore::getPNGPixmap(myImageNames[i]);
			if (myPtr == NULL)
				break;
			thePixmapPtrs.push_back(myPtr);
		}
	}
	if (thePixmapPtrs.size() == 0)
	{
		for (int i=0; i< myImageNames.count(); i++)
		{
			theRenderers.push_back(ImageStore::getRenderer(myImageNames[i]));
		}
	}

	DEBUG6("  Total images: %d pixmap, %d svg\n", thePixmapPtrs.count(), theRenderers.count());
}


DrawObject::~DrawObject ( )
{
	DEBUG5("DrawObject::~DrawObject() this=%p, bo=%p\n", this, theBaseObjectPtr);
	if (theAnchorsPtr)
	{
		delete theAnchorsPtr;
		theAnchorsPtr = NULL;
	}
}

//  
// Methods
//  


// Accessor methods
//  

void DrawObject::setIsSimRunning(bool aBool)
{
	isSimRunning = aBool;
}


void DrawObject::setUndoStackPtr(QUndoStack* aPtr)
{
	assert(aPtr != NULL);
	theUndoStackPtr = aPtr;
}

float DrawObject::getUnscaledAspectRatio(void) const
{
	QSize mySize(1, 1);

	if (getRenderer() != NULL)
		mySize = getRenderer()->defaultSize();

	if (getPixmapPtr() != NULL)
		mySize = getPixmapPtr()->size();

	return static_cast<float>(mySize.width())/
			static_cast<float>(mySize.height());
}


QUndoStack* DrawObject::getUndoStackPtr(void)
{
	assert(theUndoStackPtr != NULL);
	return theUndoStackPtr;
}

// Other methods
//  

void DrawObject::advance(int)
{
	applyPosition();

	// make sure to always redraw if the image animation frame
	// changed...
	if (theLastImageIndex != theBaseObjectPtr->getImageIndex())
		update(boundingRect());
}

void DrawObject::applyPosition(void)
{
	Position myPosition = theBaseObjectPtr->getTempCenter();
    // Qt has Y positive downwards, whereas all of the model has Y upwards.
    // that's what the minus is for :-)
    setPos(myPosition.x, -myPosition.y);
	rotate((-myPosition.angle-theOldAngle)*180/3.14);
	theOldAngle=-myPosition.angle;
}

QRectF DrawObject::boundingRect() const
{
	qreal myWidth = theBaseObjectPtr->getTempWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTempHeight()*theScale;
	qreal adjust = 0.03;
	
    return QRectF(-myWidth/2-adjust, -myHeight/2-adjust, myWidth+2*adjust, myHeight+2*adjust);
}

bool DrawObject::checkForCollision(void)
{
	QList<QGraphicsItem *> myCollides;
	int myCount = 0;

	// are we in the level editor???
	// if so: there is a "feature" to disable collision detection
	if (theIsLevelEditor==true && theIsCollisionOn==false)
		return false;

	// collision is also automatic when we run through either the X or Y axis
	// so, let's calculate the AABB for the transformed object
	QRectF myOutline = mapToScene(boundingRect()).boundingRect();
	if (myOutline.bottom() > 0.0 || myOutline.left() < 0.0)
		goto YesCollision;

	// get the list of all collisions and count non-Anchors/non-Cross items
	myCollides = collidingItems();
	for (int i=0; i< myCollides.size(); i++)
	{
		if (theAnchorsPtr)
			if (theAnchorsPtr->isAnchor(myCollides[i]))
				continue;
		if (myCollides[i]==theCrossPtr)
			continue;
		myCount++;
	}

	if (myCount>=1)
		goto YesCollision;

	removeCollisionCross();
	return false;

YesCollision:
	if (theCrossPtr==NULL)
		theCrossPtr = new Cross(this);
	return true;
}

QPixmap* DrawObject::createBitmap(int aWidth, int aHeight)
{
	DEBUG5("createBitmap for %p (%dx%d)\n", this, aWidth, aHeight);
	float myWidth = aWidth;
	float myHeight= aHeight;
	if (aWidth==0 || aHeight == 0)
	{
		float myPixPerUnit = ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
		DEBUG5("   %f pix/m at %f x %f\n", myPixPerUnit, theBaseObjectPtr->getTempWidth(), theBaseObjectPtr->getTempHeight());
		myWidth = theBaseObjectPtr->getTempWidth() *myPixPerUnit;
		myHeight= theBaseObjectPtr->getTempHeight()*myPixPerUnit;
	}
	DEBUG5("   will do %f x %f bitmap\n", myWidth, myHeight);
	assert(myWidth>0);
	assert(myHeight>0);
	// debugging assistant: catch runaway scene scalings
	assert(myWidth<2000);
	assert(myHeight<2000);

	QPixmap* myPixmap = new QPixmap(myWidth+EXTRA_WHITESPACE, myHeight+EXTRA_WHITESPACE);
	myPixmap->fill(QColor(Qt::transparent));
	QPainter myPainter(myPixmap);
//	myPainter.drawRect(0,0,myWidth+EXTRA_WHITESPACE-1,myHeight+EXTRA_WHITESPACE-1);
	myPainter.translate((myWidth+EXTRA_WHITESPACE+1)/2, (myHeight+EXTRA_WHITESPACE+1)/2);

	// let's try to do some scaling
	float myScaleX = boundingRect().width()/myWidth;
	float myScaleY = boundingRect().height()/myHeight;
	float myTrueScale;
	if (myScaleY > myScaleX)
		myTrueScale = 1/myScaleY;
	else
		myTrueScale = 1/myScaleX;
	myPainter.scale(myTrueScale, myTrueScale);

	paint(&myPainter, NULL, NULL);
	return myPixmap;
}


void DrawObject::focusInEvent (void)
{
	scene()->setFocusItem(this, Qt::OtherFocusReason);
}


void DrawObject::focusInEvent ( UNUSED_ARG QFocusEvent* event )
{
	DEBUG5("focusInEvent for %p with %d\n", this, event->reason());
	if (theAnchorsPtr!=NULL)
	{
		// the Anchors destructor will notify the other DrawObject
		delete theAnchorsPtr;
	}
	theAnchorsPtr=new Anchors(this);
	update();
}

//void DrawObject::focusOutEvent ( QFocusEvent * event )
//{
//	DEBUG1("focusOutEvent for %p with %d\n", this, event->reason());
//	delete theAnchorsPtr;
//	theAnchorsPtr = NULL;
//}

void DrawObject::focusRemove(bool alsoDeleteAnchors)
{
	if (theAnchorsPtr!=NULL && alsoDeleteAnchors==true)
	{
		delete theAnchorsPtr;
		theAnchorsPtr=NULL;
	}
	update();
}

QPixmap* DrawObject::getPixmapPtr(void) const
{
	if (theBaseObjectPtr==NULL)
		return NULL;
	int myIndex = theBaseObjectPtr->getImageIndex();
	if (isCaching)
		myIndex = theCachingIndex;
	if (thePixmapPtrs.count()>myIndex)
		return thePixmapPtrs[myIndex];
	else
		if (thePixmapPtrs.count()>0)
			return thePixmapPtrs[0];
	return NULL;
}

QSvgRenderer* DrawObject::getRenderer(void) const
{
	if (theBaseObjectPtr==NULL)
		return NULL;
	int myIndex = theBaseObjectPtr->getImageIndex();
	if (isCaching)
		myIndex = theCachingIndex;
	if (theRenderers.count()>myIndex)
		return theRenderers[myIndex];
	else
		if (theRenderers.count()>0)
			return theRenderers[0];
	return NULL;
}

void DrawObject::initAttributes ( )
{
	// the objects sizes usually are less than a meter
	// however, that does not sit well with QPainter, which is still a
	// bitmap-oriented class - we're discussing images of less than one-by-one pixel.
	// that's what we need scaling for.
	//
	// theScale is set to 100.0 - that implies centimeters.
	scale(1.0/theScale, 1.0/theScale);

	// in radians!
	theOldAngle=0;//aBaseObjectPtr->getOrigCenter().angle;

	theZValue = 2.0;

	// make sure that this item is selectable & draggable
	// (if the object allows it - of course)
	if (theBaseObjectPtr->isMovable())
	{
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
		setAcceptHoverEvents(true);
	}

	setToolTip(theBaseObjectPtr->getToolTip());

	theCrossPtr = NULL;
	applyPosition();
}


void DrawObject::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseMoveEvent(%d)\n", event->type());

	// do not allow movement of predefined objects in game mode
	if (theBaseObjectPtr->isMovable() == false)
		return;

	// unfortunately, the hot spot position in the event
	// is in item coordinates, so it rotates around with the object
	mouseMoveEvent(event->scenePos(), event->pos());
}


void DrawObject::mouseMoveEvent (const QPointF& aScenePos, const QPointF& aHotspotPos)
{
	// if this is the first call to mouseMove, we need to create and initialise the undomove
	if (theUndoMovePtr==NULL)
	{
		// a HotspotPos is in item coordinates, we need scene coords
		Vector myPos(aHotspotPos.x(), -aHotspotPos.y());
		myPos = 1/theScale*myPos;
		myPos = myPos.rotate(theBaseObjectPtr->getOrigCenter().angle);
		theUndoMovePtr = UndoObjectChange::createUndoObject(theBaseObjectPtr);
		theMoveHotspot = QPointF(myPos.dx, -myPos.dy);
		if (theAnchorsPtr)
		{
			delete theAnchorsPtr;
			theAnchorsPtr = NULL;
		}
	}

	theUndoMovePtr->update( Vector(aScenePos-theMoveHotspot) );
}

void DrawObject::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	DEBUG5("DrawObject::mouseReleaseEvent(%d) for button %d\n", event->type(), event->button());
	// TODO: FIXME: for now, we're not discriminative for left or right mousebutton...

	if (theUndoMovePtr == NULL)
		return;

	if (theUndoMovePtr->pushYourself()==true)
	{
		DEBUG5("PUSHED UNDO\n");
	}
	else
	{
		DEBUG5("CLEARED UNDO\n");
		theUndoMovePtr->undo();
		delete theUndoMovePtr;
	}
	theUndoMovePtr = NULL;

	QGraphicsItem::mouseReleaseEvent(event);
	if (theAnchorsPtr==NULL)
		theAnchorsPtr= new Anchors(this);
}

void DrawObject::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if ( theDrawDebug==true && isSimRunning==true )
		return;

	qreal myWidth = theBaseObjectPtr->getTempWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTempHeight()*theScale;

	// only when we're using the cached bitmaps, (which have a few pix whitespace)
	// let's paint larger to offset the whitespace.
	int myCachedCount = theCachePixmapPtrs.count();
	if (myCachedCount != 0 && isCaching==false)
	{
	  myWidth += theScale*EXTRA_WHITESPACE/ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
	  myHeight += theScale*EXTRA_WHITESPACE/ResizingGraphicsView::getPixelsPerSceneUnitHorizontal();
	}
	
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

	DEBUG6("DrawObject::paint for %p: @(%f,%f)\n", this, myWidth, myHeight);
	if (myCachedCount != 0 && isCaching==false)
	{
		theLastImageIndex = theBaseObjectPtr->getImageIndex();
		if (theLastImageIndex >= myCachedCount)
			theLastImageIndex = 0;
		myPainter->drawPixmap(myRect, *theCachePixmapPtrs[theLastImageIndex],
							  theCachePixmapPtrs[theLastImageIndex]->rect());
		return;
	}

	if (getPixmapPtr() != NULL)
	{
		myPainter->drawPixmap(myRect, *getPixmapPtr(), getPixmapPtr()->rect());
		return;
	}
	
	if (getRenderer() != NULL)
	{
		getRenderer()->render(myPainter, myRect);
		return;
	}

	// Backup for Body drawing
	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
	myPainter->setBrush(color);
	myPainter->drawEllipse(myRect);
}

bool DrawObject::pushUndo(QUndoCommand* anUndo)
{
	getUndoStackPtr()->push(anUndo);
	return true;
}

void DrawObject::removeCollisionCross(void)
{
	if (theCrossPtr)
	{
		delete theCrossPtr;
		theCrossPtr = NULL;
	}
}

void DrawObject::setupCache(void)
{
	// In most situations, we want to cache the SVG drawing into a bitmap
	// so we save a lot of CPU performance on drawing.
	// (derived classes can fix this - e.g. Butterfly and CokeMentosBottle)
	// The problem is that QT does a crappy job at guessing the bitmap size
	// so we have to calculate that ourselves...

	if (getRenderer()!=NULL || getPixmapPtr()!=NULL)
	{
		setCacheMode(QGraphicsItem::NoCache);
		theCachePixmapPtrs.clear();
		isCaching = true;
		for(theCachingIndex=0;
			theCachingIndex<theRenderers.count()+thePixmapPtrs.count();
			theCachingIndex++)
		{
			// let's try to use the renderer to create a cached image:
			theCachePixmapPtrs.push_back(createBitmap());
		}
		isCaching = false;
	}

	// we also still need to set the ZValue of this object.
	// it only now guaranteed has a parent
	QGraphicsItem::setZValue(theZValue);
}






// ==================== DrawObject::Cross ==================================


DrawObject::Cross::Cross(DrawObject* aParent)
{
	DEBUG5("Cross::Cross()\n");
	if (theCrossRendererPtr==NULL)
	{
		theCrossRendererPtr = ImageStore::getRenderer("BigCross");
	}

	theBaseObjectPtr = aParent->theBaseObjectPtr;
	setParentItem(aParent);
}

DrawObject::Cross::~Cross()
{
	// nothing to do
}

void DrawObject::Cross::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	qreal myWidth = theBaseObjectPtr->getTempWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTempHeight()*theScale;
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);
	if (theCrossRendererPtr)
		theCrossRendererPtr->render(myPainter, myRect);
}
