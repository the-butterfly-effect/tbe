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

#ifndef POSTITEDITOR_H
#define POSTITEDITOR_H

#include "BaseObject.h"
#include "DrawPostIt.h"
#include <QtGui/QDialog>
#include <QtGui/QIcon>

// forward declarations
class QPlainTextEdit;

namespace Ui {
    class PostItEditor;
}

/** This is the UI class for the Post-It editor
  * used to display and edit all tabs whilst editing Levels
  */
class PostItEditor : public QDialog {
    Q_OBJECT
public:
	PostItEditor(BaseObject* aBaseObjectPtr, DrawPostIt* aDrawObject, QWidget *parent = 0);
    ~PostItEditor();

	// this class has no further public members

protected:
	/// change the UI language (QT internal)
    void changeEvent(QEvent *e);

private:
	/// retrieve all content from the Post-It and fill the corresponding tabs
	void fillTabs(void);

	QString pageNrToTabString(int aPage);
	QString pageNrToXmlArg(int aPage);

	/// rewrite all tabs to have subsequent numbering
	void reflowTabs(int aChangedIndex);

	/// write all tab contents back to the Post-It
	void serializeTabs(void);

	/// @returns pointer to currently selected QPlainTextEdit
	QPlainTextEdit* getCurrentEdit(void);

private slots:
	void on_toolButtonBold_clicked();
	void on_toolButtonItalics_clicked();
	void on_toolButtonMinus_clicked();
	void on_toolButtonNewline_clicked();
	void on_toolButtonPlus_clicked();
	void on_toolButtonTest_clicked();

	/// overridden to actually write the results to the Post-It
	void slot_accepted();

	/// overridden to be able to mark text edits as changed
	void slot_textChanged();

private:
    Ui::PostItEditor *m_ui;
	BaseObject* theBaseObjectPtr;
	DrawPostIt* theDrawObject;

	QIcon theIsChangedIcon;
};

#endif // POSTITEDITOR_H
