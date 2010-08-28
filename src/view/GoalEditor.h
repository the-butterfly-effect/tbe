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

#ifndef GOALEDITOR_H
#define GOALEDITOR_H

#include "ui_GoalEditor.h"
#include <QDialog>
#include <QItemDelegate>

// forward declarations
class World;
class QStandardItemModel;


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

	enum GoalTypes
	{
		POSITIONX = 4,
		POSITIONY = 8,
		ANGLE     = 16,
		ANYTHING  = 4+8+16,
		DISTANCE  = 32
	};

	/// @returns a sparsely populated QStringList with all possible
	/// entries for column zero. They follow the enum GoalTypes.
	static QStringList getColumnZero(void);

protected:
    void changeEvent(QEvent *e);

	void populate(void);

private slots:
	void on_toolButtonMinus_clicked();
	void on_toolButtonPlus_clicked();

private:
	Ui::GoalEditor ui;
	QStandardItemModel* theModel;

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

	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option, const QModelIndex &index) const;

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
	DoubleSpinBoxDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // GOALEDITOR_H
