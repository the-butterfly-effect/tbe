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

#include "ChooseLevel.h"
#include "ui_ChooseLevel.h"

ChooseLevel::ChooseLevel(QWidget *parent) :
    QDialog(parent),
	m_ui(new Ui::ChooseLevel)
{
    m_ui->setupUi(this);
	show();
}

ChooseLevel::~ChooseLevel()
{
    delete m_ui;
}

void ChooseLevel::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString ChooseLevel::getCurrent(void)
{
	QListWidgetItem* myItemPtr = m_ui->listWidget->currentItem();
	return myItemPtr->text().section(" ", 0,0);
}

