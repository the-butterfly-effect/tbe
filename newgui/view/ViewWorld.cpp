/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#include "MainWindow.h"
#include "PieMenu.h"
#include "ViewWorld.h"
#include "World.h"

#include <QGraphicsSceneMouseEvent>

ViewWorld::ViewWorld (MainWindow* aMainWindowPtr, World* aWorldPtr)
	: QGraphicsScene( NULL ), theWorldPtr(aWorldPtr)
{
	initAttributes();

	aMainWindowPtr->setScene(this, theWorldPtr->getName());
}


void ViewWorld::initAttributes ( )
{
	DEBUG1ENTRY;
}

void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
    if (itemAt(mouseEvent->scenePos())==NULL)
        PieMenuSingleton::clearPieMenu();
    QGraphicsScene::mousePressEvent(mouseEvent);
}
