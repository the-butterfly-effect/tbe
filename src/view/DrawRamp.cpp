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
	Ramp* myRampPtr = reinterpret_cast<Ramp*>(theBaseObjectPtr);
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

	Ramp* myRampPtr = reinterpret_cast<Ramp*>(theBaseObjectPtr);
	QPointF mySlabH(0, myRampPtr->theSlabThickness*theScale);

	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
	myPainter->setBrush(color);
	myPainter->drawLine(myBounds.topLeft(), myBounds.bottomRight()-mySlabH);
	myPainter->drawLine(myBounds.bottomRight()-mySlabH, myBounds.bottomRight());
	myPainter->drawLine(myBounds.bottomRight(), myBounds.topLeft()+mySlabH);
	myPainter->drawLine(myBounds.topLeft()+mySlabH, myBounds.topLeft());

	paintHighlighted(myPainter);

//	DEBUG5("void DrawRamp::paint - %fx%f\n",
//			   myRampPtr->getSlabLength(), 
//			   myRampPtr->theSlabThickness);
	
    applyPosition();
}
