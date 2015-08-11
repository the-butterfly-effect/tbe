/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#include "ImageStore.h"
#include "Popup.h"
#include "PostItEditor.h"
#include "ui_PostItEditor.h"

#include <cassert>
#include <QPlainTextEdit>

PostItEditor::PostItEditor(BaseObject* aBaseObjectPtr, DrawPostIt* aDrawObject, QWidget *parent) :
    QDialog(parent),
	m_ui(new Ui::PostItEditor),
	theBaseObjectPtr (aBaseObjectPtr),
	theDrawObject(aDrawObject)
{
    m_ui->setupUi(this);
	fillTabs();

	connect (this, SIGNAL(accepted()), this, SLOT(slot_accepted()));

	// fix up the icons for the toolbuttons
	m_ui->toolButtonBold->setIcon(ImageRendererStore::getQIcon("IconBold", QSize(32,32)));
	m_ui->toolButtonItalics->setIcon(ImageRendererStore::getQIcon("IconItalics", QSize(32,32)));
	m_ui->toolButtonNewline->setIcon(ImageRendererStore::getQIcon("IconNewline", QSize(32,32)));

	theIsChangedIcon = ImageRendererStore::getQIcon("IconModified", QSize(32,32));
}

PostItEditor::~PostItEditor()
{
    delete m_ui;
}

void PostItEditor::changeEvent(QEvent *e)
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

void PostItEditor::fillTabs(void)
{
	int aPage=1;
	m_ui->tabWidget->clear();
	while(true)
	{
		QString myPageString = theBaseObjectPtr->theProps.
								 getPropertyNoDefault(pageNrToXmlArg(aPage));
		if (myPageString.isEmpty())
			break;

		QPlainTextEdit* myWidget = new QPlainTextEdit(myPageString);
		m_ui->tabWidget->addTab(myWidget, pageNrToTabString(aPage));
		connect (myWidget, SIGNAL(textChanged()), this, SLOT(slot_textChanged()));
		aPage++;
	}
}

QPlainTextEdit* PostItEditor::getCurrentEdit(void)
{
	return reinterpret_cast<QPlainTextEdit*>(m_ui->tabWidget->currentWidget());
}


void PostItEditor::on_toolButtonBold_clicked()
{
	// TODO/FIXME: if some block is selected, put <b> around it.

	//: dear translator, please only translate the BOLD word
	emit getCurrentEdit()->insertPlainText(tr("<b>BOLD</b>"));
}

void PostItEditor::on_toolButtonItalics_clicked()
{
	// TODO/FIXME: if some block is selected, put <i> around it.

	//: dear translator, please only translate the ITALICS word
	emit getCurrentEdit()->insertPlainText(tr("<i>ITALICS</i>"));
}

void PostItEditor::on_toolButtonMinus_clicked()
{
	if (m_ui->tabWidget->count()==0)
		return;

	//: dear translator, %1 will contain the contents of the tab, do not translate the <br>
	if (Popup::YesNoQuestion(tr(
			"Are you sure you want to remove the tab with text:<br>'%1'<br>")
			.arg(getCurrentEdit()->toPlainText()),
						 this)==false)
		return;

	int myCurrentTab = m_ui->tabWidget->currentIndex();
	m_ui->tabWidget->removeTab(myCurrentTab);
	reflowTabs(myCurrentTab);
	m_ui->tabWidget->setCurrentIndex(myCurrentTab);
}

void PostItEditor::on_toolButtonNewline_clicked()
{
	emit getCurrentEdit()->insertPlainText("<br>");
}

void PostItEditor::on_toolButtonPlus_clicked()
{
	QPlainTextEdit* myWidget = new QPlainTextEdit("new page");
	int myNewTab = m_ui->tabWidget->currentIndex()+1;
	m_ui->tabWidget->insertTab(myNewTab, myWidget, theIsChangedIcon, "temp");
	reflowTabs(myNewTab);
	m_ui->tabWidget->setCurrentIndex(myNewTab);
}

void PostItEditor::on_toolButtonTest_clicked()
{
	if (Popup::YesNoQuestion(tr(
			"In order to view the changes, we have to write the changes to"
			" the PostIt. This is non-reversible. Proceed?"), this)==false)
		return;
	serializeTabs();
	theDrawObject->displayPostit();
}

QString PostItEditor::pageNrToTabString(int aPage)
{
	return tr("Page %1").arg(aPage);
}

QString PostItEditor::pageNrToXmlArg(int aPage)
{
	return QString("page%1").arg(aPage);
}

void PostItEditor::reflowTabs(int aChangedIndex)
{
	for(int i=0; i < m_ui->tabWidget->count();i++)
	{
		m_ui->tabWidget->setTabText(i, pageNrToTabString(i+1));
		if (i>=aChangedIndex)
			m_ui->tabWidget->setTabIcon(i, theIsChangedIcon);
	}
}

void PostItEditor::serializeTabs(void)
{
	assert(theBaseObjectPtr!=NULL);

	// FIXME/TODO: this clear is very blunt - it erases all i18n!!!
	theBaseObjectPtr->theProps.clear();

	// write new contents to each changed page
	for(int i=0; i < m_ui->tabWidget->count(); i++)
	{
		QString myTabText = reinterpret_cast<QTextEdit*>(m_ui->tabWidget->widget(i))->toPlainText();
		bool myIsChanged = m_ui->tabWidget->tabIcon(i).isNull()==false;

		if (myIsChanged)
		{
			QString myKey = pageNrToXmlArg(i+1);

			// remove all translations for this page
			PropertyList::PropertyMap::const_iterator myP = theBaseObjectPtr->theProps.constBegin();
			while (myP != theBaseObjectPtr->theProps.constEnd())
			{
				// do iterators survive remove()??? apparently they do...
				if (myP.key().startsWith(myKey+"_"))
				{
					QString myTempKey = myP.key();
					++myP;
					theBaseObjectPtr->theProps.removeProperty(myTempKey);
					continue;
				}
				++myP;
			}
			// write the new (english only) key
			theBaseObjectPtr->theProps.setProperty(myKey, myTabText);
		}
		m_ui->tabWidget->setTabIcon(i,QIcon());
	}
}

void PostItEditor::slot_accepted()
{
	// check if some page was edited
	serializeTabs();
}

void PostItEditor::slot_textChanged()
{
	// this can only happen to the current text widget...
	m_ui->tabWidget->setTabIcon(m_ui->tabWidget->currentIndex(), theIsChangedIcon);
}
