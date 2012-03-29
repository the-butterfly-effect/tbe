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

#ifndef ChooseLevel_H
#define ChooseLevel_H

#include "animateddialog.h"
#include "LocalString.h"

#include <QtGui/QDialog>
#include <QtGui/QTreeWidget>
#include <QtXml/QXmlDefaultHandler>

namespace Ui {
	class ChooseLevel;
}

class ChooseLevel : public AnimatedDialog {
	Q_OBJECT
public:
	ChooseLevel(ResizingGraphicsView *aParentPtr = 0, bool isNoShow=false);
	virtual ~ChooseLevel();

        /// TODO: document
        static QString getNextLevelName(void);

signals:
	void loadLevel(const QString&);

protected slots:
	void on_theTreeWidget_itemDoubleClicked()
	{ on_pushButton_go_clicked();}

	void on_pushButton_go_clicked();

	void on_pushButton_cancel_clicked();

protected:
	void changeEvent(QEvent *e);

        /// @returns the currently selected level, which should be the first
        /// not-conquered level. If there is none, returns an empty string.
        QString getCurrent(void);

private:
	Ui::ChooseLevel *m_ui;

	bool readLevels(const QString& aFileName);


	/// this class is used by ChooseLevel to build the TreeView
	class LevelList : public QXmlDefaultHandler
	{
	public:
		/// constructor - Note that the file name is specified on a different call
		LevelList(QTreeWidget* aListWidget);

		/// implemented from QXmlDefaultHandler
		bool endElement(const QString &namespaceURI, const QString &localName,
						const QString &qName);
		/// implemented from QXmlDefaultHandler
		bool characters(const QString &aStr)
		{ currentText += aStr; return true; }

		/// implemented from QXmlDefaultHandler
		bool fatalError(const QXmlParseException &exception);
		QString errorString() const
		{ return errorStr; }

	private:
		int		theNr;
		QTreeWidget* theTreeWidget;
		QString currentText;
		QString errorStr;
	}; // end-of-LevelList


	/** this class is used after the above LevelList class to quickly retrieve
	  * title and description from a level (in the right locale)
	  */
	class FastLevelParser : public QXmlDefaultHandler
	{
	public:
		/// empty constructor - Note that the file name is specified on a different call
		FastLevelParser(void) {};

		/// implemented from QXmlDefaultHandler
		virtual bool endElement(const QString &namespaceURI, const QString &localName,
						const QString &qName);

		/// implemented from QXmlDefaultHandler
		virtual bool characters(const QString &aStr)
		{ currentText += aStr; return true; }


		virtual bool startElement(const QString &namespaceURI, const QString &localName,
					   const QString &qName, const QXmlAttributes &attributes);

		QString errorString() const
		{ return errorStr; }

		LocalString theTitle;
		LocalString theDescription;

		QXmlAttributes theAttrs;

	private:
		QString currentText;
		QString errorStr;

	}; // end-of FastLevelParser
};

#endif // ChooseLevel_H
