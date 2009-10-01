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
#include "BaseObject.h"

#include <QIcon>
#include <QString>
#include <QList>
#include <QAbstractListModel>

// forward declarations
class QDomNode;

/// this class contains everything one needs to know
/// to add an item to the toolbox
class ToolBoxItem
{
public:
	const static unsigned int INFINITE = 999;

	ToolBoxItem(unsigned int aCount,
				const QIcon& anIcon,
				const QString& aName,
				const QString& aTooltip);
//	ToolBoxItem(unsigned int aCount,
//				const BaseObject& anObject);

	/// the number of objects left (part of Qt::DisplayRole)
	unsigned int theCount;

	/// Qt::DecorationRole - the icon to show
	QIcon   theIcon;

	/** Qt::EditRole & Qt::DisplayRole - the Name of the object
	 *  (Qt::DisplayRole is the Count and theName together)
	 */
	QString	theName;

	/// Qt::ToolTipRole - the tooltip
	QString	theTooltip;

	/// TODO: add pointer to actual factory
};


/// this class contains all items in the toolbox
class ToolBoxItemListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	/// the mime type - as agreed upon between the ToolBox and the DrawWorld
	static const char* ToolboxMimeType;

	ToolBoxItemListModel(QObject *parent = 0);

	/** Fills the Toolbox with all known Objects.
	 *  This is of course only to be done in Level Editor mode or cheat...
	 */
	bool fillFromObjectFactory(void);

	/** Fills the Toolbox with everything specified
	 *  in the XML DOM node called "toolbox"
	 *  @param aToolboxDomNode QDomNode called "toolbox" containing "toolboxitems"
	 *  @returns true if successful
	 */
	bool fillFromDomNode(const QDomNode& aToolboxDomNode);

// ----- the below are needed to get a ListModel

	/// required implementation from QAbstractListModel
	virtual QVariant data(const QModelIndex &index, int role) const;

	/// required implementation from QAbstractListModel
	Qt::ItemFlags flags(const QModelIndex &index) const;

	/// required implementation from QAbstractListModel
	bool removeRows(int row, int count, const QModelIndex &parent);

	/// required implementation from QAbstractListModel
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

// ----- the below are needed for Drag&Drop

	Qt::DropActions supportedDropActions() const;

	/// needed for Drag&Drop
//	bool dropMimeData(const QMimeData *data, Qt::DropAction action,
//					  int row, int column, const QModelIndex &parent);
	/// needed for Drag&Drop
	QMimeData *mimeData(const QModelIndexList &indexes) const;

	/// needed for Drag&Drop
	QStringList mimeTypes() const;

	/// needed for Drag&Drop
//	Qt::DropActions supportedDropActions() const;



private:
	typedef QList<ToolBoxItem> ToolBoxItemPtrList;
	ToolBoxItemPtrList theList;
};

#endif // TOOLBOXITEMLISTMODEL_H
