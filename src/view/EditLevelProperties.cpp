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

#include "EditLevelProperties.h"
#include "ui_EditLevelProperties.h"

#include "Level.h"
#include "World.h"
#include "DrawWorld.h"

EditLevelProperties::EditLevelProperties(Level* aLevelPtr, QWidget *parent) :
    QDialog(parent),
	theLevelPtr(aLevelPtr),
    m_ui(new Ui::EditLevelProperties)
{
    m_ui->setupUi(this);
	m_ui->doubleSpinBoxHeight->setValue(aLevelPtr->theWorldPtr->getTheWorldHeight());
	m_ui->doubleSpinBoxWidth->setValue(aLevelPtr->theWorldPtr->getTheWorldWidth());

	connect (this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
}

EditLevelProperties::~EditLevelProperties()
{
    delete m_ui;
}

void EditLevelProperties::changeEvent(QEvent *e)
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


void EditLevelProperties::slot_accepted()
{
	// pass the values
	theLevelPtr->theWorldPtr->theWorldHeight = m_ui->doubleSpinBoxHeight->value();
	theLevelPtr->theWorldPtr->theWorldWidth = m_ui->doubleSpinBoxWidth->value();

	// and force the redraw...
	DrawWorld* myPtr = theLevelPtr->theWorldPtr->theDrawWorldPtr;
	if (myPtr)
		myPtr->drawOutlineAndBackground();
}
