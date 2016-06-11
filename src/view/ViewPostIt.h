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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef DRAWPOSTIT_H
#define DRAWPOSTIT_H

#include "ViewObject.h"
#include "PostIt.h"
#include "ui_ViewPostIt.h"

// forward declarations
class AnimatedDialog;
class AbstractObject;


/** class DrawPostIt
  *
  * This class draws the PostIt class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class ViewPostIt : public ViewObject
{

    Q_OBJECT

    // Constructors/Destructors
    //

protected:
    /**
     * Empty Constructor
     */
    explicit ViewPostIt (AbstractObjectPtr anAbstractObjectPtr, const QString &anImageName,
                         const QString &aBackgroundImageName, const QString &aButtonStyle);
    friend class ViewObject;

public:
    /**
     * Empty Destructor
     */
    virtual ~ViewPostIt ( );

    /// this member starts the display of the actual Post-It dialog
    void displayPostit(void);

    /** overridden from QGraphicsItem
     * upon a double click event, let's show the text of the note-its
     */
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * );

    /** overridden from QGraphicsItem
     * also, upon a single click event, let's show the text of the note-its
     * (or just do move if we're in the level creator)
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *anEvent);

    /// overridden from ViewObject to make sure it is not selected
    virtual void focusInEvent ( QFocusEvent * )
    {
        ;
    }

protected slots:
    void nextClicked();

protected:
    /// overridden to allow highlighting
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent *event );
    /// overridden to allow highlighting
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent *event );

private:
    virtual void initAttributes ( ) ;

    /** retrieves the Post-It page from the AbstractObject's properties list
      * the pages are numbered  "page1", "page2", etc.
      * i18n exists: "page1_nl" is the dutch version of page1.
      * if a page only exists in English, that one is returned.
      * returns an empty string if the page does not exist.
      */
    QString getPageString(unsigned int aPage);

    unsigned int theCurrentPage;

    AnimatedDialog *theDialogPtr;

    Ui::ViewPostIt *theUIPtr;

    const QString theBackgroundImageName;
    const QString theButtonStyle;

    // prevent copy constructor / assignment operator
    ViewPostIt(const ViewPostIt &);
    const ViewPostIt &operator= (const ViewPostIt &);

};

#endif // DRAWPOSTIT_H
