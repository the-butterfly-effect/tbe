/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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

#ifndef DROPDOWNWINDOW_H
#define DROPDOWNWINDOW_H

#include <QtGui>

class ViewWorld;

class DropDownWindow : public QGraphicsWidget
{
public:
	explicit DropDownWindow(ViewWorld* aViewWorldPtr);

	void setup(QMenuBar* aMenuBarPtr);

signals:

public slots:
//	void hideYourself(void);
//	void showYourself(void);

private:
	QGraphicsWidget*       theToolBoxView;
	QGraphicsWidget*       pushButton;
	QGraphicsLinearLayout* theLayoutPtr;
	QGraphicsScene* theScenePtr;

	ViewWorld* theViewWorldPtr;

	/// used for the animation
	QStateMachine theStateMachine;
};

#endif // DROPDOWNWINDOW_H
