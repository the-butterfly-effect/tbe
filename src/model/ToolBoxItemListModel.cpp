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

#include "ToolBoxItemListModel.h"
#include "ImageStore.h"

ToolBoxItemListModel::ToolBoxItemListModel(QObject *parent)
		: QAbstractListModel(parent)
{
	theList.push_back("Aap");
	theList.push_back("Noot");
	theList.push_back("Mies");
	theList.push_back("Wim");
	theList.push_back("Zus");
	theList.push_back("Jet");
}



 QVariant ToolBoxItemListModel::data(const QModelIndex &index, int role) const
 {
	if (!index.isValid())
		return QVariant();

	if (index.row() >= theList.size())
		return QVariant();

	if (role == Qt::DisplayRole)
		return theList.at(index.row());
	if (role == Qt::DecorationRole)
		return ImageStore::getQIcon("ActionDelete", QSize(32,32));
	return QVariant();
}


Qt::ItemFlags ToolBoxItemListModel::flags(const QModelIndex &index) const
{
	if (index.isValid())
		return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

	return Qt::ItemIsDropEnabled;
}


QVariant ToolBoxItemListModel::headerData(int section, Qt::Orientation orientation,
								  int role) const
{
//	if (role == Qt::DecorationRole)
//		return ImageStore::getQIcon("ActionDelete", QSize(32,32));
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
		return QString("Column %1").arg(section);
	else
		return QString("Row %1").arg(section);
}


bool ToolBoxItemListModel::removeRows(int row, int count, const QModelIndex &parent)
{
	// TODO: NOT IMPLEMENTED YET
	return false;
}


int ToolBoxItemListModel::rowCount(const QModelIndex &parent) const
{
	return theList.count();
}
