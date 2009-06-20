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

#include "tbe_global.h"
#include "BaseObject.h"
#include "DrawWorld.h"
#include "DrawObject.h"
#include "PieMenu.h"
#include "ImageStore.h"


PieMenu::PieMenu(DrawObject* aDrawObjectPtr)
		: theDrawObjectPtr(aDrawObjectPtr)
{
	createActions();
}

PieMenu::~PieMenu()
{
}


QAction* PieMenu::actionFactoryMethod(const QString anIconName, const QString& aName, const QString& aTip,
									  const char* aSlotName, bool isEnabled)
{
	QAction* newAct = new QAction(ImageStore::getQIcon(anIconName, QSize(32,32)), aName, this);
	newAct->setToolTip(aTip);
	newAct->setStatusTip(aTip);
	newAct->setEnabled(isEnabled);
	connect(newAct, SIGNAL(triggered()), this, aSlotName);
	addAction(newAct);
	return newAct;
}

void PieMenu::createActions(void)
{
	BaseObject* myBOPtr = theDrawObjectPtr->getBaseObjectPtr();
	QString myBOName = myBOPtr->getName();

	actionFactoryMethod("ActionDelete", tr("Delete %1").arg(myBOName),
						tr("This will delete %1 from the screen and put it back in the Toolbox").arg(myBOName),
						SLOT(deleteObject()), myBOPtr->isMovable());
	actionFactoryMethod("ActionAdjust", tr("Adjust Properties"),
						tr("(if possibile) Modify the properties of %1").arg(myBOName),
						SLOT(adjust()), false);
}


void PieMenu::deleteObject(void)
{
	// TODO: implemente delete
	// do this by implementing UndoDeleteCommand
}


QString PieMenu::getEditModeIconName(PieMenu::EditMode anEditMode)
{
	switch(anEditMode)
	{
	case MOVE:
		return "ActionMove";
	case ROTATE:
		return "ActionRotate";
	case RESIZE_HORI:
		return "ActionResizeHori";
	case RESIZE_VERTI:
		return "ActionResizeVerti";
	case RESIZE_ROTATE:
		return "ActionResizeRotate";
	case NONE:
		return "ActionNone";
	}
	return "";
}
