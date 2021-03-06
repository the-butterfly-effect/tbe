/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef VIEWDETONATORBOX_H
#define VIEWDETONATORBOX_H

#include "ViewObject.h"

// forward declarations
class AbstractObject;

/** class ViewDetonatorBox
  *
  * This class draws the DetonatorBox class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class ViewDetonatorBox : public ViewObject
{

    Q_OBJECT

protected:
    // Constructors/Destructors
    //

    /// Image Constructor
    ViewDetonatorBox (AbstractObjectPtr aAbstractObjectPtr, const QString &anImageName);
    friend class ViewObject;

public:
    /**
     * Empty Destructor
     */
    virtual ~ViewDetonatorBox ( ) override;

    /// this member starts the display of the actual ChoosePhoneNumber dialog
    void displayChoosePhoneNumber(void);

protected:
    /// overridden to ALWAYS allow highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent *event ) override;
    /// overridden to ALWAYS allow mouse button presses
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent *event) override;

private:
    // prevent copy constructor / assignment operator
    ViewDetonatorBox(const ViewDetonatorBox &);
    const ViewDetonatorBox &operator= (const ViewDetonatorBox &);
};

#endif // VIEWDETONATORBOX_H
