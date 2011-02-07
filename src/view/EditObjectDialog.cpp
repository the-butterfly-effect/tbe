/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "EditObjectDialog.h"
#include "DrawObject.h"
#include "ImageStore.h"

#include <QCloseEvent>

QPoint EditObjectDialog::thePosition = QPoint(0,0);

EditObjectDialog::EditObjectDialog(BaseObject* aBaseObjectPtr, QWidget *aParent)
		: QDialog(aParent, Qt::Tool), theBOPtr(aBaseObjectPtr), theUndoPtr(NULL)
{
	DEBUG4("EditObjectDialog::EditObjectDialog\n");
	ui.setupUi(this);

	theBOPtr = NULL;
	readFromObject(aBaseObjectPtr);

	// upon the very very first init of this dialog, let Qt place the dialog
	// that will at least ensure that the dialog is overlapping the main window.
	// after that, we're in charge!
	if (thePosition != QPoint(0,0))
		move(thePosition);
}

EditObjectDialog::~EditObjectDialog()
{
	thePosition = pos();
}

void EditObjectDialog::lineEditID_valueChanged ( void )
{
	// note: this member also gets called whenever we change a property,
	// I have no clue why - the good news is that pushYourself() will figure
	// out if anything changed and not push and ID change if there is none...
	UndoObjectChange* myUndoPtr = UndoObjectChange::createUndoObject(theBOPtr);
	QString myID = ui.lineEditID->text().trimmed();
	theBOPtr->setID(myID);
	myUndoPtr->update(myID);
	myUndoPtr->pushYourself();
}

void EditObjectDialog::position_editingFinished()
{
	if (theUndoPtr != NULL)
	{
		// just to make sure the latest change also got in...
		position_valueChanged(0);
		theUndoPtr->pushYourself();
	}
	theUndoPtr = NULL;
}

void EditObjectDialog::position_valueChanged (double )
{
	if (theUndoPtr == NULL)
		theUndoPtr = UndoObjectChange::createUndoObject(theBOPtr);
	theUndoPtr->update(
			Position(ui.spinBoxX->value(),
					 ui.spinBoxY->value(),
					 ui.spinBoxAngle->value() ),
			Vector(ui.spinBoxWidth->value(),
				   ui.spinBoxHeight->value()));
}



void EditObjectDialog::propertyCellChanged ( int aRow, int aColumn )
{
	// retrieve corresponding key and value
	QString myKey   = ui.tableWidget->verticalHeaderItem(aRow)->text();
	QString myValue = ui.tableWidget->item(aRow, aColumn)->text().trimmed();
	// is the value changed?
	QString myPropValue;
	theBOPtr->theProps.property2String(myKey, &myPropValue);
	if (myValue == myPropValue)
		return;
	// so, yes it changed.

// FIXME/TODO: this line doesn't work as expected - it will crash tbe.
// I think this is due to recursive calling propertyCellChanged...
//	ui.tableWidget->item(aRow, 0)->setIcon(ImageStore::getQIcon("IconModified", QSize(32,32)));

	UndoObjectChange* myUndoPtr = UndoObjectChange::createUndoObject(theBOPtr);
	theBOPtr->theProps.setProperty(myKey, myValue);
	myUndoPtr->update(myKey, myValue);
	myUndoPtr->pushYourself();

	// now we need to make sure the properties are updated...
	theBOPtr->parseProperties();
}


void EditObjectDialog::readFromObject(BaseObject* aBaseObjectPtr)
{
	// prevent spawning of signals for every update we do below
	// connect everything back up at the end
	disconnect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));

	disconnect(ui.spinBoxAngle,  SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	disconnect(ui.spinBoxHeight, SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	disconnect(ui.spinBoxWidth,  SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	disconnect(ui.spinBoxX,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	disconnect(ui.spinBoxY,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );

	disconnect(ui.lineEditID,    SIGNAL(editingFinished()),    this, SLOT(lineEditID_valueChanged() ));
	disconnect(ui.tableWidget,   SIGNAL(cellChanged(int,int)), this, SLOT(propertyCellChanged(int,int)));

	// if we just changed the base object and there's still an undo pointer
	// around, it will not have any interesting changes
	if (theUndoPtr!=NULL)
	{
		delete theUndoPtr;
		theUndoPtr = NULL;
	}

	theBOPtr = aBaseObjectPtr;
	if (aBaseObjectPtr!=NULL)
	{
		ui.lineEditID->setText(aBaseObjectPtr->getID());
		ui.groupBox->setTitle(tr("Basic Properties for %1").arg(aBaseObjectPtr->getName()));

		//  TODO: QLabel *labelUnique;

		ui.spinBoxX     -> setValue(aBaseObjectPtr->getOrigCenter().x);
		ui.spinBoxY     -> setValue(aBaseObjectPtr->getOrigCenter().y);
		ui.spinBoxAngle -> setValue(aBaseObjectPtr->getOrigCenter().angle);
		ui.spinBoxWidth -> setValue(aBaseObjectPtr->getTheWidth());
		ui.spinBoxHeight-> setValue(aBaseObjectPtr->getTheHeight());

		QStringList myAllPropertiesList = aBaseObjectPtr->theProps.getDefaultPropertyList();
		QStringList::iterator myI= myAllPropertiesList.begin();

		ui.tableWidget->clear();
		ui.tableWidget->setColumnCount(1);
		ui.tableWidget->setRowCount(myAllPropertiesList.count());

		QTableWidgetItem* myCol = new QTableWidgetItem();
		myCol->setText(tr("Value"));
		ui.tableWidget->setHorizontalHeaderItem(0,myCol);

		int myRow = 0;
		while (myI != myAllPropertiesList.end())
		{
			QString myKey = *myI;
			QString myValue;
			aBaseObjectPtr->theProps.property2String(*myI, &myValue);
			QTableWidgetItem* myKeyItem = new QTableWidgetItem(myKey);
			ui.tableWidget->setVerticalHeaderItem(myRow, myKeyItem);
			QTableWidgetItem* myValueItem = new QTableWidgetItem(myValue);
			myValueItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			if (myValue != aBaseObjectPtr->theProps.getDefaultProperty(*myI))
				myValueItem->setIcon(ImageStore::getQIcon("IconModified", QSize(32,32)));
			ui.tableWidget->setItem(myRow, 0, myValueItem);

			myRow++;
			myI++;
		}
		ui.tableWidget->resizeColumnToContents(0);
	}

	connect(ui.spinBoxAngle,  SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	connect(ui.spinBoxHeight, SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	connect(ui.spinBoxWidth,  SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	connect(ui.spinBoxX,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	connect(ui.spinBoxY,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );

	connect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	connect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	connect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));

	connect(ui.lineEditID,    SIGNAL(editingFinished()),    this, SLOT(lineEditID_valueChanged() ));
	connect(ui.tableWidget,   SIGNAL(cellChanged(int,int)), this, SLOT(propertyCellChanged(int,int)));

	if (aBaseObjectPtr==NULL)
		setEnabled(false);
	else
		setEnabled(true);
	return;
}
