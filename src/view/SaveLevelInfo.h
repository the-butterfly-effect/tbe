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

#ifndef SAVELEVELINFO_H
#define SAVELEVELINFO_H

#include "ui_SaveLevelInfo.h"
#include "tbe_global.h"
#include "Level.h"
#include <QDialog>

class SaveLevelInfo : public QDialog
{
	 Q_OBJECT

public:
	SaveLevelInfo(Level* aLevelPtr, QWidget *parent = 0);
	virtual ~SaveLevelInfo();

	/** takes the input from the form and fills in the Level fields
	 *  @returns false if level data is invalid - Level is not changed.
	 */
	bool commitToLevel(void);

public slots:
	void on_FileDialogButton_clicked();

private:
	Ui::SaveLevelInfo ui;
	Level*	theLevelPtr;
};

#endif // SAVELEVELINFO_H
