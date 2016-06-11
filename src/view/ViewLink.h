/* The Butterfly Effect
 * This file copyright (C) 2012  Klaas van Gend
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

#ifndef VIEWLINK_H
#define VIEWLINK_H

#include "ViewObject.h"

// forward declarations
class AbstractObject;
class ViewLink;

/**
 * @brief The ViewLine class is used by ViewLink to draw the line and allows
 *        selecting and hovering by the user if in Level Creator mode.
 */
class ViewLine : public QGraphicsLineItem
{
public:
    ViewLine(ViewLink *aParentPtr) : QGraphicsLineItem(nullptr), theParent(aParentPtr)
    {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

protected:
    /** Overridden from QGraphicsLineItem.
     *  If we're in Level Creator mode, allow clicking the object.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *anEvent) override;

private:
    ViewLink *theParent;
};

/** class ViewLink
  *
  * This class draws the DetonatorBox class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class ViewLink : public ViewObject
{

    Q_OBJECT

    // Constructors/Destructors
    //

protected:
    /// Image Constructor, specify a color for the line.
    /// @param anImageName  Specify a color. Color syntax needs to be parsible by QColor::setNamedColor().
    ///                     Examples:
    ///                       * names: "transparent", "olive", "red"
    ///                         see https://www.w3.org/TR/SVG/types.html#ColorKeywords
    ///                       * #RGB (each of R, G, and B is a single hex digit)
    ///                       * #RRGGBB  (e.g. #FF0000 is red)
    ///                       * #AARRGGBB (so alpha is the FIRST field, 00=transparent, FF=opaque)
    explicit ViewLink (AbstractObjectPtr aAbstractObjectPtr, const QString &anImageName);
    friend class ViewObject;

public:
    /**
     * Empty Destructor
     */
    virtual ~ViewLink ( );


    /// Draws a line between aFirstPoint and aSecondPoint, using the color
    /// specified in anImageName during construction.
    /// @param aFirstPoint  first end point of the line
    /// @param aSecondPoint second end point of the line
    void setEndpoints(const Vector &aFirstPoint,
                      const Vector &aSecondPoint);

private:
    // prevent copy constructor / assignment operator
    ViewLink(const ViewLink &);
    const ViewLink &operator= (const ViewLink &);

    ViewLine *theLinePtr;
    friend class ViewLine;

    QString theImageName;
};

#endif // VIEWLINK_H
