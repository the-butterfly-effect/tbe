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

#include "tbe_global.h"
#include "SaveLevelInfo.h"
#include "Popup.h"
#include <QFileDialog>

SaveLevelInfo::SaveLevelInfo(Level* aLevelPtr, QWidget *parent)
	: QDialog(parent), theLevelPtr(aLevelPtr)
{
	isUserOKOverwrintingFile = false;

	ui.setupUi(this);
	ui.theFileNameField        ->setText(theLevelPtr->theFileName);
	ui.theAuthorNameField      ->setText(theLevelPtr->theLevelAuthor);
	ui.theLevelDescriptionField->setText(theLevelPtr->theLevelDescription);
	ui.theLicenseField         ->setText(theLevelPtr->theLevelLicense);
	ui.theTitleField           ->setText(theLevelPtr->theLevelName);

	QDate myDate = QDate::fromString(theLevelPtr->theLevelDate);
	if (myDate.isValid())
		ui.theDateEdit->setDate(myDate);
	else
	{
		DEBUG5("SaveLevelInfo::SaveLevelInfo - Date '%s' is not understood, setting to current date\n",
			   theLevelPtr->theLevelDate.toAscii().constData());
		ui.theDateEdit->setDate(QDate::currentDate());
	}

	// FIXME: TEMPORARY
	ui.theLevelDescriptionField->setText("THIS DIALOG IS NOT IMPLEMENTED YET!!!");
}


SaveLevelInfo::~SaveLevelInfo()
{
}



void SaveLevelInfo::accept()
{
	if (performFileExists(ui.theFileNameField->text()))
		QDialog::accept();
}


bool SaveLevelInfo::commitToLevel(void)
{
	if (ui.theTitleField->text().isEmpty())
		return false;
	if (ui.theAuthorNameField->text().isEmpty())
		return false;

	theLevelPtr->theLevelAuthor      = ui.theAuthorNameField->text();
	theLevelPtr->theLevelDate        = ui.theDateEdit->text();
	theLevelPtr->theLevelDescription = ui.theLevelDescriptionField->toPlainText();
	theLevelPtr->theLevelLicense     = ui.theLicenseField->text();
	theLevelPtr->theLevelName        = ui.theTitleField->text();

	if (ui.theFileNameField->text().isEmpty())
		return false;
	else
		theLevelPtr->theFileName = ui.theFileNameField->text();

	return true;
}


bool SaveLevelInfo::performFileExists(const QString& aFileName)
{
	if (isUserOKOverwrintingFile==true)
		return true;

	if (QFile::exists(aFileName))
	{
		DEBUG5("File '%s' already exists!\n", aFileName.toAscii().constData());
		if (Popup::YesNoQuestion(tr("A File with name '%1' file already exists. Overwrite?\n").arg(aFileName),
								 this)==false)
			return false;
		isUserOKOverwrintingFile = true;
	}
	return true;
}

void SaveLevelInfo::on_FileDialogButton_clicked()
{
	// use QFileInfo to brush up the file name
	QFileInfo myFileInfo(ui.theFileNameField->text());

	isUserOKOverwrintingFile = false;

	QString myFileName = QFileDialog::getSaveFileName(this,
		tr("Save Level"), myFileInfo.absoluteFilePath(), tr("TBE levels (*.tbe *.xml)"));

	if (performFileExists(myFileName)==false)
		return;

	ui.theFileNameField->setText(myFileName);
}
