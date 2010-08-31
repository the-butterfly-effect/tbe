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
	ComboBoxDelegate* myVariableDelegate = new ComboBoxDelegate();
	myVariableDelegate->setItems(GoalSerializer::getColumnZero());
	ui.tableView->setItemDelegateForColumn ( 0, myVariableDelegate);

	ComboBoxDelegate* myConditionDelegate = new ComboBoxDelegate();
	myConditionDelegate->setItems(QString(">;<;"+getT10nOf_change()).split(";"));
	ui.tableView->setItemDelegateForColumn ( 2, myConditionDelegate);
	ui.tableView->setItemDelegateForColumn ( 3, new DoubleSpinBoxDelegate());

	ComboBoxDelegate* myObjectIDDelegate = new ComboBoxDelegate();
	myObjectIDDelegate->setItems(theWorldPtr->getAllIDs());
	ui.tableView->setItemDelegateForColumn ( 1, myObjectIDDelegate);
	ui.tableView->setItemDelegateForColumn ( 4, myObjectIDDelegate);

	connect(theModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_itemChanged(QStandardItem*)));
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

void GoalEditor::fixupCellColoring(int aRow)
{
	// highlight cells as red if object name doesn't exist
	if (theWorldPtr->findObjectByID(theModel->item(aRow, 1)->text())==NULL)
		theModel->item(aRow, 1)->setForeground(QBrush(Qt::red));
	else
		theModel->item(aRow, 1)->setForeground(QBrush(Qt::black));
	if (theWorldPtr->findObjectByID(theModel->item(aRow, 4)->text())==NULL)
		theModel->item(aRow, 4)->setForeground(Qt::red);
	else
		theModel->item(aRow, 4)->setForeground(Qt::black);

	// grey out cells if contents cannot exist
	if (theModel->item(aRow,2)->text()==getT10nOf_change())
	{
		theModel->item(aRow, 3)->setBackground(Qt::gray);
		theModel->item(aRow, 3)->setText("");
		theModel->item(aRow, 4)->setBackground(Qt::gray);
		theModel->item(aRow, 4)->setText("");
	}
	else
	{
		theModel->item(aRow, 3)->setBackground(Qt::white);

		// Position doesn't need field 4
		if (GoalSerializer::getColumnZero().indexOf(theModel->item(aRow,0)->text())
				 < GoalSerializer::DISTANCE)
		{
			theModel->item(aRow, 4)->setBackground(Qt::gray);
			theModel->item(aRow, 4)->setText("");
		}
		else
			// otherwise need all fields
			theModel->item(aRow, 4)->setBackground(Qt::white);
	}

}


void GoalEditor::on_toolButtonMinus_clicked()
{
	if (theModel->rowCount()==0)
		return;
	int myLine = ui.tableView->currentIndex().row();

	QString myRowString = rowToString(myLine, ' ');

	//: translator, be careful not to translate the %'s and the <br>'s...
	if (Popup::YesNoQuestion(tr("Are you sure you want to remove goal %1:"
							"<br>%2").arg(myLine+1)
			.arg(myRowString.replace('<',"&lt;")),
			this) == true)
	{
		theModel->removeRow(myLine);
	}
}

void GoalEditor::on_toolButtonPlus_clicked()
{
	QList<QStandardItem*> myList;
	myList.push_back(new QStandardItem(""));
	myList.push_back(new QStandardItem(tr("no object")));
	myList.push_back(new QStandardItem(">"));
	myList.push_back(new QStandardItem("0.0"));
	myList.push_back(new QStandardItem(tr("no object")));
	theModel->appendRow(myList);
	fixupCellColoring(theModel->rowCount()-1);
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

		assert(myGoal.size()==5);
		for (int i=0; i<5; i++)
		{
			QStandardItem* myItemPtr = new QStandardItem(myGoal[i]);
			myRowList.push_back(myItemPtr);
		}
		theModel->appendRow(myRowList);
		fixupCellColoring(myRow);

		++myG;
		myRow++;
	}

}

QString GoalEditor::rowToString(int aRow, char aSeparator) const
{
	if (theModel->rowCount()<aRow)
		return "";
	return QString("%1%2%3%4%5%6%7%8%9")
			.arg(theModel->item(aRow,0)->text()).arg(aSeparator)
			.arg(theModel->item(aRow,1)->text()).arg(aSeparator)
			.arg(theModel->item(aRow,2)->text()).arg(aSeparator)
			.arg(theModel->item(aRow,3)->text()).arg(aSeparator)
			.arg(theModel->item(aRow,4)->text());
}

void GoalEditor::slot_itemChanged(QStandardItem* anItem)
{
	int myRow = anItem->row();

	// the X/Y/Angle cell only accepts 'change' as field 2
	if (theModel->item(myRow,0)->text()==GoalSerializer::getColumnZero()[GoalSerializer::ANYTHING])
	{
		DEBUG2("Goal type Position X/Y/Angle + '>' or '<' cannot exist -> fixing...\n");
		theModel->item(myRow,2)->setText(getT10nOf_change());
	}

	// Distance and 'change' cannot work together
	if (theModel->item(myRow,0)->text()==GoalSerializer::getColumnZero()[GoalSerializer::DISTANCE]
		&& theModel->item(myRow,2)->text()==getT10nOf_change())
	{
		DEBUG2("Goal type Distance + 'change' cannot exist -> fixing...\n");
		theModel->item(myRow,2)->setText(">");
	}

	// we need to ensure that the row is still correctly greyed out / turned red...
	fixupCellColoring(myRow);
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
