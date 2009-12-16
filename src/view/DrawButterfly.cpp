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

#include "DrawButterfly.h"
#include "BaseObject.h"
#include "ImageStore.h"
#include "DrawWorld.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawButterfly::DrawButterfly (BaseObject* aBaseObjectPtr,
						UNUSED_ARG const QString& anImageName,
						UNUSED_ARG DrawObject::ImageType anImageType)
	: DrawObject(aBaseObjectPtr, anImageName)
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawButterfly\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);
}

DrawButterfly::~DrawButterfly ( )
{
}

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  



void DrawButterfly::initAttributes ( )
{
	
}


void DrawButterfly::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
	qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
	QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

	DEBUG5("DrawButterfly::paint for %p: @(%f,%f)\n", this, myWidth, myHeight);


	if (theRenderer != NULL)
	{
		// what to render depends on the state of the object
		Butterfly::ButterflyStatus myStatus =
				dynamic_cast<Butterfly*>(theBaseObjectPtr)->getState();

		if (myStatus == Butterfly::DEAD)
			emit (reinterpret_cast<DrawWorld*>(scene()))->on_death();
		
		if (myStatus == Butterfly::FLAP_HALF)
			theRenderer->render(myPainter, "Wing-halfopen", myRect);

		if (myStatus == Butterfly::STILL)
			theRenderer->render(myPainter, "Wing-closed", myRect);

		theRenderer->render(myPainter, "Body", myRect);

		if (myStatus == Butterfly::FLAP_OPEN || myStatus == Butterfly::DEAD)
			theRenderer->render(myPainter, "Wing-open", myRect);

		return;
	}

	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
	// Body
	myPainter->setBrush(color);
	myPainter->drawEllipse(-myWidth/2, -myHeight/2, myWidth, myHeight);
}
