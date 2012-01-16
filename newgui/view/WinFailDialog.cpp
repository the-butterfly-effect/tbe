/* The Butterfly Effect
 * This file copyright (C) 2012 Klaas van Gend
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

#include "ImageCache.h"
#include "MainWindow.h"
#include "WinFailDialog.h"
#include "ui_WinFailDialog.h"

WinFailDialog::WinFailDialog(MessageType aType,
                             ResizingGraphicsView* aParent) :
    AnimatedDialog(aParent),
    ui(new Ui::WinFailDialog),
    theParentPtr(aParent)
{
    ui->setupUi(this);
    ui->replayButton->setIcon(ImageCache::getQIcon("ActionUndo", QSize(32,32)));

	QString myMessage;
	switch(aType)
	{
		case CONGRATS:
			//: make sure the translated text fits - the rest won't be shown
			myMessage = tr("Congratulations");
			ui->nextButton->setEnabled(true);
			break;
		case DEATH:
			//: make sure the translated text fits - the rest won't be shown
			myMessage = tr("Fail - retry?");
			ui->nextButton->setEnabled(false);
			ui->replayButton->setText(tr("Retry"));
			break;
	}
	ui->label->setText(myMessage);
	connect(ui->replayButton, SIGNAL(clicked()), theParentPtr, SLOT(slot_actionReplay()));
	connect(ui->chooseButton, SIGNAL(clicked()), theParentPtr, SLOT(slot_actionChooseLevel()));
	connect(ui->nextButton,   SIGNAL(clicked()), theParentPtr, SLOT(slot_actionNextLevel()));
}

WinFailDialog::~WinFailDialog()
{
	delete ui;
}

void WinFailDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
