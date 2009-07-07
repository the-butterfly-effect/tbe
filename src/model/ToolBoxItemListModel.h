/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#ifndef TOOLBOXITEMLISTMODEL_H
#define TOOLBOXITEMLISTMODEL_H

#include "tbe_global.h"

#include <QPixmap>
#include <QString>
#include <QList>
#include <QAbstractListModel>


/// this class contains everything one needs to know
/// to add an item to the toolbox
class ToolBoxItem
{
public:

	/// Qt::DecorationRole - the icon to show
	QPixmap theIcon;

	/// Qt::DisplayRole - the Name of the object
	QString	theName;

	/// Qt::ToolTipRole - the tooltip
	QString	theTooltip;

	/// Qt::EditRole  - the number of objects left
	int		theCount;

	/// TODO: add pointer to actual factory
};


/// this class contains all items in the toolbox
class ToolBoxItemListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	ToolBoxItemListModel(QObject *parent = 0);

	bool fillFromObjectFactory(void);

	/// required implementation from QAbstractListModel
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	/// required implementation from QAbstractListModel
	virtual QVariant data(const QModelIndex &index, int role) const;

	/// required implementation from QAbstractListModel
	virtual QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const;

private:
//	typedef QList<ToolBoxItem*> ToolBoxItemPtrList;
//	ToolBoxItemPtrList theList;
	QStringList	theList;
};

#endif // TOOLBOXITEMLISTMODEL_H
