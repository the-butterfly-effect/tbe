/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#include "toolbox.h"
#include "ImageStore.h"
#include "BaseObjectSerializer.h"
#include "UndoDeleteCommand.h"
#include "DrawObject.h"
#include "LocalString.h"

#include <QtGui>
#include <QMimeData>
#include <cassert>


const char* TBItem::ToolboxMimeType   = "image/x-tbe-tool-scene";
const char* TBItem::DrawWorldMimeType = "image/x-tbe-scene-tool";


TBItem::TBItem(unsigned int aCount,
				const QString& anIconName,
				const QString& aName,
				QDomNode aDomNode)
	: theCount(aCount),
	theName(aName),
	theDomNode(aDomNode),
	theFactoryPtr(NULL)
{
	if (anIconName.isEmpty())
		createIcon();
	else
		theIcon = ImageStore::getQIcon(anIconName,
				  QSize(TOOLBOX_ICON_SIZE,TOOLBOX_ICON_SIZE));

	if (aName.isEmpty())
	{
		BaseObject* myBOPtr = getNewObject();
		theName = myBOPtr->getName();
		delete myBOPtr;
	}

	// actually display the count + setIcon
	modifyCount(0);

}


TBItem::TBItem(const ObjectFactory* aFactoryPtr)
	: theCount(INFINITE),
	theFactoryPtr(aFactoryPtr)
{
	assert(aFactoryPtr != NULL);
	if (aFactoryPtr==NULL)
		return;
	DEBUG4("TBItem::TBItem(ObjectFactory*) for %s\n", ASCII(aFactoryPtr->getFactoryName()));
	BaseObject* myPtr = getNewObject();
	if (myPtr != NULL)
	{
		theName = myPtr->getName();
		delete myPtr;
	}
	createIcon();
}

void TBItem::createIcon(void)
{
	QPixmap* myPixmap = createPixmap(TOOLBOX_ICON_SIZE);
	if (myPixmap != NULL)
		theIcon = QIcon(*myPixmap);
	else
	{
		// let's hope this works.
		// TODO: if not, we can still try the image name property...
		theIcon = ImageStore::getQIcon(theName,
					QSize(TOOLBOX_ICON_SIZE,TOOLBOX_ICON_SIZE));
	}
	setIcon(theIcon);
}

QPixmap* TBItem::createPixmap(int aSuggestedSize)
{
	QPixmap* myPixmapPtr = NULL;
	BaseObject* myPtr = getNewObject();
	if (myPtr != NULL)
	{
		DrawObject* myDOPtr = myPtr->createDrawObject();
		if (myDOPtr != NULL)
			myPixmapPtr = myDOPtr->createBitmap(aSuggestedSize, aSuggestedSize);
	}
	delete myPtr;
	return myPixmapPtr;
}

BaseObject* TBItem::getNewObject(void)
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
		assert(theDomNode.isNull()==false);
		myBOPtr = BaseObjectSerializer::createObjectFromDom(theDomNode, false);
		if (myBOPtr == NULL)
		{
			DEBUG2("error parsing toolbox: did not get valid object '%s' from <object>\n",
				   ASCII(theDomNode.attributes().namedItem("name").nodeValue()));
		}
	}
	return myBOPtr;
}

bool TBItem::modifyCount(int delta)
{
	if (theCount != INFINITE)
		theCount += delta;
	setText( QObject::tr("%1x %2").arg(theCount).arg(theName));

	if (theCount >0)
	{
		// (re)set original icon
		setIcon(theIcon);
		return false;
	}
	else
	{
		// replace icon by cross
		setIcon(ImageStore::getQIcon("BigCross",
						QSize(TOOLBOX_ICON_SIZE,TOOLBOX_ICON_SIZE)));
		return true;
	}
}



// #######################################################################


ToolBox::ToolBox(QWidget *parent)
	: QListWidget(parent)
{
	setAcceptDrops(true);
	setDragEnabled(true);
	setDropIndicatorShown(true);
	setGridSize(QSize(100, 64));
	setIconSize(QSize(TBItem::TOOLBOX_ICON_SIZE, TBItem::TOOLBOX_ICON_SIZE));
	setMovement(QListView::Snap);
	setSpacing(10);
	setViewMode(QListView::IconMode);
	UndoDeleteCommand::setToolBoxPtr(this);
}

ToolBox::~ToolBox()
{
	UndoDeleteCommand::setToolBoxPtr(NULL);
}

void ToolBox::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(TBItem::ToolboxMimeType) ||
		event->mimeData()->hasFormat(TBItem::DrawWorldMimeType))
		event->accept();
	else
		event->ignore();
}

void ToolBox::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(TBItem::ToolboxMimeType) ||
		event->mimeData()->hasFormat(TBItem::DrawWorldMimeType))
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	} else
		event->ignore();
}

void ToolBox::dropEvent(QDropEvent *event)
{
	DEBUG4("void ToolBox::dropEvent(\"%s\")\n", ASCII(event->mimeData()->formats().join(";")));
	if (event->mimeData()->hasFormat(TBItem::DrawWorldMimeType))
	{
		QByteArray pieceData = event->mimeData()->data(TBItem::ToolboxMimeType);
		QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
		QString myObjectName;
		dataStream >> myObjectName;

		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->ignore();
}

bool ToolBox::modifyCountOfBaseObject(BaseObject* aPtr, signed int aDelta)
{
	if(aPtr == NULL)
		return false;
	CreationMap::iterator myIterator = theCreationMap.find(aPtr);
	if (myIterator == theCreationMap.end())
		return false;
	(*myIterator)->modifyCount(aDelta);
	return true;
}

void ToolBox::startDrag(Qt::DropActions /*supportedActions*/)
{
	DEBUG4("ToolBox::startDrag()\n");
	TBItem* item = dynamic_cast<TBItem*>(currentItem());

	// don't start a D&D if we have nothing left...
	if (item->hasObjectsLeft()==false)
		return;

	// create MIME data
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << item->getID();
	QMimeData* myMimeDataPtr = new QMimeData;
	myMimeDataPtr->setData(TBItem::ToolboxMimeType, itemData);

	// let's NOT add an icon to the QDrag
	QDrag* myDragPtr = new QDrag(this);
	myDragPtr->setMimeData(myMimeDataPtr);

	// no need to check if it succeeds - getMeACopy() will be called anyway.
	myDragPtr->exec(Qt::MoveAction);
}


bool ToolBox::fillFromDomNode(const QDomNode& aToolboxDomNode)
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
		// <toolboxitem count="1" name="Ramp \" icon="RightRamp">
		//      <name lang="nl">Helling \</name>
		//      <object width="2" height="1" type="RightRamp" />
		// </toolboxitem>

		// note that name is optional, if no name specified, we will take the
		// name from the object - which *might* be localized (maybe not)...

		// simple sanity checks
		if (myTBI.nodeName() != "toolboxitem")
		{
			DEBUG2("error parsing toolbox: expected <toolboxitem> but got <%s>\n", ASCII(myTBI.nodeName()));
			myResult = false;
			break;
		}

		myO = myTBI.firstChildElement("object");
		if (myO.isNull())
		{
			DEBUG2("error parsing toolbox: no <object> found\n");
			myResult = false;
			break;
		}

		QDomNamedNodeMap myNodeMap = myTBI.attributes();
		LocalString myTBI_Name;
		myTBI_Name.fillFromDOM(myTBI, "name", myNodeMap.namedItem("name").nodeValue());

		QString myTBI_IconName = myNodeMap.namedItem("icon").nodeValue();
		bool    isOK;
		int     myTBI_Count = myNodeMap.namedItem("count").nodeValue().toInt(&isOK);
		if (!isOK)
			myTBI_Count = TBItem::INFINITE;

		TBItem* myItemPtr = new TBItem( myTBI_Count, myTBI_IconName, myTBI_Name.result(),  myO);
		addItem(myItemPtr);
		if (myTBI==aToolboxDomNode.lastChild())
			break;
	}
	return myResult;
}

bool ToolBox::fillFromObjectFactory(void)
{
	ObjectFactory::ObjectFactoryList* myListPtr = ObjectFactory::getAllFactories();
	int i=0;
	for (i=0; i< myListPtr->count(); i++)
	{
		const ObjectFactory* myPtr = myListPtr->at(i);
		if (myPtr != NULL)
		{
			TBItem* myTBItemPtr = new TBItem(myPtr);
			addItem(myTBItemPtr);
		}
	}
	delete myListPtr;
	return true;
}

BaseObject* ToolBox::getMeACopyOf(const QString& anObjectName)
{
	int i=0;
	while ( item(i)!=0)
	{
		TBItem* myItem = dynamic_cast<TBItem*>(item(i));
		if (myItem->getID() == anObjectName)
		{
			BaseObject* myPtr = myItem->getNewObject();
			if (myPtr == NULL)
				return NULL;
			theCreationMap.insert(myPtr, myItem);
			return myPtr;
		}
		++i;
	}
	return NULL;
}
