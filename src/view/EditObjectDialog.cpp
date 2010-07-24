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


void EditObjectDialog::position_valueChanged (double )
{
	if (theUndoPtr == NULL)
		theUndoPtr = new UndoManualCommand(theBOPtr);
	theUndoPtr->setNewVal(
			Position(ui.spinBoxX->value(),
				ui.spinBoxY->value(),
				ui.spinBoxAngle->value() ) );
}

void EditObjectDialog::readFromObject(BaseObject* aBaseObjectPtr)
{
	disconnect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	disconnect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	disconnect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
	disconnect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));

	// if we just changed the base object and there's still an undo pointer
	// around, we want to push this one to the stack...
	if (aBaseObjectPtr!=theBOPtr && theUndoPtr!=NULL)
	{
		theUndoPtr->pushYourself();
		theUndoPtr = NULL;
	}

	if (aBaseObjectPtr==NULL)
		goto theEnd;

	ui.lineEditID->setText(aBaseObjectPtr->getID());
	ui.groupBox->setTitle(tr("Basic Properties for %1").arg(aBaseObjectPtr->getName()));
	//  TODO: QLabel *labelUnique;

	ui.spinBoxX     -> setValue(aBaseObjectPtr->getOrigCenter().x);
	ui.spinBoxY     -> setValue(aBaseObjectPtr->getOrigCenter().y);
	ui.spinBoxAngle -> setValue(aBaseObjectPtr->getOrigCenter().angle);
	ui.spinBoxWidth -> setValue(aBaseObjectPtr->getTheWidth());
	ui.spinBoxHeight-> setValue(aBaseObjectPtr->getTheHeight());

	//
	//	QTreeWidget *treeWidget;
	if (aBaseObjectPtr!=theBOPtr)
	{
		ui.treeWidget->clear();
		QStringList myAllPropertiesList = aBaseObjectPtr->theProps.getDefaultPropertyList();
		QStringList::iterator myI= myAllPropertiesList.begin();
		while (myI != myAllPropertiesList.end())
		{
			QString myKey = *myI;
			QString myValue = aBaseObjectPtr->theProps.getDefaultProperty(*myI);
			aBaseObjectPtr->theProps.propertyToString(*myI, &myValue);
			QTreeWidgetItem* myLine = new QTreeWidgetItem(ui.treeWidget);
			myLine->setText(0, myKey);
			myLine->setText(1, myValue);
			myI++;
		}
	}

	connect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	connect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(widthHeight_valueChanged(double)));
	connect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));

theEnd:
	if (aBaseObjectPtr==NULL)
		setEnabled(false);
	else
		setEnabled(true);
	theBOPtr = aBaseObjectPtr;
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
