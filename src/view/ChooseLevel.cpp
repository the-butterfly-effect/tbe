/* The Butterfly Effect
 * This file copyright (C) 2009,2015 Klaas van Gend
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

#include "ChooseLevel.h"
#include "Popup.h"
#include "Translator.h"
#include "ui_ChooseLevel.h"
#include "tbe_paths.h"

#include <QCoreApplication>
#include <QXmlSimpleReader>
#include <QSettings>
#include <QTreeWidgetItemIterator>

static const int  NR_COLUMN=0;
static const int  TITLE_COLUMN=1;
static const int  FILENAME_COLUMN=2;

std::shared_ptr<LevelList> ChooseLevel::theLLPtr = nullptr;

ChooseLevel::ChooseLevel(ResizingGraphicsView *aParentPtr, bool isNoShow,
						 const QString& aDirectory, const QString& aFileName) :
	AnimatedDialog(aParentPtr),
	m_ui(new Ui::ChooseLevel)
{
    DEBUG1ENTRY;
	m_ui->setupUi(this);
	theLLPtr = std::make_shared<LevelList>(aDirectory, aFileName);
	if (!isNoShow)
	{
		fillTreeWidget();
		appearAnimated();
	}
}


ChooseLevel::~ChooseLevel()
{
	DEBUG1ENTRY;
	delete m_ui;
}


void ChooseLevel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
		fillTreeWidget();
        break;
    default:
        break;
    }
}


void ChooseLevel::fillTreeWidget()
{
	m_ui->theTreeWidget->clear();
	bool isFirst = true;
	bool hasSkipped = false;
	QString myNextName = theLLPtr->getFirstLevel();
	int myNr = 0;
	QTreeWidgetItem* item;
	QTreeWidgetItem* firstSkipped;
	do {
		++myNr;
		item = new QTreeWidgetItem(m_ui->theTreeWidget);
		LevelList::LevelMetaInfo myMeta =theLLPtr->getLevelMetaInfo(myNextName);
		item->setText(FILENAME_COLUMN, myMeta.theFileName);
        item->setText(TITLE_COLUMN, TheGetText(myMeta.theTitle));
        item->setToolTip(TITLE_COLUMN, TheGetText(myMeta.theDescription));
		switch(myMeta.theStatus)
		{
		case LevelList::LevelMetaInfo::COMPLETED:
			item->setText(NR_COLUMN, tr("done"));
			break;
		case LevelList::LevelMetaInfo::SKIPPED:
			item->setText(NR_COLUMN, tr("skipped"));
			if (!hasSkipped)
			{
				hasSkipped = true;
				firstSkipped = item;
			}
			break;
		default:
			item->setText(NR_COLUMN, QString::number(myNr));
			if (isFirst)
				m_ui->theTreeWidget->setCurrentItem(item);
			isFirst = false;
			break;
		}

		myNextName = theLLPtr->getNextLevel(myNextName);
	} while (!myNextName.isEmpty() );
	/* No item has been selected (happens when all levels done/skipped)  */
	if (isFirst)
	{
		/* Player has some skipped levels remaining: Select first skipped level */
		if (hasSkipped)
			m_ui->theTreeWidget->setCurrentItem(firstSkipped);
		/* Player has finished all levels: Select last level */
		else
			m_ui->theTreeWidget->setCurrentItem(item);
	}
}

QString ChooseLevel::getCurrent(void)
{
	DEBUG1ENTRY;
	QTreeWidgetItem* myItemPtr = m_ui->theTreeWidget->currentItem();
	if (myItemPtr==nullptr)
		return "";
	return myItemPtr->text(FILENAME_COLUMN);
}


QString ChooseLevel::getNextLevelName()
{
	if(theLLPtr==nullptr)
		theLLPtr = std::make_shared<LevelList>(LEVELS_DIRECTORY, "levels.xml");
	return theLLPtr->getNextToPlayLevel();
}


void ChooseLevel::on_pushButton_go_clicked()
{
	DEBUG1ENTRY;
	emit loadLevel(getCurrent());
	disappearAnimated();
}


void ChooseLevel::on_pushButton_cancel_clicked()
{
	disappearAnimated();
}

