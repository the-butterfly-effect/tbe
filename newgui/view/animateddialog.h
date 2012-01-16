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

#ifndef ANIMATEDDIALOG_H
#define ANIMATEDDIALOG_H

#include <QtCore/QPropertyAnimation>
#include <QtCore/QTimer>
#include <QtGui/QWidget>
#include "resizinggraphicsview.h"

/** This class is the base of all dialogs that appear _within_ the
  * resizinggraphicsview. It contains the animation and resizing logic.
  */
class AnimatedDialog : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatedDialog(ResizingGraphicsView* aParentPtr = NULL);

signals:

public slots:
    void appearAnimated();
    void disappearAnimated();

private:
    /// Private pointer to our parent - which should at least be a QWidget
    /// but preferable a resizinggraphicsview...
    ResizingGraphicsView* theOurParentPtr;
    QPropertyAnimation theAnimation;
    QTimer theHideTimer;
};

#endif // ANIMATEDDIALOG_H
