/* The Butterfly Effect
 * This file copyright (C) 2010,2011  Klaas van Gend
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


GoalEditor::GoalEditor(World *aWorldPtr, QWidget *parent) :
    QDialog(parent),
    theWorldPtr(aWorldPtr)
{
    ui.setupUi(this);

    setupViewAndModel(ui.tableViewGoals, &theGoalsModelPtr);
    setupViewAndModel(ui.tableViewFails, &theFailsModelPtr);

    connect(theGoalsModelPtr, SIGNAL(itemChanged(QStandardItem *)), this,
            SLOT(slot_itemChanged(QStandardItem *)));
    connect(theFailsModelPtr, SIGNAL(itemChanged(QStandardItem *)), this,
            SLOT(slot_itemChanged(QStandardItem *)));
    populate();
}

GoalEditor::~GoalEditor()
{
//  delete ui;
    delete theGoalsModelPtr;
    delete theFailsModelPtr;
}


void GoalEditor::accept()
{

    // first, we're going to use the serializer to create the new Goals
    // only when we've been able to create all the Goals classes, we will
    // discard the old ones and put the new ones to work.

    GoalPtrList theGoalPtrList;

    for (int i = 0; i < theFailsModelPtr->rowCount(); i++)
        addNewGoalToList(theGoalPtrList, i, *theFailsModelPtr, true);
    for (int i = 0; i < theGoalsModelPtr->rowCount(); i++)
        addNewGoalToList(theGoalPtrList, i, *theGoalsModelPtr, false);

    // update the goals to the Worlds
    theWorldPtr->theGoalPtrList.clear();
    for (int i = 0; i < theGoalPtrList.size(); i++)
        theWorldPtr->addGoal(theGoalPtrList[i]);

    QDialog::accept();
}


bool GoalEditor::addNewGoalToList(GoalPtrList &aList, int i, const QStandardItemModel &aModel,
                                  bool anIsFail)
{
    QString myString = rowToString(&aModel, i, ';');
    Goal *myGoalPtr = GoalSerializer::createObjectFromString(theWorldPtr, myString);
    if (myGoalPtr != nullptr) {
        myGoalPtr->isFail = anIsFail;
        aList.push_back(myGoalPtr);
        return true;
    }
    delete myGoalPtr;
    Popup::Warning(tr("Not all goals/fails were OK\nNothing was changed yet, please fix."), this);
    return false;
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

void GoalEditor::fixupCellColoring(QStandardItemModel *aModel, int aRow)
{
    // highlight cells as red if object name doesn't exist
    if (theWorldPtr->findObjectsByID(aModel->item(aRow, 1)->text()).empty())
        aModel->item(aRow, 1)->setForeground(QBrush(Qt::red));
    else
        aModel->item(aRow, 1)->setForeground(QBrush(Qt::black));
    if (theWorldPtr->findObjectsByID(aModel->item(aRow, 4)->text()).empty())
        aModel->item(aRow, 4)->setForeground(Qt::red);
    else
        aModel->item(aRow, 4)->setForeground(Qt::black);

    // grey out cells if contents cannot exist
    if (aModel->item(aRow, 2)->text() == getT10nOf_change()) {
        aModel->item(aRow, 3)->setBackground(Qt::gray);
        aModel->item(aRow, 3)->setText("");
        aModel->item(aRow, 4)->setBackground(Qt::gray);
        aModel->item(aRow, 4)->setText("");
    } else {
        aModel->item(aRow, 3)->setBackground(Qt::white);

        // Position doesn't need field 4
        if (GoalSerializer::getColumnZero().indexOf(aModel->item(aRow, 0)->text())
                < GoalSerializer::DISTANCE) {
            aModel->item(aRow, 4)->setBackground(Qt::gray);
            aModel->item(aRow, 4)->setText("");
        } else
            // otherwise need all fields
            aModel->item(aRow, 4)->setBackground(Qt::white);
    }

}


void GoalEditor::on_toolButtonGoalsMinus_clicked()
{
    ui.tableViewGoals->setFocus(Qt::OtherFocusReason);
    if (theGoalsModelPtr->rowCount() == 0)
        return;
    if (!ui.tableViewGoals->currentIndex().isValid())
        return;
    int myLine = ui.tableViewGoals->currentIndex().row();

    QString myRowString = rowToString(theGoalsModelPtr, myLine, ' ');

    //: translator, be careful not to translate the %'s and the <br>'s...
    if (Popup::YesNoQuestion(tr("Are you sure you want to remove goal %1:"
                                "<br>%2").arg(myLine + 1)
                             .arg(myRowString.replace('<', "&lt;")),
                             this) == true) {
        theGoalsModelPtr->removeRow(myLine);
    }
}

void GoalEditor::on_toolButtonFailsMinus_clicked()
{
    ui.tableViewFails->setFocus(Qt::OtherFocusReason);
    if (theFailsModelPtr->rowCount() == 0)
        return;
    if (!ui.tableViewFails->currentIndex().isValid())
        return;
    int myLine = ui.tableViewFails->currentIndex().row();

    QString myRowString = rowToString(theFailsModelPtr, myLine, ' ');

    //: translator, be careful not to translate the %'s and the <br>'s...
    if (Popup::YesNoQuestion(tr("Are you sure you want to remove fail %1:"
                                "<br>%2").arg(myLine + 1)
                             .arg(myRowString.replace('<', "&lt;")),
                             this) == true) {
        theFailsModelPtr->removeRow(myLine);
    }
}

void GoalEditor::on_toolButtonGoalsPlus_clicked()
{
    // TODO: integrate Goals and Fails
    QList<QStandardItem *> myList;
    myList.push_back(new QStandardItem(""));
    myList.push_back(new QStandardItem(tr("no object")));
    myList.push_back(new QStandardItem(">"));
    myList.push_back(new QStandardItem("0.0"));
    myList.push_back(new QStandardItem(tr("no object")));
    theGoalsModelPtr->appendRow(myList);
    fixupCellColoring(theGoalsModelPtr, theGoalsModelPtr->rowCount() - 1);
}

void GoalEditor::on_toolButtonFailsPlus_clicked()
{
    // TODO: integrate Goals and Fails
    QList<QStandardItem *> myList;
    myList.push_back(new QStandardItem(""));
    myList.push_back(new QStandardItem(tr("no object")));
    myList.push_back(new QStandardItem(">"));
    myList.push_back(new QStandardItem("0.0"));
    myList.push_back(new QStandardItem(tr("no object")));
    theFailsModelPtr->appendRow(myList);
    fixupCellColoring(theFailsModelPtr, theFailsModelPtr->rowCount() - 1);
}


void GoalEditor::populate(void)
{
    assert(theWorldPtr != nullptr);

    theGoalsModelPtr->clear();
    theFailsModelPtr->clear();

    int myGoalsRow = 0;
    int myFailsRow = 0;

    World::GoalPtrList::const_iterator myG = theWorldPtr->theGoalPtrList.begin();
    while (myG != theWorldPtr->theGoalPtrList.end()) {
        // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
        QStringList myGoal = GoalSerializer::goalToStringList(*myG);
        QList<QStandardItem *> myRowList;

        assert(myGoal.size() == 5);
        for (int i = 0; i < 5; i++) {
            QStandardItem *myItemPtr = new QStandardItem(myGoal[i]);
            myRowList.push_back(myItemPtr);
        }

        if ( (*myG)->isFail == true) {
            theFailsModelPtr->appendRow(myRowList);
            fixupCellColoring(theFailsModelPtr, myFailsRow);
            myFailsRow++;
        } else {
            theGoalsModelPtr->appendRow(myRowList);
            fixupCellColoring(theGoalsModelPtr, myGoalsRow);
            myGoalsRow++;
        }

        ++myG;
    }

}


QString GoalEditor::rowToString(const QStandardItemModel *aModel, int aRow, char aSeparator) const
{
    if (aModel->rowCount() < aRow)
        return "";
    if (aRow < 0)
        return "";
    return QString("%1%2%3%4%5%6%7%8%9")
           .arg(aModel->item(aRow, 0)->text()).arg(aSeparator)
           .arg(aModel->item(aRow, 1)->text()).arg(aSeparator)
           .arg(aModel->item(aRow, 2)->text()).arg(aSeparator)
           .arg(aModel->item(aRow, 3)->text()).arg(aSeparator)
           .arg(aModel->item(aRow, 4)->text());
}

void GoalEditor::setupViewAndModel(QTableView *aViewPtr, QStandardItemModel **aModelPtrPtr)
{
    // Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
    (*aModelPtrPtr) = new QStandardItemModel(4, 1);

    // TODO/FIXME: why don't the headers below work???
    //: translators: Cond. is short for Condition - otherwise it doesn't fit
    (*aModelPtrPtr)->setHorizontalHeaderLabels(QString(
                                                   tr("Variable;Object;Cond.;Value;Object2")).split(";"));

    aViewPtr->setModel((*aModelPtrPtr));
    // setup the various delegates for the different columns
    ComboBoxDelegate *myVariableDelegate = new ComboBoxDelegate();
    myVariableDelegate->setItems(GoalSerializer::getColumnZero());
    aViewPtr->setItemDelegateForColumn ( 0, myVariableDelegate);

    ComboBoxDelegate *myConditionDelegate = new ComboBoxDelegate();
    myConditionDelegate->setItems(QString(">;<;>=;" + getT10nOf_change()).split(";"));
    aViewPtr->setItemDelegateForColumn ( 2, myConditionDelegate);
    aViewPtr->setItemDelegateForColumn ( 3, new DoubleSpinBoxDelegate(this, -10, 50));

    ComboBoxDelegate *myObjectIDDelegate = new ComboBoxDelegate();
    myObjectIDDelegate->setItems(theWorldPtr->getAllIDs());
    aViewPtr->setItemDelegateForColumn ( 1, myObjectIDDelegate);
    ui.tableViewGoals->setItemDelegateForColumn ( 4, myObjectIDDelegate);
}


void GoalEditor::slot_itemChanged(QStandardItem *anItem)
{
    int myRow = anItem->row();

    QStandardItemModel *myModelPtr = anItem->model();

    // the X/Y/Angle cell only accepts 'change' as field 2
    if (myModelPtr->item(myRow, 0)->text() ==
            GoalSerializer::getColumnZero()[GoalSerializer::ANYTHING]) {
        DEBUG2("Goal/Fail type Position X/Y/Angle + '>' or '<' cannot exist -> fixing...");
        myModelPtr->item(myRow, 2)->setText(getT10nOf_change());
    }

    // Distance and 'change' cannot work together
    if (myModelPtr->item(myRow, 0)->text() == GoalSerializer::getColumnZero()[GoalSerializer::DISTANCE]
            && myModelPtr->item(myRow, 2)->text() == getT10nOf_change()) {
        DEBUG2("Goal/Fail type Distance + 'change' cannot exist -> fixing...");
        myModelPtr->item(myRow, 2)->setText(">");
    }

    // we need to ensure that the row is still correctly greyed out / turned red...
    fixupCellColoring(myModelPtr, myRow);
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

    QComboBox *myComboBoxPtr = static_cast<QComboBox *>(anEditor);
    myComboBoxPtr->setCurrentIndex(myComboBoxPtr->findText(myValue));
}


void ComboBoxDelegate::setItems(const QStringList aList)
{
    theList = aList;
}


void ComboBoxDelegate::setModelData(QWidget *anEditor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QComboBox *myComboBoxPtr = static_cast<QComboBox *>(anEditor);
    QString myValue = myComboBoxPtr->currentText();
    model->setData(index, myValue, Qt::EditRole);
}

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent, float aMin, float aMax)
    : QItemDelegate(parent), theMax(aMax), theMin(aMin)
{
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &/* option */,
                                             const QModelIndex &/* index */) const
{
    QDoubleSpinBox *myEditor = new QDoubleSpinBox(parent);
    myEditor->setMinimum(theMin);
    myEditor->setMaximum(theMax);
    myEditor->setDecimals(2);
    myEditor->setSingleStep(0.1);
    return myEditor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *mySpinBoxPtr = static_cast<QDoubleSpinBox *>(editor);
    mySpinBoxPtr->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *mySpinBoxPtr = static_cast<QDoubleSpinBox *>(editor);
    mySpinBoxPtr->interpretText();
    double value = mySpinBoxPtr->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *anEditor,
                                                 const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    anEditor->setGeometry(option.rect);
}
