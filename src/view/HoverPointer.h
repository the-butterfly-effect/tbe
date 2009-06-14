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

#ifndef HOVERPOINTER_H
#define HOVERPOINTER_H

#include <QGraphicsSvgItem>


// forward declarations
class QGraphicsScene;
class DrawObject;


class HoverPointer : public QGraphicsSvgItem
{
public:
	HoverPointer(QGraphicsScene* aScene, DrawObject* aParent);
	//virtual ~HoverPointer();


	/** overridden from QGraphicsItem
	 *  if the user drags the object around, this even will be called for each pixel.
	 *  let's actually adjust the coordinates!!!
	 *
	 *  @param event the even to handle
	 */
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );


	/** Overridden from QGraphicsSvgItem to ensure that we can set the offset
	  * to get the center of the icon below the hotspot of the mouse pointer
	  */
	virtual void setPos(const QPointF& aPos);

	/** Overridden from QGraphicsSvgItem to ensure that we can set the offset
	  * to get the center of the icon below the hotspot of the mouse pointer
	  */
	virtual void setPos(qreal x, qreal y);

private:
	qreal theOffset;
	DrawObject* theDrawObjectPtr;
};

#endif // HOVERPOINTER_H
