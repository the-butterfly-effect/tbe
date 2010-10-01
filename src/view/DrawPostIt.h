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

#ifndef DRAWPOSTIT_H
#define DRAWPOSTIT_H

#include "DrawObject.h"
#include "PostIt.h"
#include "ui_PostItViewer.h"

// forward declarations
class BaseObject;


/** class DrawPostIt
  *
  * This class draws the PostIt class, which has no physics representation,
  * and cannot be moved *BUT* can be clicked to reveal "hints"...
  */

class DrawPostIt : public QObject, public DrawObject
{

	Q_OBJECT

public:
	// Constructors/Destructors
	//

	/**
	 * Empty Constructor
	 */
	DrawPostIt (BaseObject* aBaseObjectPtr);

	/**
	 * Empty Destructor
	 */
	virtual ~DrawPostIt ( );

	/// this member starts the display of the actual Post-It dialog
	void displayPostit(void);

	/** overridden from QGraphicsItem
	 * upon a double click event, let's show the text of the note-its
	 */
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* );

	/** overridden from QGraphicsItem
	 * also, upon a single click event, let's show the text of the note-its
	 * (or just do move if we're in leveleditor)
	 */
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* anEvent);

	/// overridden from DrawObject to make sure it is not selected
	virtual void focusInEvent ( QFocusEvent* )
		{ ; }

	/// overriden from DrawObject to allow extra graphics on hovering
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget);

protected slots:
	void nextClicked();

protected:
	/// overridden to allow highlighting
	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	/// overridden to allow highlighting
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

private:
	virtual void initAttributes ( ) ;

	/** retrieves the Post-It page from the BaseObject's properties list
	  * the pages are numbered  "page1", "page2", etc.
	  * i18n exists: "page1_nl" is the dutch version of page1.
	  * if a page only exists in English, that one is returned.
	  * returns an empty string if the page does not exist.
	  */
	QString getPageString(unsigned int aPage);

	unsigned int theCurrentPage;

	QDialog*	theDialogPtr;

	Ui::PostItViewer* theUIPtr;

	bool isHovering;
};

#endif // DRAWPOSTIT_H
