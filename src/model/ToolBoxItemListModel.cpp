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


ToolBoxItem::ToolBoxItem(
		unsigned int   aCount, const QIcon&   anIcon,
		const QString& aName,  const QString& aTooltip)
		: theCount(aCount), theIcon(anIcon),
		  theName(aName),   theTooltip(aTooltip)
{
	; // nothing to do here...
}


ToolBoxItemListModel::ToolBoxItemListModel(QObject *parent)
		: QAbstractListModel(parent)
{
	QIcon myIcon = ImageStore::getQIcon("ActionDelete", QSize(32,32));
	theList.push_back( ToolBoxItem(  3, myIcon, "Aap",  "dit is een aap"));
	theList.push_back( ToolBoxItem(  3, myIcon, "Noot", "Noot is tweede in het leesplankje"));
	theList.push_back( ToolBoxItem(999, myIcon, "Mies", "Mies Bouwman?"));
	theList.push_back( ToolBoxItem(  3, myIcon, "Wim",  "Wim Lex"));
	theList.push_back( ToolBoxItem(  2, myIcon, "Zus",  "Zus was vroeger een voornaam"));
	theList.push_back( ToolBoxItem(  1, myIcon, "Jet", " Jet is de laatste uit de eerste rij"));
}



 QVariant ToolBoxItemListModel::data(const QModelIndex &index, int role) const
 {
	if (!index.isValid())
		return QVariant();

	if (index.row() >= theList.size())
		return QVariant();
	const ToolBoxItem& myItem = theList.at(index.row());

	switch(role)
	{
	case Qt::DisplayRole:
		if (myItem.theCount!=ToolBoxItem::INFINITE)
			return tr("%2 (%1 left)").arg(QString::number(myItem.theCount), myItem.theName);
		else
			return tr("%1 (unlimited)").arg(myItem.theName);;
		break;
	case Qt::DecorationRole:
		return theList.at(index.row()).theIcon;
		break;
	case Qt::ToolTipRole:
		return theList.at(index.row()).theTooltip;
		break;
	default:
		return QVariant();
	}
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
