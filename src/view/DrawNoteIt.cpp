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
#include "ui_NoteItViewer.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawNoteIt::DrawNoteIt (BaseObject* aBaseObjectPtr)
	: DrawObject(aBaseObjectPtr, "NoteIt")
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

void DrawNoteIt::initAttributes ( )
{
	
}


void DrawNoteIt::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
	DEBUG5("double click!!!\n");
	 theDialogPtr = new QDialog;
	 Ui::NoteItViewer myUI;
	 myUI.setupUi(theDialogPtr);

	 theCurrentPage = 0;
	 nextClicked();

	 QObject::connect(static_cast<QObject*>(myUI.pushButton_Next), SIGNAL(clicked()),
					  this, SLOT(nextClicked()));

	 theDialogPtr->exec();
}


void DrawNoteIt::nextClicked()
{
	theCurrentPage++;
	// if there is a next page, display it

	// if there is no next page, replace button text with "Finish"

	// if it already says Finish, close it!
}
