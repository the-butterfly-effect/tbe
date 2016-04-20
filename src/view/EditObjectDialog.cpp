/* The Butterfly Effect
 * This file copyright (C) 2010,2013,2014,2016 Klaas van Gend
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

#include "AbstractObject.h"
#include "EditObjectDialog.h"
#include "ViewObject.h"
#include "ImageCache.h"
#include "EditPropertyUndoCommand.h"
#include "MoveUndoCommand.h"
#include "ResizeUndoCommand.h"
#include "RotateUndoCommand.h"
#include "UndoSingleton.h"

// Initializer for theClosePreventer global static variable.
std::atomic<bool> EditObjectDialog::PreventClose::theClosePreventer(false);


EditObjectDialog::EditObjectDialog(QWidget *aParent)
        : QDialog(aParent, Qt::Tool), theMUCPtr(nullptr),
          theRszUCPtr(nullptr), theRotUCPtr(nullptr)
{
    DEBUG1ENTRY;
    ui.setupUi(this);
}

EditObjectDialog::~EditObjectDialog()
{
}


void EditObjectDialog::angle_editingFinished()
{
    PreventClose myLocalDoNotClose;
    if (nullptr != theRotUCPtr)
    {
        theRotUCPtr->editAngleDone(ui.spinBoxAngle->value());
    }
    theRotUCPtr = nullptr;
}

void EditObjectDialog::angle_valueChanged(double)
{
    if (nullptr == theRotUCPtr)
    {
        // get rid of the PieMenu icons or everything falls to pieces
        PieMenuSingleton::clearPieMenu();
        ViewObjectPtr myVOPtr = getAORealPtr()->theViewObjectPtr;
        closeExistingUndos();
        PreventClose myLocalDoNotClose;
        theRotUCPtr = (RotateUndoCommand*)UndoSingleton::createUndoCommand(myVOPtr,
                                                        ActionIcon::ACTION_ROTATE);
    }
    theRotUCPtr->editAngleMove(ui.spinBoxAngle->value());
}


void EditObjectDialog::closeExistingUndos()
{
    angle_editingFinished();
    position_editingFinished();
    size_editingFinished();
}

void EditObjectDialog::lineEditID_valueChanged ( void )
{
    // TODO/FIXME: Code duplication warning: this member is rather close
    // to the propertyCellChanged() member.
    ViewObjectPtr myVOPtr = getAORealPtr()->theViewObjectPtr;
    closeExistingUndos();
    PreventClose myLocalDoNotClose;
    EditPropertyUndoCommand* myUndoPtr =
            (EditPropertyUndoCommand*)UndoSingleton::createUndoCommand(myVOPtr,
                                            ActionIcon::ACTION_EDITPROPERTIES);
    myUndoPtr->changedID(getAORealPtr()->theID, ui.lineEditID->text());

    // commit this undo and make sure its changes are 'permanent'
    myUndoPtr->mouseReleaseEvent(nullptr);
}

void EditObjectDialog::position_editingFinished()
{
    PreventClose myLocalDoNotClose;
    if (theMUCPtr)
        theMUCPtr->basicReleaseEvent();
    // todo: figure out if this is a memory leak (probably not)
    theMUCPtr=nullptr;
}

void EditObjectDialog::position_valueChanged (double )
{
    if (nullptr == theMUCPtr)
    {
        // get rid of the PieMenu icons or everything falls to pieces
        PieMenuSingleton::clearPieMenu();
        ViewObjectPtr myVOPtr = getAORealPtr()->theViewObjectPtr;
        closeExistingUndos();
        PreventClose myLocalDoNotClose;
        theMUCPtr = (MoveUndoCommand*)UndoSingleton::createUndoCommand(myVOPtr,
                                                     ActionIcon::ACTION_MOVE);
        theMUCPtr->basicPressEvent(false, THESCALE*QPointF(ui.spinBoxX->value(),
                                                    -ui.spinBoxY->value()));
    }
    else
    {
        PreventClose myLocalDoNotClose;
        theMUCPtr->basicMoveEvent(THESCALE*QPointF(ui.spinBoxX->value(),
                                                   -ui.spinBoxY->value()));
    }
}


void EditObjectDialog::propertyCellChanged ( int aRow, int aColumn )
{
	// retrieve corresponding key and value
	QString myKey   = ui.tableWidget->verticalHeaderItem(aRow)->text();
	QString myValue = ui.tableWidget->item(aRow, aColumn)->text().trimmed();
	// is the value changed?
	QString myPropValue;
    if (!theAOPtr.expired())
        getAORealPtr()->theProps.property2String(myKey, &myPropValue);
	if (myValue == myPropValue)
		return;

    // If we get here, yes it changed.
    // Let's act on it!
    ViewObjectPtr myVOPtr = getAORealPtr()->theViewObjectPtr;
    closeExistingUndos();
    PreventClose myLocalDoNotClose;
    EditPropertyUndoCommand* myUndoPtr =
            (EditPropertyUndoCommand*)UndoSingleton::createUndoCommand(myVOPtr,
                                            ActionIcon::ACTION_EDITPROPERTIES);
    QString myOrigValue;
    theObjectProps.property2String(myKey, &myOrigValue);
    myUndoPtr->changedProperty(myKey, myOrigValue, myValue);

    // commit this undo and make sure its changes are 'permanent'
    myUndoPtr->mouseReleaseEvent(nullptr);
}


void EditObjectDialog::size_editingFinished()
{
    PreventClose myLocalDoNotClose;
    if (nullptr != theRszUCPtr)
    {
        theRszUCPtr->mouseReleaseEvent(nullptr);
    }
    theRszUCPtr = nullptr;
}

void EditObjectDialog::size_valueChanged(double)
{
    if (nullptr == theRszUCPtr)
    {
        // get rid of the PieMenu icons or everything falls to pieces
        PieMenuSingleton::clearPieMenu();
        ViewObjectPtr myVOPtr = getAORealPtr()->theViewObjectPtr;
        closeExistingUndos();
        theRszUCPtr = (ResizeUndoCommand*)UndoSingleton::createUndoCommand(myVOPtr,
                                                        ActionIcon::ACTION_RESIZE);
    }
    PreventClose myLocalDoNotClose;
    theRszUCPtr->basicMoveEvent(getAORealPtr()->getOrigCenter(),
                                ui.spinBoxWidth->value(),
                                ui.spinBoxHeight->value());
}


void EditObjectDialog::updateAbstractObjectPtr(AbstractObjectPtr anAbstractObjectPtr)
{
    // This member gets called whenever the anyone calls ViewObject::setNewGeometry.
    // That includes when it gets called from (Abstract|Move|Resize|Rotate)UndoCommand.
    // In the case of a user manipulating the ViewObject by mouse, that's fine
    //  - if there's an undo object open in our UI, it should be closed.
    // If the user manipulated our UI, we should not close the undo...
    if (!PreventClose::isClosePrevented())
        closeExistingUndos();

    // prevent spawning of signals for every update we do below
    // connect everything back up at the end
    disconnect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(angle_valueChanged(double)));
    disconnect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(size_valueChanged(double)));
    disconnect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(size_valueChanged(double)));
    disconnect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));
    disconnect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double)));

    disconnect(ui.spinBoxAngle,  SIGNAL(editingFinished()), this, SLOT(angle_editingFinished()) );
    disconnect(ui.spinBoxHeight, SIGNAL(editingFinished()), this, SLOT(size_editingFinished()) );
    disconnect(ui.spinBoxWidth,  SIGNAL(editingFinished()), this, SLOT(size_editingFinished()) );
    disconnect(ui.spinBoxX,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
    disconnect(ui.spinBoxY,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );

    disconnect(ui.lineEditID,    SIGNAL(editingFinished()),    this, SLOT(lineEditID_valueChanged() ));
    disconnect(ui.tableWidget,   SIGNAL(cellChanged(int,int)), this, SLOT(propertyCellChanged(int,int)));

    theAOPtr = anAbstractObjectPtr;
    if (!theAOPtr.expired())
    {
        AbstractObject* myAORealPtr = anAbstractObjectPtr.get();
        ui.lineEditID->setText(myAORealPtr->getID());
        ui.groupBox->setTitle(tr("Basic Properties for '%1'").arg(myAORealPtr->getName()));

		//  TODO: QLabel *labelUnique;

        ui.spinBoxX     -> setValue(myAORealPtr->getOrigCenter().x);
        ui.spinBoxY     -> setValue(myAORealPtr->getOrigCenter().y);
        ui.spinBoxAngle -> setValue(myAORealPtr->getOrigCenter().angle);
        ui.spinBoxWidth -> setValue(myAORealPtr->getTheWidth());
        ui.spinBoxHeight-> setValue(myAORealPtr->getTheHeight());

        QStringList myAllPropertiesList = myAORealPtr->theProps.getDefaultPropertyList();
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
            myAORealPtr->theProps.property2String(*myI, &myValue);
            theObjectProps.setProperty(myKey, myValue);

            QTableWidgetItem* myKeyItem = new QTableWidgetItem(myKey);
			ui.tableWidget->setVerticalHeaderItem(myRow, myKeyItem);
			QTableWidgetItem* myValueItem = new QTableWidgetItem(myValue);
			myValueItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
            if (myValue != myAORealPtr->theProps.getDefaultProperty(*myI))
                myValueItem->setIcon(ImageCache::getQIcon("IconModified", QSize(32,32)));
			ui.tableWidget->setItem(myRow, 0, myValueItem);

			myRow++;
			myI++;
		}
		ui.tableWidget->resizeColumnToContents(0);
	}

    connect(ui.spinBoxAngle,  SIGNAL(editingFinished()), this, SLOT(angle_editingFinished()) );
    connect(ui.spinBoxHeight, SIGNAL(editingFinished()), this, SLOT(size_editingFinished()) );
    connect(ui.spinBoxWidth,  SIGNAL(editingFinished()), this, SLOT(size_editingFinished()) );
	connect(ui.spinBoxX,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );
	connect(ui.spinBoxY,      SIGNAL(editingFinished()), this, SLOT(position_editingFinished()) );

    connect(ui.spinBoxAngle,  SIGNAL(valueChanged(double)), this, SLOT(angle_valueChanged(double) ));
    connect(ui.spinBoxHeight, SIGNAL(valueChanged(double)), this, SLOT(size_valueChanged(double)));
    connect(ui.spinBoxWidth,  SIGNAL(valueChanged(double)), this, SLOT(size_valueChanged(double)));
	connect(ui.spinBoxX,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));
	connect(ui.spinBoxY,      SIGNAL(valueChanged(double)), this, SLOT(position_valueChanged(double) ));

	connect(ui.lineEditID,    SIGNAL(editingFinished()),    this, SLOT(lineEditID_valueChanged() ));
	connect(ui.tableWidget,   SIGNAL(cellChanged(int,int)), this, SLOT(propertyCellChanged(int,int)));

    if (theAOPtr.expired())
		setEnabled(false);
	else
		setEnabled(true);
    return;
}

