/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef EDITLEVELPROPERTIES_H
#define EDITLEVELPROPERTIES_H

#include <QtGui/QDialog>

// forward declarations
class Level;

namespace Ui {
    class EditLevelProperties;
}


/// this class allows editing of various Level and World settings
class EditLevelProperties : public QDialog {
    Q_OBJECT
public:
	EditLevelProperties(Level* aLevelPtr, QWidget *parent = 0);
    ~EditLevelProperties();

protected:
    void changeEvent(QEvent *e);

private slots:
	/// overridden to actually write the results to the Level/World
	void slot_accepted();

private:
	Level* theLevelPtr;
    Ui::EditLevelProperties *m_ui;
};

#endif // EDITLEVELPROPERTIES_H
