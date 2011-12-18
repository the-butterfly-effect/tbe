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

#include "Position.h"

#include <QtGui/QGraphicsSceneMouseEvent>

ViewWorld::ViewWorld (MainWindow* aMainWindowPtr, World* aWorldPtr)
	: QGraphicsScene(0, -THESCALE*aWorldPtr->getTheWorldHeight(),
					 THESCALE*aWorldPtr->getTheWorldWidth(), THESCALE*aWorldPtr->getTheWorldHeight()),
	  theWorldPtr(aWorldPtr)
{
	initAttributes();

	aMainWindowPtr->setScene(this, theWorldPtr->getName());

	addRect(0, 0, getWidth(), -getHeight());

	if (theWorldPtr->theBackground.theBackgroundGradient.count()==0 &&
		theWorldPtr->theBackground.theImageName.isEmpty())
	{
		// the default if nothing else specified: a blue gradient background
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
		theWorldPtr->theBackground.theBackgroundGradient.push_back(
				Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
	}
	setBackgroundBrush(Qt::blue);
	QLinearGradient myBackground(0,0, 0,-getHeight());
	foreach(Background::GradientStop myGS, theWorldPtr->theBackground.theBackgroundGradient)
		myBackground.setColorAt(myGS.thePosition, QColor(myGS.theR*255, myGS.theG*255, myGS.theB*255, myGS.theAlpha*255));
	setBackgroundBrush(myBackground);
}


qreal ViewWorld::getHeight()
{
	return THESCALE*theWorldPtr->getTheWorldHeight();
}


qreal ViewWorld::getWidth()
{
	return THESCALE*theWorldPtr->getTheWorldWidth();
}


void ViewWorld::initAttributes ( )
{
	DEBUG1ENTRY;
}

void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
    QGraphicsItem* myItemPtr = itemAt(mouseEvent->scenePos());
    if (myItemPtr!=NULL)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }
    // nothing clicked: clear pie menu
    PieMenuSingleton::clearPieMenu();
}
