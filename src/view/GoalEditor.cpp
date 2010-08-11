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

void GoalEditor::populate(void)
{
	assert(theWorldPtr!=NULL);

	ui.tableWidget->clear();
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
		}

		++myG;
		myRow++;
	}

}
