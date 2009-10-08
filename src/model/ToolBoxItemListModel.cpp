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
#include "BaseObjectSerializer.h"

#include <QMimeData>
#include <QDomNode>

const char* ToolBoxItemListModel::ToolboxMimeType = "image/x-puzzle-piece";


ToolBoxItem::ToolBoxItem(unsigned int aCount,
				const QIcon&   anIcon,
				const QString& aName,
				QDomNode aDomNode)
	: theCount(aCount),
	theIcon(anIcon),
	theName(aName),
	theDomNode(aDomNode),
	theFactoryPtr(NULL)
{
	; // nothing to do
}


ToolBoxItem::ToolBoxItem(const ObjectFactory* aFactoryPtr)
	: theCount(INFINITE),
	theFactoryPtr(aFactoryPtr)
{
	assert(aFactoryPtr != NULL);
	if (aFactoryPtr==NULL)
		return;
	BaseObject* myPtr = aFactoryPtr->createObject();
	if (myPtr != NULL)
	{
		// TODO: this way of getting icon names is not foolproof, but it works (for now)
		theIcon = ImageStore::getQIcon(myPtr->getName(), QSize(32,32));
		theName = myPtr->getName();
		delete myPtr;
	}
}

ToolBoxItem::~ToolBoxItem()
{
	; // nothing to do anymore - there are no pointers in here...
}


BaseObject* ToolBoxItem::getNewObject(void)
{
	if (theCount <= 0)
		return NULL;

	BaseObject* myBOPtr = NULL;
	if (theFactoryPtr != NULL)
	{
		myBOPtr = theFactoryPtr->createObject();
	}
	else
	{
		myBOPtr = BaseObjectSerializer::createObjectFromDom(theDomNode, false);
		if (myBOPtr == NULL)
		{
			DEBUG2("error parsing toolbox: did not get valid object '%s' from <object>\n",
				   ASCII(theDomNode.attributes().namedItem("name").nodeValue()));
		}
	}
	if (myBOPtr!=NULL && theCount > 0)
	{
		if (theCount != INFINITE)
			theCount--;
	}

	return myBOPtr;
}


// **************************************************************************
// **************************************************************************


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
			return tr("%1x %2").arg(QString::number(myItem.theCount), myItem.theName);
		else
			return tr("%1 (unlimited)").arg(myItem.theName);;
		break;
	case Qt::DecorationRole:
		return myItem.theIcon;
		break;
	case Qt::ToolTipRole:
		return myItem.theTooltip;
		break;
	case Qt::EditRole:	// edit role is used to build the mime type
		return myItem.getID();
	default:
		return QVariant();
	}
}

bool ToolBoxItemListModel::fillFromDomNode(const QDomNode& aToolboxDomNode)
{
	bool myResult = true;
	QDomNode myTBI, myO;

	// simple sanity check
	if (aToolboxDomNode.nodeName() != "toolbox")
	{
		DEBUG1("Level's <toolbox> node is not called toolbox ?!?\n");
		return false;
	}

	for (myTBI=aToolboxDomNode.firstChild(); !myTBI.isNull(); myTBI=myTBI.nextSibling())
	{
		// a toolbox object entry has the following layout:
		// <toolboxitem count="1" name="Right Ramp" icon="RightRamp">
		//      <object width="2" height="1" type="RightRamp" />
		// </toolboxitem>

		// simple sanity checks
		if (myTBI.nodeName() != "toolboxitem")
		{
			DEBUG2("error parsing toolbox: expected <toolboxitem> but got <%s>\n", ASCII(myTBI.nodeName()));
			myResult = false;
			break;
		}

		myO = myTBI.firstChild();
		if (myO.nodeName() != "object")
		{
			DEBUG2("error parsing toolbox: expected <object> but got <%s>\n", ASCII(myO.nodeName()));
			myResult = false;
			break;
		}

		QDomNamedNodeMap myNodeMap = myTBI.attributes();
		QString myTBI_Name     = myNodeMap.namedItem("name").nodeValue();
		QString myTBI_IconName = myNodeMap.namedItem("icon").nodeValue();
		bool    isOK;
		int     myTBI_Count = myNodeMap.namedItem("count").nodeValue().toInt(&isOK);
		if (!isOK)
			myTBI_Count = ToolBoxItem::INFINITE;

		// TODO: this way of getting icon names is not foolproof, but it works (for now)
		if (myTBI_IconName.isEmpty())
			myTBI_IconName = myTBI_Name;

		QIcon myIcon = ImageStore::getQIcon(myTBI_IconName, QSize(32,32));
		theList.push_back( ToolBoxItem( myTBI_Count, myIcon, myTBI_Name,  myO));

		if (myTBI==aToolboxDomNode.lastChild())
			break;
	}
	return myResult;
}

bool ToolBoxItemListModel::fillFromObjectFactory(void)
{
	ObjectFactory::ObjectFactoryList* myListPtr = ObjectFactory::getAllFactories();
	int i=0;
	for (i=0; i< myListPtr->count(); i++)
	{
		const ObjectFactory* myPtr = myListPtr->at(i);
		if (myPtr != NULL)
		{
			theList.push_back(ToolBoxItem(myPtr));
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


BaseObject* ToolBoxItemListModel::getMeACopyOf(const QString& anObjectName)
{
	// iterate through the list to find the item whose data() equals anObjectName

	ToolBoxItemPtrList::iterator i;
	for (i= theList.begin(); i != theList.end(); ++i)
	{
		if ((*i).getID() == anObjectName)
		{
			return (*i).getNewObject();
		}
	}

	return NULL;
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


bool ToolBoxItemListModel::removeRows(int, int, const QModelIndex& )
{
	// TODO: NOT IMPLEMENTED YET
	return false;
}


int ToolBoxItemListModel::rowCount(const QModelIndex&) const
{
	return theList.count();
}


Qt::DropActions ToolBoxItemListModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}


