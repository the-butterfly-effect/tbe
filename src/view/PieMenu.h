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

#ifndef PIEMENU_H
#define PIEMENU_H

#include <QMenu>

// forward declarations
class DrawObject;

/** class PieMenu
  *
  * This class implements the concept of a pie menu -
  * right-clicking an object will show various icons around the object
  * that allow to move, rotate, resize, etc
  *
  * FOR NOW, it uses the regular QMenu structure
  *
  */

class PieMenu : public QMenu
{
	Q_OBJECT

public:

        // Constructors/Destructors
        //

        /**
         * Constructor
         */
        PieMenu (DrawObject* aDrawObjectPtr);

        /**
         * Empty Destructor
         */
        virtual ~PieMenu ( );

		enum EditMode
		{
			MOVE=1,
			ROTATE,
			RESIZE_HORI,
			RESIZE_VERTI,
			RESIZE_ROTATE,
			NONE
		};

		static EditMode getEditMode();

		static QString getEditModeIconName(EditMode anEditMode=static_cast<EditMode>(0));

protected slots:
		void deleteObject(void);

		void move(void)
		{ setEditMode(MOVE); }

		void resizeRotate(void)
		{ setEditMode(RESIZE_ROTATE); }

private:
		/** this member populates the menu
		 */
		void createActions(void);

		/** this factory method creates and attaches a single QAction
		 *  @param anIconName
		 *  @param aName
		 *  @param aTip
		 *  @param aSlotName
		 *  @param isEnabled
		 */
		QAction* actionFactoryMethod(const QString anIconName, const QString& aName, const QString& aTip,
									 const char* aSlotName, bool isEnabled=true);

		DrawObject* theDrawObjectPtr;

		void setEditMode(EditMode aNewEditMode);
};

#endif // PIEMENU_H
