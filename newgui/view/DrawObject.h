/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QGraphicsPixmapItem>

// forward declarations
class BaseObject;


/** class DrawObject
  *
  * This class abstracts the actual drawing of objects
  *
  */
class DrawObject : public QGraphicsPixmapItem
{
public:
	/// simple constructor
	explicit DrawObject(BaseObject* aBaseObjectPtr);

	/// image name constructor
	DrawObject(BaseObject* aBaseObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawObject ( );

// FIXME/TODO: I think I no longer need this one as I'm going to use the scaling
// functions now... That allows me to stay closer to Qt's library code :-)
	/// overriden from QGraphicsItem, so we can specify our own object sizes
	/// @returns a QRectF with the width and height of the object in meters
//	virtual QRectF boundingRect() const;

	BaseObject* getBaseObjectPtr(void) const
		{ return theBaseObjectPtr; }


signals:

public slots:
	void rotateSomeMore(void);

protected:
	// Protected attributes
	//

	BaseObject* theBaseObjectPtr;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( DrawObject )
};

#endif // DRAWOBJECT_H
