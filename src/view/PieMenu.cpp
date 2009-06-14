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
#include "DrawObject.h"
#include "PieMenu.h"
#include "ImageStore.h"


static PieMenu::EditMode theGlobalEditMode = PieMenu::MOVE;

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
	BaseObject* myBOPtr = theDrawObjectPtr->theBaseObjectPtr;
	QString myBOName = myBOPtr->getName();

	actionFactoryMethod(getEditModeIconName(MOVE), tr("Move %1").arg(myBOName),
						tr("In this mode, you can move %1 around").arg(myBOName),
						SLOT(move()), true);
	actionFactoryMethod(getEditModeIconName(RESIZE_ROTATE), tr("Resize/Rotate"),
						tr("In this mode, you can resize or rotate %1").arg(myBOName),
						SLOT(resizeRotate()), myBOPtr->isRotatable() | myBOPtr->isResizable());
	actionFactoryMethod("ActionDelete", tr("Delete %1").arg(myBOName),
						tr("This will delete %1 from the screen and put it back in the Toolbox").arg(myBOName),
						SLOT(deleteObject()), myBOPtr->isMovable());
	actionFactoryMethod("ActionAdjust", tr("Adjust Properties"),
						tr("(if possibile) Modify the properties of %1").arg(myBOName),
						SLOT(adjust()), false);
}


void PieMenu::deleteObject(void)
{
	// TODO implement delete
}


PieMenu::EditMode
PieMenu::getEditMode()
{
	return theGlobalEditMode;
}

QString PieMenu::getEditModeIconName(PieMenu::EditMode anEditMode)
{
	if (anEditMode==0)
		anEditMode=theGlobalEditMode;
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


void PieMenu::setEditMode(EditMode aNewEditMode)
{
	DEBUG4("setEditMode(%d)\n", aNewEditMode);
	theGlobalEditMode = aNewEditMode;
}
