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

#include "DrawNoteIt.h"
#include "BaseObject.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawNoteIt::DrawNoteIt (BaseObject* aBaseObjectPtr)
	: DrawObject(aBaseObjectPtr, "NoteIt"), theDialogPtr(NULL)
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawNoteIt\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);

}

DrawNoteIt::~DrawNoteIt ( )
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


QString DrawNoteIt::getPageString(unsigned int aPage)
{
	QString myPageNr = "page"+QString::number(aPage);
	QString myPlainPage = theBaseObjectPtr->getProperty(myPageNr);
	if (myPlainPage.isEmpty())
		return myPlainPage;
	// first look for page1_nl, then for page1_nl_NL
	QString myLocName = QLocale::system().name();
	myPageNr += "_" + myLocName.mid(0,2);
	QString myLocal1Page = theBaseObjectPtr->getProperty(myPageNr);
	myPageNr += "_" + myLocName.mid(3,2);
	QString myLocal2Page = theBaseObjectPtr->getProperty(myPageNr);

	if (myLocal2Page.isEmpty()==false)
		return myLocal2Page;
	if (myLocal1Page.isEmpty()==false)
		return myLocal1Page;
	return myPlainPage;
}


void DrawNoteIt::initAttributes ( )
{
	
}


void DrawNoteIt::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
	DEBUG5("double click!!!\n");
	 theDialogPtr = new QDialog;
	 theUIPtr = new Ui::NoteItViewer();
	 theUIPtr->setupUi(theDialogPtr);

	 theCurrentPage = 0;
	 nextClicked();

	 QObject::connect(static_cast<QObject*>(theUIPtr->pushButton_Next), SIGNAL(clicked()),
					  this, SLOT(nextClicked()));
	 QObject::connect(static_cast<QObject*>(theUIPtr->pushButton_Cancel), SIGNAL(clicked()),
					  theDialogPtr, SLOT(reject()));

	 theDialogPtr->exec();
}


void DrawNoteIt::nextClicked()
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
