#include "DrawObject.h"
#include "BaseObject.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawObject::DrawObject (BaseObject* aBaseObjectPtr)
	: theBaseObjectPtr(aBaseObjectPtr)
{
	theBodyID = aBaseObjectPtr->getTheBodyID();
	initAttributes();
//	dReal myX = 
//	setRect ( qreal x, qreal y, qreal width, qreal height )

	// only set the scale once !!!
    DEBUG5("scale %p: %f, %f\n", this, theBaseObjectPtr->getTheWidth()/2.0, theBaseObjectPtr->getTheHeight()/2.0);
    scale(theBaseObjectPtr->getTheWidth()/2.0, theBaseObjectPtr->getTheHeight()/2.0);

}

DrawObject::~DrawObject ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void DrawObject::advance(int step)
{
	// TODO: Add rotation here
	const dReal *pos1 = dGeomGetPosition (theBaseObjectPtr->getTheGeomID());
	setPos(pos1[0], pos1[1]);
}

QRectF DrawObject::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

void DrawObject::initAttributes ( ) 
{
	
}

void DrawObject::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
    // Body
    myPainter->setBrush(color);
    myPainter->drawEllipse(-1, -1, 1, 1);


    const dReal *pos1 = dGeomGetPosition (theBaseObjectPtr->getTheGeomID());
DEBUG5("%p: %f, %f\n", this, pos1[0], pos1[1]);    
    setPos(pos1[0], pos1[1]);
}
