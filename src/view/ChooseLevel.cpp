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

#include <QtCore/QCoreApplication>
#include <QtXml/QXmlSimpleReader>
#include <QtCore/QSettings>
#include <QtGui/QTreeWidgetItemIterator>

static const int  NR_COLUMN=0;
static const int  TITLE_COLUMN=1;
static const int  FILENAME_COLUMN=2;


ChooseLevel::ChooseLevel(ResizingGraphicsView *aParentPtr, bool isNoShow) :
	AnimatedDialog(aParentPtr),
	m_ui(new Ui::ChooseLevel)
{
	m_ui->setupUi(this);
	readLevels( LEVELS_DIRECTORY + "/levels.xml" );
	if (!isNoShow)
		appearAnimated();
}

ChooseLevel::~ChooseLevel()
{
	delete m_ui;
}

void ChooseLevel::changeEvent(QEvent *e)
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


QString ChooseLevel::getCurrent(void)
{
	QTreeWidgetItem* myItemPtr = m_ui->theTreeWidget->currentItem();
	if (myItemPtr==NULL)
		return "";
	return myItemPtr->text(FILENAME_COLUMN);
}


QString ChooseLevel::getNextLevelName(void)
{
    ChooseLevel myDialog(NULL, true);
    QString myNextLevelName = myDialog.getCurrent();
    if (myNextLevelName.isEmpty()==false)
        return myNextLevelName;

    // TODO/FIXME: fix this - we better always return a name here!
    return NULL;
}



void ChooseLevel::on_pushButton_go_clicked()
{
	emit loadLevel(getCurrent());
	disappearAnimated();
}


void ChooseLevel::on_pushButton_cancel_clicked()
{
	disappearAnimated();
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
                    QString myLineStatus = (*it)->text(NR_COLUMN);
                    if ( myLineStatus.contains(QRegExp("[0-9]")) )
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
		currentText = LEVELS_DIRECTORY + "/" + currentText.trimmed();

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
			item->setText(TITLE_COLUMN, myParser.theTitle.result());
		if (myParser.theDescription.isEmpty()==false)
			item->setToolTip(TITLE_COLUMN, myParser.theDescription.result());

		QSettings mySettings;
                QString myLevelStatus = mySettings.value("completed/"+currentText).toString();
                if (myLevelStatus.isEmpty()==false)
                {
                    if (myLevelStatus=="done")
                        item->setText(NR_COLUMN, tr("done"));
                    else if (myLevelStatus=="skipped")
                        item->setText(NR_COLUMN, tr("skipped"));
                    else
                        item->setText(NR_COLUMN, tr("???"));
                }
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
		theTitle.add(currentText.trimmed(), theAttrs.value("lang"));

	if (qName == "description")
		theDescription.add(currentText.trimmed(), theAttrs.value("lang"));

	// no need to parse everything beyond the levelinfo
	if (qName == "levelinfo")
		return false;

	currentText = "";
	return true;
}


bool ChooseLevel::FastLevelParser::startElement(const QString &, const QString &,
					   const QString &qName, const QXmlAttributes &attributes)
{
	// this potentially is an expensive copy operation, let's reduce it...
	if (qName == "title" || qName == "description")
	{
		theAttrs = attributes;
	}
	return true;
}
