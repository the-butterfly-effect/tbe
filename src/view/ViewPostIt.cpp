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

#include "ViewPostIt.h"
//#include "PostItEditor.h"
#include "ImageCache.h"
#include "animateddialog.h"
#include "AbstractObject.h"

#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QGraphicsSceneMouseEvent>

// Constructors/Destructors
//

ViewPostIt::ViewPostIt (AbstractObjectPtr aAbstractObjectPtr)
	: ViewObject(aAbstractObjectPtr, "PostIt"),
	  theCurrentPage(0),
	  theDialogPtr(NULL),
	  theUIPtr(NULL)
{
    // everything is done in the ViewObject constructor
	DEBUG5ENTRY;

    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

ViewPostIt::~ViewPostIt ( )
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

void ViewPostIt::displayPostit(void)
{
    theUIPtr = new Ui::ViewPostIt();
    QLabel* myUIWidgetPtr = new QLabel();
    theUIPtr->setupUi(myUIWidgetPtr);
    theDialogPtr = new AnimatedDialog(ResizingGraphicsView::me());
    theDialogPtr->resize(240,240);

    QPixmap myPixmap;
    ImageCache::getPixmap("PostItBackground", QSize(240,240), &myPixmap);
    Q_ASSERT(myPixmap.isNull()==false);
    myUIWidgetPtr->setPixmap(myPixmap);
    myUIWidgetPtr->setParent(theDialogPtr);

    theCurrentPage = 0;
    nextClicked();

    connect(static_cast<QObject*>(theUIPtr->pushButton_Next), SIGNAL(clicked()),
            this, SLOT(nextClicked()));
    connect(static_cast<QObject*>(theUIPtr->pushButton_Cancel), SIGNAL(clicked()),
            theDialogPtr, SLOT(disappearAnimated()));
    emit theDialogPtr->appearAnimated();
}

void ViewPostIt::hoverEnterEvent ( QGraphicsSceneHoverEvent* )
{
    // this looks great, but unfortunately it also affects all children
    QGraphicsColorizeEffect* myEffect = new QGraphicsColorizeEffect();
    myEffect->setColor(QColor(192, 192, 0));
    setGraphicsEffect(myEffect);
}

void ViewPostIt::hoverLeaveEvent ( QGraphicsSceneHoverEvent* )
{
    setGraphicsEffect(NULL);
}



QString ViewPostIt::getPageString(unsigned int aPage)
{
    QString myPageNr = "page"+QString::number(aPage);

    // get the default/original string (English) in 'page1'
    // this will be returned if none of the below exist
    QString myPageString = theAbstractObjectPtr->theProps.getPropertyNoDefault(myPageNr);

    // then look for 'page1_nl', then for 'page1_nl_NL'
    // if the second one exists, it overrides an existing first one
    QString myLocName = QLocale::system().name();
    myPageNr += "_" + myLocName.mid(0,2);
    theAbstractObjectPtr->theProps.property2String(myPageNr, &myPageString,false);
    myPageNr += "_" + myLocName.mid(3,2);
    theAbstractObjectPtr->theProps.property2String(myPageNr, &myPageString,false);
    return myPageString;
}


void ViewPostIt::initAttributes ( )
{

}


void ViewPostIt::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	DEBUG5("double click!!!");
//	if (theIsLevelEditor)
//	{
//		QDialog* myEditorPtr = new PostItEditor(theAbstractObjectPtr, this);
//		myEditorPtr->exec();
//	}
//	else
    displayPostit();
    event->accept();
}

void ViewPostIt::mousePressEvent(QGraphicsSceneMouseEvent* anEvent)
{
    // depending on whether we're level editor or not, move or display
    if (theIsLevelEditor)
        ViewObject::mousePressEvent(anEvent);
    else
        mouseDoubleClickEvent(anEvent);
}

void ViewPostIt::nextClicked()
{
    theCurrentPage++;
    QString myPageString = getPageString(theCurrentPage);

    // no page? that means the user has hit the finish button
    if (myPageString.isEmpty())
    {
        emit theDialogPtr->disappearAnimated();
        return;
    }

    theUIPtr->theLabel->setText("<b>"+myPageString+"</b>");
    theUIPtr->theLabel->setAlignment(Qt::AlignCenter);
    theUIPtr->theLabel->setWordWrap(true);

    // if there is no next page, replace button text with "Finish"
    if (getPageString(theCurrentPage+1).isEmpty())
        theUIPtr->pushButton_Next->setText(tr("Finish"));
}
