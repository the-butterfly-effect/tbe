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

#include "GoalEditor.h"
#include "Goal.h"
#include "GoalSerializer.h"
#include "Popup.h"
#include "World.h"
#include <QtGui>
#include <QComboBox>
#include <QDoubleSpinBox>


// this source file contains member definitions for three classes:
// first GoalEditor, then ComboBoxDelegate, then DoubleSpinBoxDelegate


GoalEditor::GoalEditor(World* aWorldPtr, QWidget *parent) :
	QDialog(parent),
	theWorldPtr(aWorldPtr)
{
	ui.setupUi(this);

	// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	theModel = new QStandardItemModel(4, 1);

	// TODO/FIXME: why don't the headers below work???
	//: translators: Cond. is short for Condition - otherwise it doesn't fit
	theModel->setHorizontalHeaderLabels(QString("Variable;Object;Cond.;Value;Object2").split(";"));

	ui.tableView->setModel(theModel);

	// setup the various delegates for the different columns
	// TODO: the Combo delegate for the first column
	ComboBoxDelegate* myConditionDelegate = new ComboBoxDelegate();
	myConditionDelegate->setItems(QString(">;<").split(";"));
	ui.tableView->setItemDelegateForColumn ( 2, myConditionDelegate);
	ui.tableView->setItemDelegateForColumn ( 3, new DoubleSpinBoxDelegate());
	ComboBoxDelegate* myObjectIDDelegate = new ComboBoxDelegate();
	myObjectIDDelegate->setItems(theWorldPtr->getAllIDs());
	ui.tableView->setItemDelegateForColumn ( 1, myObjectIDDelegate);
	ui.tableView->setItemDelegateForColumn ( 4, myObjectIDDelegate);
	populate();
}

GoalEditor::~GoalEditor()
{
//	delete ui;
	delete theModel;
}

void GoalEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
		ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void GoalEditor::on_toolButtonMinus_clicked()
{
	if (theModel->rowCount()==0)
		return;
	int myLine = ui.tableView->currentIndex().row();
	QList<QStandardItem*> myList;
	myList.push_back(theModel->item(myLine,0));
	myList.push_back(theModel->item(myLine,1));
	myList.push_back(theModel->item(myLine,2));
	myList.push_back(theModel->item(myLine,3));
	myList.push_back(theModel->item(myLine,4));

	//: translator, be careful not to translate the %'s and the <br>'s...
	if (Popup::YesNoQuestion(tr("Are you sure you want to remove goal %1:"
							"<br>%2 %3 %4 %5").arg(myLine+1)
			.arg(myList[0]->text()).arg(myList[1]->text())
			.arg(myList[2]->text()).arg(myList[3]->text())
			.arg(myList[4]->text()),
			this) == true)
	{
		theModel->removeRow(myLine);
	}
}

void GoalEditor::on_toolButtonPlus_clicked()
{
	// FIXME/TODO: still to do...
}


void GoalEditor::populate(void)
{
	assert(theWorldPtr!=NULL);

	theModel->clear();

	int myRow = 0;
	World::GoalPtrList::const_iterator myG = theWorldPtr->theGoalPtrList.begin();
	while (myG != theWorldPtr->theGoalPtrList.end())
	{
		// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
		QStringList myGoal = GoalSerializer::goalToStringList(*myG);
		QList<QStandardItem*> myRowList;

		for (int i=0; i<5; i++)
		{
			if (myGoal[i].isEmpty()==false)
			{
				QStandardItem* myItemPtr = new QStandardItem(myGoal[i]);
				myRowList.push_back(myItemPtr);
				// fields 1 and 4 are object names - make them red if trouble
				if (i==1 || i==4)
				{
					if (theWorldPtr->findObjectByID(myGoal[i])==NULL)
						myItemPtr->setForeground(QBrush(Qt::red));
				}
			}
			else
			{
				QStandardItem* myItemPtr = new QStandardItem("");
				myItemPtr->setBackground(QBrush(QColor(Qt::gray)));
				myRowList.push_back(myItemPtr);
			}
		}
		theModel->appendRow(myRowList);

		++myG;
		myRow++;
	}

}



// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
	 : QItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
			const QStyleOptionViewItem &/* option */,
			const QModelIndex &/* index */) const
{
	QComboBox *myEditorPtr = new QComboBox(parent);
	myEditorPtr->addItems(theList);
	return myEditorPtr;
}

void ComboBoxDelegate::setEditorData(QWidget *anEditor,
				 const QModelIndex &index) const
{
	QString myValue = index.model()->data(index, Qt::EditRole).toString();

	QComboBox* myComboBoxPtr = static_cast<QComboBox*>(anEditor);
	myComboBoxPtr->setCurrentIndex(myComboBoxPtr->findText(myValue));
}


void ComboBoxDelegate::setItems(const QStringList aList)
{
	theList = aList;
}


void ComboBoxDelegate::setModelData(QWidget *anEditor, QAbstractItemModel *model,
				const QModelIndex &index) const
{
	QComboBox* myComboBoxPtr = static_cast<QComboBox*>(anEditor);
	QString myValue = myComboBoxPtr->currentText();
	model->setData(index, myValue, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}



// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent)
	 : QItemDelegate(parent)
{
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
			const QStyleOptionViewItem &/* option */,
			const QModelIndex &/* index */) const
{
	QDoubleSpinBox *myEditor = new QDoubleSpinBox(parent);
	myEditor->setMinimum(-10.0);
	myEditor->setMaximum(50.0);
	myEditor->setDecimals(2);
	myEditor->setSingleStep(0.1);
	return myEditor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
				 const QModelIndex &index) const
{
	double value = index.model()->data(index, Qt::EditRole).toDouble();

	QDoubleSpinBox *mySpinBoxPtr = static_cast<QDoubleSpinBox*>(editor);
	mySpinBoxPtr->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
				const QModelIndex &index) const
{
	QDoubleSpinBox *mySpinBoxPtr = static_cast<QDoubleSpinBox*>(editor);
	mySpinBoxPtr->interpretText();
	double value = mySpinBoxPtr->value();

	model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *anEditor,
const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	anEditor->setGeometry(option.rect);
}
