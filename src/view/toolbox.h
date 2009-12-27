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

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "tbe_global.h"
#include "BaseObject.h"

#include <QIcon>
#include <QDomNode>
#include <QListWidget>
#include <QListWidgetItem>


class TBItem : public QListWidgetItem
{
public:
	/// the mime type - as agreed upon between the ToolBox and the DrawWorld
	static const char* ToolboxMimeType;

	TBItem(unsigned int aCount,
		   const QIcon& anIcon,
		   const QString& aName,
		   const QDomNode myDomNode);

	TBItem(const ObjectFactory* aFactoryPtr);

	/** returns a pointer to a new object and decreases the count of objects left
	  * @returns a valid pointer to a new object or NULL if no objects left or errors
	  */
	BaseObject* getNewObject(void);

	const static unsigned int INFINITE = 999;

	QString getID(void) const
	{
		return "TBE-" + QString::number(reinterpret_cast<qulonglong>(this),16);
	}

	/** modifies the number of objects left,
	  * will also change the object description (which contains the object count)
	  * @param aDelta - the number to add/subtract
	  * @returns   true if still objects left
	  */
	bool modifyCount(int aDelta);

private:
	/// the number of objects left (part of Qt::DisplayRole)
	unsigned int theCount;

	/// the name of the object
	QString	theName;

	/// using the BaseObjectSerializer, we can create objects from this DomNode
	QDomNode theDomNode;

	/// using the ObjectFactory, we can create objects from this Name
	const ObjectFactory* theFactoryPtr;
};


class ToolBox : public QListWidget
{
	Q_OBJECT

public:
	ToolBox(QWidget *parent = 0);

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

	/** get a copy of the object in the ToolBoxItem specified
	  * @param anObjectName  internal name (see data(,Qt::EditRole)) of a ToolBoxItem
	  * @returns NULL if Object is not available, otherwise pointer to new fresh object
	  *          it will also decrease the counter of the item
	  */
	BaseObject* getMeACopyOf(const QString& anObjectName);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void startDrag(Qt::DropActions supportedActions);
};

#endif // TOOLBOX_H
