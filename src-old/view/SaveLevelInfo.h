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

	QString getFileName(void) const
		{ return ui.theFileNameField->text(); }

public slots:
	void FileDialogButton_clicked();

	/// overridden to intercept the accept and allow to check the file name
	virtual void accept();

	/// called whenever the user changes the text.
	void fileNameChanged()
		{ isUserOKOverwritingFile = false; }


private:
	Ui::SaveLevelInfo ui;
	Level*	theLevelPtr;

	/** checks whether a file name already exists.
	 *  if the file exists and isUserOKOverwrintingFile is not true,
	 *  it will popup a question for overwrite?
	 *  @returns true if user is OK with overwriting
	 *           *or* isUserOKOverwrintingFile is already true
	 *           *or* the file does not exist yet
	 */
	bool performFileExists(const QString& aFileName);



	/// set to true if user answered the question whether the file should be overwritten
	bool isUserOKOverwritingFile;
};

#endif // SAVELEVELINFO_H
