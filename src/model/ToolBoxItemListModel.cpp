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

#include <QMimeData>

const char* ToolBoxItemListModel::ToolboxMimeType = "image/x-puzzle-piece";


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
	; // nothing to do here either...
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
		return myItem.theIcon;
		break;
	case Qt::ToolTipRole:
		return myItem.theTooltip;
		break;
	case Qt::EditRole:
		return myItem.theName;
	default:
		return QVariant();
	}
}


bool ToolBoxItemListModel::fillFromObjectFactory(void)
{
	ObjectFactory::ObjectFactoryList* myListPtr = ObjectFactory::getAllFactories();
	int i=0;
	for (i=0; i< myListPtr->count(); i++)
	{
		BaseObject* myPtr = myListPtr->at(i)->createObject();
		if (myPtr != NULL)
		{
			// TODO: get the Icon (name) from the BaseObject
			QIcon myIcon = ImageStore::getQIcon("ActionRotate", QSize(32,32));
			theList.push_back( ToolBoxItem( ToolBoxItem::INFINITE, myIcon, myPtr->getName(),  myPtr->getToolTip()));
			delete myPtr;
		}
	}
	delete myListPtr;
	return true;
}


Qt::ItemFlags ToolBoxItemListModel::flags(const QModelIndex &index) const
{
	if (index.isValid())
		return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

	return Qt::ItemIsDropEnabled;
}


QMimeData* ToolBoxItemListModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes)
	{
		if (index.isValid())
		{
			// stream theName of the object to create only
			QString myItemName = qVariantValue<QString>(data(index, Qt::EditRole));
			DEBUG5("ToolBoxItemListModel::mimeData: '%s'\n", myItemName.toAscii().constData());
			stream << myItemName;
		}
	}

	mimeData->setData(ToolboxMimeType, encodedData);
	return mimeData;
}


QStringList ToolBoxItemListModel::mimeTypes() const
{
	QStringList types;
	types << ToolboxMimeType;
	return types;
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


Qt::DropActions ToolBoxItemListModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}


