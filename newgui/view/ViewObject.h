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

#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H

#include <QGraphicsPixmapItem>

// forward declarations
class AbstractObject;
class PieMenu;

/** class ViewObject
  *
  * This class abstracts the actual drawing of objects
  *
  */
class ViewObject : public QGraphicsPixmapItem
{
public:
	/// simple constructor
	explicit ViewObject(AbstractObject* anAbstractObjectPtr);

	/// image name constructor
	ViewObject(AbstractObject* anAbstractObjectPtr, const QString& anImageName);

	/**
	 * Empty Destructor
	 */
	virtual ~ViewObject ( );

	AbstractObject* getAbstractObjectPtr(void) const
		{ return theAbstractObjectPtr; }

protected:
    /// overridden to allow detection of mouse button presses
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event);
    /// overridden to allow object highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent* event );
    /// overridden to allow highlighting
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent* event );

protected:
	// Protected attributes
	//

	AbstractObject* theAbstractObjectPtr;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( ViewObject )

private:
	void initViewObjectAttributes(void);
};

#endif // VIEWOBJECT_H
