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

#include "ViewObject.h"
#include "AbstractObject.h"
#include "PieMenu.h"

#include "tbe_global.h"

#include <QFile>
#include <QGraphicsColorizeEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPixmapCache>
#include <QSvgRenderer>

ViewObject::ViewObject(AbstractObject* anAbstractObjectPtr) :
	QGraphicsPixmapItem(NULL), theAbstractObjectPtr(anAbstractObjectPtr)
{
	// nothing to do yet :-)
	DEBUG3ENTRY;
	Q_ASSERT(anAbstractObjectPtr!=NULL);
	initViewObjectAttributes();
}

ViewObject::ViewObject(AbstractObject* anAbstractObjectPtr, const QString& anImageName) :
		QGraphicsPixmapItem(NULL), theAbstractObjectPtr(anAbstractObjectPtr)
{
	DEBUG3ENTRY;
	Q_ASSERT(anAbstractObjectPtr!=NULL);
	loadImage(anImageName);

	thePixmapWidth = boundingRect().width();
	thePixmapHeight= boundingRect().height();

	adjustObjectDrawing();

	initViewObjectAttributes();
}

ViewObject::~ViewObject()
{
	// nothing to do yet :-)
}


void ViewObject::adjustObjectDrawing(void)
{
	// FIXME/TODO: do we need these if we use setTransform?
	prepareGeometryChange();
	qreal myWidth  = theAbstractObjectPtr->getTheWidth();
	qreal myHeight = theAbstractObjectPtr->getTheHeight();

	// set transformations: origin, scale, translate

	// Qt by default scales and rotates around (0,0),
	// which is a corner of the object

	// Problem: Qt merges transformations and handles them in the wrong order.
	// * it first uses a rotation set with setRotate.
	// * then it executes a transformation set with setTransform
	//   (whatever transformation is set here)
	// * then it executes a translation set with setPos
	//
	// Problem2: you cannot put a rotation and a translation and a scaling
	// into setTransform because the three together will automagically
	// result in an unwanted slanting of the object.
	//
	// Problem3: if you set a rotation and a scaling in the transform matrix
	// it still will look like you do the rotation first and the scaling second
	//
	// So the end result:
	// * we rotate and scale (in that order!) in the setTransform
	// * then we translate in the setPos to set the the midpoint at the
	//   defined position (instead of the top-left corner)

    QTransform theTransform;
     // Qt has the Y axis in opposite direction, so negate the angle...
     theTransform.rotateRadians(-theAbstractObjectPtr->getOrigCenter().angle);
     theTransform.scale(THESCALE*myWidth/thePixmapWidth,
                        THESCALE*myHeight/thePixmapHeight);
    setTransform(theTransform,false);
    update();

	// Now we need to calculate the translation, based on the scaled&rotated
	// object that happened around the top-left corner.
	// So, where is the middle of our object currently?
	// And -as always- don't forget to correct for Qt's wrong Y axis...
	Position myMiddle = Position(0,0, -theAbstractObjectPtr->getOrigCenter().angle)
						+ Vector(myWidth/2, myHeight/2);
	myMiddle.y = - myMiddle.y;
	setPos(theAbstractObjectPtr->getOrigCenter().toQPointF() - myMiddle.toQPointF());
}

void ViewObject::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
    // this looks great, but unfortunately it also affects all children
    QGraphicsEffect* myEffect = new QGraphicsColorizeEffect();
    setGraphicsEffect(myEffect);
}


void ViewObject::hoverLeaveEvent ( QGraphicsSceneHoverEvent* )
{
    setGraphicsEffect(NULL);
}

void ViewObject::initViewObjectAttributes(void)
{
//    setFlags(QGraphicsItem::ItemIsMovable |
//             QGraphicsItem::ItemIsSelectable);
    setFlags(QGraphicsItem::ItemIsFocusable);
    setAcceptsHoverEvents(true);
}


bool ViewObject::loadImage(const QString& anImageName)
{
	Q_ASSERT(anImageName.isEmpty() == false);

	QPixmap myTempPixmap = NULL;

	// is the image present in the cache?
	if (QPixmapCache::find(anImageName, myTempPixmap))
	{
		DEBUG4("Image '%s' found in image cache!", ASCII(anImageName));
	}
	else
	{
		// No, it isn't. Let's try to load the image.
		QString myFullPathName;
		// I use a do...while(true) loop here to be able to break from a loop
		// that is run only once.
		do
		{
			myFullPathName = QString("../images/%1.png").arg(anImageName);
			DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
			if (QFile::exists(myFullPathName))
				break;

			myFullPathName = QString("../images/%1.jpg").arg(anImageName);
			DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
			if (QFile::exists(myFullPathName))
				break;

			myFullPathName = QString("../images/%1.svg").arg(anImageName);
			DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
			if (QFile::exists(myFullPathName))
			{
				// render the SVG into the Pixmap
				// TODO/FIXME: magic numbers (pixmap size) here
				myTempPixmap = QPixmap(160,160);
				myTempPixmap.fill(QColor(255,255,255,0));

				QSvgRenderer myRenderer(myFullPathName);
				QPainter myPainter;
				myPainter.begin(&myTempPixmap);
				myPainter.setRenderHint(QPainter::Antialiasing);
				myRenderer.render(&myPainter);
				myPainter.end();
				break;
			}

			myFullPathName = "../images/BowlingBall.png";
			DEBUG4("attempt to load '%s'", ASCII(myFullPathName));
		} while(false);

		if (myTempPixmap.isNull())
			myTempPixmap = QPixmap(myFullPathName);
		QPixmapCache::insert(anImageName, myTempPixmap);
	}

	setPixmap(myTempPixmap);
	return true;
}


void ViewObject::mousePressEvent ( QGraphicsSceneMouseEvent* anEvent )
{
    hoverLeaveEvent(NULL);
    PieMenuSingleton::addPieMenuToViewObject(this, anEvent->pos());
}

void ViewObject::setNewGeometry(const Position& aNewPosition, qreal aNewWidth, qreal aNewHeight)
{
    DEBUG4("%s", ASCII(QString("setNewGeometry Pos: %1, WxH %2x%3")
                .arg(aNewPosition.toString())
                .arg(aNewWidth).arg(aNewHeight)));
    theAbstractObjectPtr->setTheWidth(aNewWidth);
    theAbstractObjectPtr->setTheHeight(aNewHeight);
    setNewGeometry(aNewPosition);
}

void ViewObject::setNewGeometry(const Position& aNewPosition)
{
    theAbstractObjectPtr->setOrigCenter(aNewPosition);
    adjustObjectDrawing();
}
