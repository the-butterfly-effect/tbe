/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "DrawPolyObject.h"
#include "BaseObject.h"
#include "ImageStore.h"
#include "DrawWorld.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

// Constructors/Destructors
//  

DrawPolyObject::DrawPolyObject (BaseObject* aBaseObjectPtr,
						const QString& anImageName,
						DrawObject::ImageType anImageType)
	: DrawObject(aBaseObjectPtr, anImageName, anImageType)
{
	// everything is done in the DrawObject constructor
	DEBUG5("DrawPolyObject::DrawPolyObject()\n");

	setFlag(QGraphicsItem::ItemIsSelectable,true);
}

DrawPolyObject::~DrawPolyObject ( )
{
	DEBUG5("DrawPolyObject::~DrawPolyObject() for %p\n", this);
}

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  


QPainterPath DrawPolyObject::shape() const
{
	QPainterPath path;

	// we need to iterate over the shapelist,
	// get the polygon from each b2PolygonDef and add it to the path
	BaseObject::ShapeList::iterator myI = theBaseObjectPtr->theShapeList.begin();

	while (myI != theBaseObjectPtr->theShapeList.end())
	{
		QPolygonF myPolygon;

		// only handle the true poly shapes
		b2PolygonDef* myPoly = dynamic_cast<b2PolygonDef*>(*myI);
		if (myPoly==NULL)
		{
			++myI;
			continue;
		}

		int i;
		for (i=0; i < myPoly->vertexCount; i++)
		{
			b2Vec2 myVec = myPoly->vertices[i];
			myPolygon << QPointF(myVec.x*theScale, -myVec.y*theScale);
		}
		// and close the loop
		b2Vec2 myVec = myPoly->vertices[0];
		myPolygon << QPointF(myVec.x*theScale, -myVec.y*theScale);

		path.addPolygon ( myPolygon );
		++myI;
	}
	return path;
}


void DrawPolyObject::paint(QPainter* myPainter, const QStyleOptionGraphicsItem *aQ, QWidget *aW)
{
	DrawObject::paint(myPainter, aQ, aW);
	if (theDrawPolyOutline)
		myPainter->drawPath(shape());
}
