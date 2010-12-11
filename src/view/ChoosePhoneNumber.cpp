/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "ChoosePhoneNumber.h"
#include "ui_ChoosePhoneNumber.h"
#include "TriggerExplosion.h"


ChoosePhoneNumber::ChoosePhoneNumber(DetonatorBox* aBoxPtr, QWidget *parent) :
    QWidget(parent),
	m_ui(new Ui::ChoosePhoneNumber),
	theDBPtr(aBoxPtr)
{
    m_ui->setupUi(this);
	assert(theDBPtr!=NULL);

	// the top location gets the currently selected phone#.
	// always insert it - even if it is an empty string!
	QString myFirst = theDBPtr->getCurrentPhoneNumber();
	m_ui->comboBox->insertItem(0, myFirst);

	// all remaining possible phone numbers are inserted below
	QStringList myPhoneList = theDBPtr->getAllPhoneNumbers();
	int mySetNumber = myPhoneList.indexOf(myFirst);
	myPhoneList.removeAt(mySetNumber);
	m_ui->comboBox->insertItems(1, myPhoneList);

	connect(m_ui->pushButton_Cancel,SIGNAL(clicked()), this, SLOT(close()));
	connect(m_ui->pushButton_OK,SIGNAL(clicked()), this, SLOT(close()));
}

ChoosePhoneNumber::~ChoosePhoneNumber()
{
    delete m_ui;
}

void ChoosePhoneNumber::changeEvent(QEvent *e)
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

void ChoosePhoneNumber::on_comboBox_activated()
{
	// is the empty line selected?
	QString myLine = m_ui->comboBox->currentText();
	DEBUG4("ChoosePhoneNumber::on_comboBox_activated - selected '%s'\n", ASCII(myLine));
	if (myLine.isEmpty())
		return;

	// if there is an empty entry in the list and it is not activated anymore,
	// let's remove it!
	if (m_ui->comboBox->itemText(0).isEmpty())
		m_ui->comboBox->removeItem(0);

	// let's notify the detonator box
	assert(theDBPtr!=NULL);
	theDBPtr->setPhoneNumber(myLine);

	emit close();
}
