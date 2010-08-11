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
#include "GoalEditor.h"
#include "DrawObject.h"

EditObjectDialog::EditObjectDialog(BaseObject* aBaseObjectPtr, QWidget *aParent)
		: QDialog(aParent, Qt::Tool), theBOPtr(aBaseObjectPtr), theUndoPtr(NULL)
{
	DEBUG4("EditObjectDialog::EditObjectDialog\n");
	ui.setupUi(this);

	theBOPtr = NULL;
	readFromObject(aBaseObjectPtr);

}

EditObjectDialog::~EditObjectDialog()
{
}


void EditObjectDialog::lineEditID_valueChanged ( void )
{
	// FIXME/TODO: put this into an UNDO
	theBOPtr->setID(ui.lineEditID->text().trimmed());
}


void EditObjectDialog::on_toolButtonGoals_clicked()
{
	// the Goals dialog is modal, i.e. it can stay floating around
	GoalEditor* theGoalEditorPtr = new GoalEditor(theBOPtr->theWorldPtr, QApplication::activeWindow());
	theGoalEditorPtr->show();
}



void EditObjectDialog::position_valueChanged (double )
{
	if (theUndoPtr == NULL)
		theUndoPtr = new UndoManualCommand(theBOPtr);
	theUndoPtr->setNewVal(
			Position(ui.spinBoxX->value(),
				ui.spinBoxY->value(),
				ui.spinBoxAngle->value() ) );
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
	// FIXME/TODO: put this into an UNDO
	theBOPtr->theProps.setProperty(myKey, myValue);
	// and run object's parseProperties to make sure the sim is updated
	theBOPtr->parseProperties();
}


void EditObjectDialog::readFromObject(BaseObject* aBaseObjectPtr)
{
	// prevent spawning of signals for every update we do below
	// connect everything back up at the end
	disconnect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	disconnect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	disconnect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.lineEditID,    SIGNAL(editingFinished()),    this, SLOT(lineEditID_valueChanged() ));
	disconnect(ui.tableWidget,   SIGNAL(cellChanged(int,int)), this, SLOT(propertyCellChanged(int,int)));

	// if we just changed the base object and there's still an undo pointer
	// around, we want to push this one to the stack...
	if (aBaseObjectPtr!=theBOPtr && theUndoPtr!=NULL)
	{
		theUndoPtr->pushYourself();
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
			QString myValue = aBaseObjectPtr->theProps.getDefaultProperty(*myI);
			aBaseObjectPtr->theProps.property2String(*myI, &myValue);
			QTableWidgetItem* myKeyItem = new QTableWidgetItem(myKey);
			ui.tableWidget->setVerticalHeaderItem(myRow, myKeyItem);
			QTableWidgetItem* myValueItem = new QTableWidgetItem(myValue);
			myValueItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
			ui.tableWidget->setItem(myRow, 0, myValueItem);

			myRow++;
			myI++;
		}
		ui.tableWidget->resizeColumnToContents(0);
	}

	connect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	connect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
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

void EditObjectDialog::widthHeight_valueChanged (double )
{
	if (theUndoPtr == NULL)
		theUndoPtr = new UndoManualCommand(theBOPtr);
	theUndoPtr->setNewVal(
			ui.spinBoxWidth->value(),
			ui.spinBoxHeight->value() );
}
