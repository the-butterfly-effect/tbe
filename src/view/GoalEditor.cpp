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

GoalEditor::GoalEditor(World* aWorldPtr, QWidget *parent) :
	QDialog(parent),
	theWorldPtr(aWorldPtr)
{
	ui.setupUi(this);
	populate();
}

GoalEditor::~GoalEditor()
{
//	delete ui;
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
	if (ui.tableWidget->rowCount()==0)
		return;
	int myLine = ui.tableWidget->currentRow();
	ui.tableWidget->setRangeSelected(
			QTableWidgetSelectionRange(myLine, 0, myLine, ui.tableWidget->columnCount()-1), true);
	QList<QTableWidgetItem *> myList = ui.tableWidget->selectedItems();

	//: translator, be careful not to translate the %'s and the <br>'s...
	if (Popup::YesNoQuestion(tr("Are you sure you want to remove goal %1:"
							"<br>%2 %3 %4 %5").arg(myLine+1)
			.arg(myList[0]->text()).arg(myList[1]->text())
			.arg(myList[2]->text()).arg(myList[3]->text())
			.arg(myList[4]->text()),
			this) == true)
	{
		ui.tableWidget->removeRow(myLine);
	}
}

void GoalEditor::on_toolButtonPlus_clicked()
{
	// FIXME/TODO: still to do...
}


void GoalEditor::populate(void)
{
	assert(theWorldPtr!=NULL);

	ui.tableWidget->clear();
	ui.tableWidget->setColumnCount(5);
	ui.tableWidget->setHorizontalHeaderLabels(
			tr("Variable;Object;Cond.;Value;Object2").split(";") );
	ui.tableWidget->setRowCount(theWorldPtr->theGoalPtrList.count());

	int myRow = 0;
	World::GoalPtrList::const_iterator myG = theWorldPtr->theGoalPtrList.begin();
	while (myG != theWorldPtr->theGoalPtrList.end())
	{
		// Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
		QStringList myGoal = GoalSerializer::goalToStringList(*myG);

		for (int i=0; i<5; i++)
		{
			if (myGoal[i].isEmpty()==false)
			{
				QTableWidgetItem* myItemPtr = new QTableWidgetItem(myGoal[i]);
				ui.tableWidget->setItem(myRow, i, myItemPtr);
				// fields 1 and 4 are object names - make them red if trouble
				if (i==1 || i==4)
				{
					if (theWorldPtr->findObjectByID(myGoal[i])==NULL)
						myItemPtr->setForeground(QBrush(Qt::red));
				}
			}
			else
			{
				QTableWidgetItem* myItemPtr = new QTableWidgetItem(" ");
				myItemPtr->setBackgroundColor(QColor(Qt::gray));
				ui.tableWidget->setItem(myRow, i, myItemPtr);
			}
		}

		++myG;
		myRow++;
	}

}
