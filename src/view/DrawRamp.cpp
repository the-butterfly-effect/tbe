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

void DrawRamp::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QRectF myBounds = boundingRect();

	RightRamp* myRampPtr = reinterpret_cast<RightRamp*>(theBaseObjectPtr);
	QPointF mySlabH(0, myRampPtr->theSlabThickness*theScale);

	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
	myPainter->setBrush(color);

	if (myRampPtr->isRight)
	{
		myPainter->drawLine(myBounds.topLeft(), myBounds.bottomRight()-mySlabH);
		myPainter->drawLine(myBounds.bottomRight()-mySlabH, myBounds.bottomRight());
		myPainter->drawLine(myBounds.bottomRight(), myBounds.topLeft()+mySlabH);
		myPainter->drawLine(myBounds.topLeft()+mySlabH, myBounds.topLeft());
	}
	else
	{
		myPainter->drawLine(myBounds.topRight(), myBounds.bottomLeft()-mySlabH);
		myPainter->drawLine(myBounds.bottomLeft()-mySlabH, myBounds.bottomLeft());
		myPainter->drawLine(myBounds.bottomLeft(), myBounds.topRight()+mySlabH);
		myPainter->drawLine(myBounds.topRight()+mySlabH, myBounds.topRight());
	}


//	DEBUG5("void DrawRamp::paint - %fx%f\n",
//			   myRampPtr->getSlabLength(), 
//			   myRampPtr->theSlabThickness);
	
    applyPosition();
}
