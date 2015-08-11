/* The Butterfly Effect
 * This file copyright (C) 2010,2013  Klaas van Gend
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

#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include "ui_EditObjectDialog.h"
#include "tbe_global.h"
#include "AbstractObject.h"
#include "AbstractUndoCommand.h"
#include <QDialog>

class AbstractUndoCommand;


/** Dialog to edit all properties of an object.
  * Owned and created by ResizingGraphicsView, which also has a
  * "slot_showEditObjectDialog(AbstractObject *anAOPtr)" member.
  */
class EditObjectDialog : public QDialog
{
public:
    Q_OBJECT

public:
    EditObjectDialog(AbstractObjectPtr aAbstractObjectPtr, QWidget *parent = 0);
    virtual ~EditObjectDialog();

public slots:
    /** updates all modal dialog fields to reflect the values of the object
      * pointed to.
      * NOTE: the pointer is cached - if the user changes something,
      * this will be written back into the object.
      * @param anAbstractObjectPtr pointer to AbstractObject to read all data
      *                       from, or a NULL pointer to grey out all
      *                       values
      * @returns true if successful - which should be always
      */
    void readFromObject(AbstractObjectPtr anAbstractObjectPtr);

private slots:
    void position_editingFinished();
    void position_valueChanged(double);
    void propertyCellChanged ( int row, int column );
    void lineEditID_valueChanged ( void );

private:
    Ui::EditObjectDialog ui;

    AbstractObjectWeakPtr theAOPtr;

    /// use whenever you need something from the real object instead of the
    /// std::weak_ptr.
    AbstractObject* getAORealPtr(void)
    {
        if (theAOPtr.expired())
            return nullptr;
        else
            return AbstractObjectPtr(theAOPtr).get();
    }

    /// @returns pointer to the *Abstract* UndoCommand
    AbstractUndoCommand* getUndoPtr(void);

    // kill possibility for copy constructor&assignment operator
    EditObjectDialog(const EditObjectDialog&) = delete;
    const EditObjectDialog& operator=(const EditObjectDialog&) = delete;

    // persistant position of the dialog
    static QPoint thePosition;
};

#endif // EDITOBJECTDIALOG_H
