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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "ChoosePhoneNumber.h"
#include "ui_ChoosePhoneNumber.h"
#include "TriggerExplosion.h"


ChoosePhoneNumber::ChoosePhoneNumber(DetonatorBox* aBoxPtr, ResizingGraphicsView *aParentPtr) :
	AnimatedDialog(aParentPtr),
	m_ui(new Ui::ChoosePhoneNumber),
	theDBPtr(aBoxPtr)
{
	m_ui->setupUi(this);
	assert(theDBPtr!=nullptr);

	// all possible phone numbers are inserted below
	m_ui->comboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
	QStringList myPhoneList = theDBPtr->getAllPhoneNumbers();
	myPhoneList.sort();
	m_ui->comboBox->clear();
	m_ui->comboBox->addItems(myPhoneList);

	// and select the current item from the DetonatorBox
	QString myPhoneNumber = theDBPtr->getCurrentPhoneNumber();
	int myPhoneNrIndex = m_ui->comboBox->findText (myPhoneNumber, Qt::MatchExactly);
	m_ui->comboBox->setCurrentIndex(myPhoneNrIndex);

	connect(m_ui->pushButton_Cancel,SIGNAL(clicked()), this, SLOT(disappearAnimated()));
	connect(m_ui->pushButton_OK,SIGNAL(clicked()), this, SLOT(disappearAnimated()));
	this->adjustSize();
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
	DEBUG4("ChoosePhoneNumber::on_comboBox_activated - selected '%s'", ASCII(myLine));
	assert(myLine.isEmpty()==false);

	// let's notify the detonator box
	assert(theDBPtr!=nullptr);
	theDBPtr->setPhoneNumber(myLine);
}
