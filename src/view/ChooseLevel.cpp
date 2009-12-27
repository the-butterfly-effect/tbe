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
#include "Popup.h"
#include "ui_ChooseLevel.h"
#include "tbe_global.h"

#include <QXmlSimpleReader>
#include <QSettings>
#include <QTreeWidgetItemIterator>

static const int  NR_COLUMN=0;
static const int  TITLE_COLUMN=1;
static const int  FILENAME_COLUMN=2;


ChooseLevel::ChooseLevel(QWidget *parent) :
    QDialog(parent),
	m_ui(new Ui::ChooseLevel)
{
    m_ui->setupUi(this);
	// TODO/FIXME: hardcoded file name here
	readLevels("levels/levels.xml");
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
	QTreeWidgetItem* myItemPtr = m_ui->theTreeWidget->currentItem();
	return myItemPtr->text(FILENAME_COLUMN);
}



bool ChooseLevel::readLevels(const QString& aFileName )
{
	LevelList handler(m_ui->theTreeWidget);
	QXmlSimpleReader reader;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);

	QFile file(aFileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		// note that critical won't return...
		Popup::Critical(tr("Level parser:\n"
						   "Cannot read the level descriptions in '%1':\n%2.")
						.arg(aFileName).arg(file.errorString()), this);
		return false;
	}

	QXmlInputSource xmlInputSource(&file);
	if (reader.parse(xmlInputSource))
	{
		// find the first item that not has "done" in the NR_COLUMN field
		QTreeWidgetItemIterator it(m_ui->theTreeWidget);
		while( *it )
		{
			if ( (*it)->text(NR_COLUMN) != "done")
			{
				m_ui->theTreeWidget->setCurrentItem(*it);
				(*it)->setSelected(true);
				break;
			}
			++it;
		}
		return true;
	}
	return false;
}

// ###################################################


 ChooseLevel::LevelList::LevelList(QTreeWidget* aTreeWidget)
 {
	 theNr = 0;
	 theTreeWidget =  aTreeWidget;
	 currentText.clear();
 }

bool ChooseLevel::LevelList::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
	if (qName == "level")
	{
		// remove any starting/trailing whitespace and add the path name
		currentText = "levels/" + currentText.trimmed();

		QTreeWidgetItem* item = new QTreeWidgetItem(theTreeWidget);
		item->setText(NR_COLUMN, QString::number(++theNr));
		item->setText(FILENAME_COLUMN, currentText);

		// read Level title and Level description from the level file
		FastLevelParser myParser;
		QXmlSimpleReader reader;
		reader.setContentHandler(&myParser);
		reader.setErrorHandler(&myParser);
		QFile file(currentText);
		if (!file.open(QFile::ReadOnly | QFile::Text))
			goto RETURN;

		QXmlInputSource xmlInputSource(&file);
		reader.parse(xmlInputSource);
		if (myParser.theTitle.isEmpty()==false)
			item->setText(TITLE_COLUMN, myParser.theTitle);
		if (myParser.theDescription.isEmpty()==false)
			item->setToolTip(TITLE_COLUMN, myParser.theDescription);

		QSettings mySettings;
		if (mySettings.value("completed/"+currentText).isValid())
			item->setText(NR_COLUMN, "done");
	}
RETURN:
	currentText.clear();
	return true;
}

bool ChooseLevel::LevelList::fatalError(const QXmlParseException &exception)
{
	Popup::Critical(tr("LevelList:\nParse error at line %1, column %2:\n%3")
					.arg(exception.lineNumber())
					.arg(exception.columnNumber())
					.arg(exception.message()), theTreeWidget);
	return false;
}


// ###################################################

bool ChooseLevel::FastLevelParser::endElement(const QString & /* namespaceURI */,
											  const QString & /* localName */,
											  const QString &qName)
{
	if (qName == "title")
		theTitle = currentText.trimmed();
	if (qName == "description")
		theDescription = currentText.trimmed();

	// no need to parse everything beyond the levelinfo
	if (qName == "levelinfo")
		return false;
	return true;
}
