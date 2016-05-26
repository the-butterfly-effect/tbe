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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */


#ifndef STUBDRAWOBJECT_H
#define STUBDRAWOBJECT_H

#include <QGraphicsItem>

// forward declarations
class BaseObject;
class QUndoStack;
class QUndoCommand;
class QSvgRenderer;
class DrawWorld;

class DrawObject : public QGraphicsItem
{
public:
    // Constructors/Destructors
    //

    /// Simple Constructor
    DrawObject (BaseObject *aBaseObjectPtr);

    enum ImageType {
        IMAGE_PNG,
        IMAGE_SVG,
        IMAGE_ANY
    };

    /// Svg/Png Constructor
    DrawObject (BaseObject *aBaseObjectPtr, const QString &anImageName,
                ImageType anImageType = IMAGE_ANY);


    /**
     * Empty Destructor
     */
    virtual ~DrawObject ( );


    /// overriden from QGraphicsItem
    virtual QRectF boundingRect() const;

    /// overriden from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    /// setup caching for this object
    void setupCache(void);

public:
    BaseObject *theBaseObjectPtr;

public:
    virtual void initAttributes ( ) ;

};

#endif // STUBDRAWOBJECT_H
