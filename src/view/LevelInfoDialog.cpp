/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "LevelInfoDialog.h"
#include "ui_LevelInfoDialog.h"

LevelInfoDialog::LevelInfoDialog(Level* aLevelPtr, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LevelInfoDialog)
{
    m_ui->setupUi(this);

	m_ui->labelTitle->setText(tr("Title: <b>%1</b>").arg(aLevelPtr->theLevelName.result()));
	m_ui->labelAuthor->setText(tr("Author: <b>%1</b>").arg(aLevelPtr->theLevelAuthor));
	m_ui->labelLicense->setText(tr("License: <b>%1</b>").arg(aLevelPtr->theLevelLicense));
	m_ui->labelCreationDate->setText(tr("Date: <b>%1</b>").arg(aLevelPtr->theLevelDate));
	m_ui->textBrowser->setText(aLevelPtr->theLevelDescription.result());
}

LevelInfoDialog::~LevelInfoDialog()
{
    delete m_ui;
}

void LevelInfoDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LevelInfoDialog::on_pushButton_clicked(void)
{
	// as far as I know, I'm allowed to do this, the destructor will notify the
	// parent (i.e. QGraphicsScene i.e. DrawWorld) of its imminent destruction.
	delete this;
}

