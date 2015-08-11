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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "DrawPostIt.h"
#include "BaseObject.h"
#include "PostItEditor.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//

DrawPostIt::DrawPostIt (BaseObject* aBaseObjectPtr)
	: DrawObject(aBaseObjectPtr, "PostIt"), theDialogPtr(NULL)
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawPostIt\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);
	setCacheMode(QGraphicsItem::NoCache);
	setAcceptHoverEvents(true);
	isHovering = false;
}

DrawPostIt::~DrawPostIt ( )
{
	if (theDialogPtr)
		delete theDialogPtr;
}

//
// Methods
//


// Accessor methods
//


// Other methods
//

void DrawPostIt::displayPostit(void)
{
	theDialogPtr = new QDialog;
	theUIPtr = new Ui::PostItViewer();
	theUIPtr->setupUi(theDialogPtr);

	theCurrentPage = 0;
	nextClicked();

	QObject::connect(static_cast<QObject*>(theUIPtr->pushButton_Next), SIGNAL(clicked()),
				  this, SLOT(nextClicked()));
	QObject::connect(static_cast<QObject*>(theUIPtr->pushButton_Cancel), SIGNAL(clicked()),
				  theDialogPtr, SLOT(reject()));
	theDialogPtr->exec();
	}

void DrawPostIt::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
	isHovering=true;
	update();
}

void DrawPostIt::hoverLeaveEvent ( QGraphicsSceneHoverEvent* )
{
	isHovering=false;
	update();
}



QString DrawPostIt::getPageString(unsigned int aPage)
{
	QString myPageNr = "page"+QString::number(aPage);

	// get the default/original string (English) in 'page1'
	// this will be returned if none of the below exist
	QString myPageString = theBaseObjectPtr->theProps.getPropertyNoDefault(myPageNr);

	// then look for 'page1_nl', then for 'page1_nl_NL'
	// if the second one exists, it overrides an existing first one
	QString myLocName = QLocale::system().name();
	myPageNr += "_" + myLocName.mid(0,2);
	theBaseObjectPtr->theProps.property2String(myPageNr, &myPageString,false);
	myPageNr += "_" + myLocName.mid(3,2);
	theBaseObjectPtr->theProps.property2String(myPageNr, &myPageString,false);
	return myPageString;
}


void DrawPostIt::initAttributes ( )
{

}


void DrawPostIt::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	DEBUG5("double click!!!\n");
	if (theIsLevelEditor)
	{
		QDialog* myEditorPtr = new PostItEditor(theBaseObjectPtr, this);
		myEditorPtr->exec();
	}
	else
		displayPostit();
	event->accept();
}

void DrawPostIt::mousePressEvent(QGraphicsSceneMouseEvent* anEvent)
{
	// depending on whether we're level editor or not, move or display
	if (theIsLevelEditor)
		DrawObject::mousePressEvent(anEvent);
	else
		mouseDoubleClickEvent(anEvent);
}

void DrawPostIt::nextClicked()
{
	theCurrentPage++;
	QString myPageString = getPageString(theCurrentPage);

	// no page? that means the user has hit the finish button
	if (myPageString.isEmpty())
	{
		theDialogPtr->accept();
		return;
	}

	theUIPtr->theLabel->setText("<b>"+myPageString+"</b>");
	theUIPtr->theLabel->setAlignment(Qt::AlignCenter);
	theUIPtr->theLabel->setWordWrap(true);

	// if there is no next page, replace button text with "Finish"
	if (getPageString(theCurrentPage+1).isEmpty())
		theUIPtr->pushButton_Next->setText(tr("Finish"));
}


void DrawPostIt::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *myStyle, QWidget *myWPtr)
{
	DrawObject::paint(myPainter, myStyle, myWPtr);

	if (isHovering)
	{
		qreal myWidth = theBaseObjectPtr->getTheWidth()*theScale;
		qreal myHeight= theBaseObjectPtr->getTheHeight()*theScale;
		QRectF myRect(-myWidth/2.0,-myHeight/2.0,myWidth,myHeight);

		QPen   myPen(Qt::NoPen);
		myPainter->setPen(myPen);
		QColor myColor(255,255,255,155);
		myPainter->setBrush(myColor);

		myPainter->drawRect(myRect);
	}
}
