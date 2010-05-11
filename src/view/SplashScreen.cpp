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
#include "SplashScreen.h"

#include <QGraphicsSceneMouseEvent>
#include <QBrush>

// Constructors/Destructors
//  


// TODO: FIXME: hardcoded path name here!!!
SplashScreen::SplashScreen (const QString& anSVGFileName)
	: QGraphicsSvgItem(anSVGFileName)
{
	setFlags(QGraphicsItem::ItemIsSelectable);

	// No need to keep the myTextPtr outside the constructor as QT will
	// keep track of the children itself
	QGraphicsSimpleTextItem* myTextPtr = new QGraphicsSimpleTextItem(tr("(click this pane to start)"), this);
	QRectF myTextBounds = myTextPtr->boundingRect();
	qreal myResize = 0.3 * boundingRect().width() / myTextBounds.width() ;
	myTextPtr->setBrush(Qt::red);
	myTextPtr->scale(myResize, myResize);
	myTextBounds = myTextPtr->mapToParent( myTextPtr->boundingRect() ).boundingRect();
	myTextPtr->setPos( boundingRect().center().x()-myTextBounds.center().x(),
					   boundingRect().height()-1.5*myTextBounds.height());

}

SplashScreen::~SplashScreen ( ) { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void SplashScreen::mousePressEvent ( UNUSED_ARG QGraphicsSceneMouseEvent * event )
{
	DEBUG5("SplashScreen::sceneEvent(%d)\n", event->type());
	emit clicked();
}
