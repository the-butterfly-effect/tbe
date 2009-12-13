/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#include "DrawCokeMentosBottle.h"
#include "BaseObject.h"
#include "ImageStore.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawCokeMentosBottle::DrawCokeMentosBottle (BaseObject* aBaseObjectPtr,
						UNUSED_ARG const QString& anImageName,
						UNUSED_ARG DrawObject::ImageType anImageType)
	: DrawObject(aBaseObjectPtr, "CokeMentosBottle")
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawCokeMentosBottle\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);
}

DrawCokeMentosBottle::~DrawCokeMentosBottle ( )
{
}

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  



void DrawCokeMentosBottle::initAttributes ( )
{
	
}


void DrawCokeMentosBottle::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

	DEBUG6("DrawCokeMentosBottle::paint for %p: @(%f,%f)\n", this, myWidth, myHeight);
	if (theRenderer != NULL)
	{
		// what to render depends on the state of the object
		CokeMentosBottle::BottleStatus myStatus =
				dynamic_cast<CokeMentosBottle*>(theBaseObjectPtr)->getBottleStatus();

		if (myStatus == CokeMentosBottle::TRIGGERED
			|| myStatus == CokeMentosBottle::BLOWING)
			theRenderer->render(myPainter, "foam", myRect);

		if (myStatus == CokeMentosBottle::UNTRIGGERED)
			theRenderer->render(myPainter, "liquid", myRect);

		theRenderer->render(myPainter, "bottle", myRect);

		if (myStatus == CokeMentosBottle::UNTRIGGERED
			|| myStatus == CokeMentosBottle::TRIGGERED)
			theRenderer->render(myPainter, "cap", myRect);

		goto checkForCollision;
	}

	{
		// Body
		QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
		myPainter->setBrush(color);
		myPainter->drawEllipse(-myWidth/2, -myHeight/2, myWidth, myHeight);
	}

checkForCollision:
	drawCollisionCross(myPainter, myRect);

}
