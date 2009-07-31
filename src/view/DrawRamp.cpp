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

#include "DrawRamp.h"
#include "BaseObject.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawRamp::DrawRamp (BaseObject* aBaseObjectPtr)
	: DrawObject(aBaseObjectPtr)
{
	// everything is done in the DrawObject constructor
}

DrawRamp::~DrawRamp ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void DrawRamp::advance(int)
{
	// this object never moves - it shouldn't need to advance...
	//applyPosition();
}

QRectF DrawRamp::boundingRect() const
{
	RightRamp* myRampPtr = reinterpret_cast<RightRamp*>(theBaseObjectPtr);
	qreal myWidth = myRampPtr->getTheWidth()*theScale;
	qreal myHeight= myRampPtr->getTheHeight()*theScale;
	const qreal adjust = 0.1;
    return QRectF(-myWidth/2-adjust, -myHeight/2-adjust, myWidth+2*adjust, myHeight+2*adjust);
}

void DrawRamp::initAttributes ( ) 
{
	
}

QPainterPath DrawRamp::shape() const
{
	RightRamp* myRampPtr = reinterpret_cast<RightRamp*>(theBaseObjectPtr);
	qreal myHW = myRampPtr->getTheWidth()*theScale/2.0;
	qreal myHH = myRampPtr->getTheHeight()*theScale/2.0;
	qreal mySH = myRampPtr->theSlabThickness*theScale;

	QPolygonF myPoly;
	if (myRampPtr->isRight)
	{
		myPoly << QPointF(-myHW, -myHH);
		myPoly << QPointF(-myHW, -myHH+mySH);
		myPoly << QPointF( myHW,  myHH);
		myPoly << QPointF( myHW,  myHH-mySH);
		myPoly << QPointF(-myHW, -myHH);
	}
	else
	{
		myPoly << QPointF( myHW, -myHH);
		myPoly << QPointF(-myHW,  myHH-mySH);
		myPoly << QPointF(-myHW,  myHH);
		myPoly << QPointF( myHW, -myHH+mySH);
		myPoly << QPointF( myHW, -myHH);
	}

	QPainterPath myPath;
	myPath.addPolygon(myPoly);
	return myPath;
}

void DrawRamp::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
	myPainter->setBrush(color);

	QPainterPath myPath = shape();
	myPainter->drawPath(myPath);

	//	DEBUG5("void DrawRamp::paint - %fx%f\n",
//			   myRampPtr->getSlabLength(), 
//			   myRampPtr->theSlabThickness);
	
    applyPosition();
}
