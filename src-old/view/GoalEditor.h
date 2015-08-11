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

#ifndef GOALEDITOR_H
#define GOALEDITOR_H

#include "ui_GoalEditor.h"
#include <QDialog>
#include <QItemDelegate>

// forward declarations
class Goal;
class World;
class QStandardItemModel;
class QStandardItem;


// this header file contains three class declarations:
// first GoalEditor, then ComboBoxDelegate, then DoubleSpinBoxDelegate


/// the GoalEditor dialog pops up a QTableView with rows that each represent
/// a Goal.
/// Each column has a dedicated delegate to edit the contents of that column.
class GoalEditor : public QDialog {
    Q_OBJECT
public:
	GoalEditor(World* aWorldPtr, QWidget *parent = 0);
    ~GoalEditor();

	static QString getT10nOf_change(void)
	{ return tr("change"); }

protected:
    void changeEvent(QEvent *e);

	/** depending on the contents of the cells,
	  * turns cells red (if object names do not exist) or greys/ungreys cells
	  * @param aModel  the QStandardItemModel to do this for
	  * @param aRow to fix the coloring for.
	  */
	void fixupCellColoring(QStandardItemModel* aModel, int aRow);

	void populate(void);

	/** retrieves the input from the fields of row aRow and
	  * collides them to a string - with separator aSeparator
	  * @param aModel  the QStandardItemModel to do this for
	  * @param aRow  row identifier
	  * @param aSeparator character to separate the fields
	  *                   probably only ' ' and ';' will be used
	  * @return the concatenated string
	  */
	QString rowToString(const QStandardItemModel* aModel, int aRow, char aSeparator) const;

	/** Sets up the tableView and corresponding QStandardItemModel
	  * @param aModelPtrPTr double pointer to the QStandardItemModel to initialise
	  * @param aViewPtr view to initialise and assign aModelPtrPtr to
	  */
	void setupViewAndModel(QTableView* aViewPtr, QStandardItemModel** aModelPtrPtr);

	typedef QList<Goal*> GoalPtrList;
	/** creates a Goal from line i in aModel and adds it to aList
	  * @param aList     list of pointers to Goals
	  * @param i         line number of aModel to handle
	  * @param aModel    QSIModel to handle
	  * @param anIsFail  true if this is a Fail, false if this is a Goal
	  */
	bool addNewGoalToList(GoalPtrList& aList, int i, const QStandardItemModel& aModel, bool anIsFail);


public slots:
	/** Overridden from QDialog to be able to save the Goals
	  * when the user closes the dialog and prevent the
	  * dialog from closing when the Goals cannot be saved due to errors.
	  */
	virtual void accept();

private slots:
	void on_toolButtonFailsMinus_clicked();
	void on_toolButtonFailsPlus_clicked();

	void on_toolButtonGoalsMinus_clicked();
	void on_toolButtonGoalsPlus_clicked();

	void slot_itemChanged(QStandardItem* anItem);

private:
	Ui::GoalEditor ui;
	QStandardItemModel* theGoalsModelPtr;
	QStandardItemModel* theFailsModelPtr;

	World* theWorldPtr;
};


// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

/// this is the Delegate to edit a field using a ComboBox with predefined values
class ComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboBoxDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;

	void setItems(const QStringList);

	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;

private:
	QStringList theList;
};


// --------------------------------------------------------------------------
// --------------------------------------------------------------------------


/// this is the Delegate to edit a field using a QDoubleSpinBox
class DoubleSpinBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	DoubleSpinBoxDelegate(QObject *parent, float aMin, float aMax);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	float theMax;
	float theMin;
};


#endif // GOALEDITOR_H
