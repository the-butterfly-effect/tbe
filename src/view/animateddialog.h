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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
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
    enum AppearanceDirection
    {
        FROM_TOP,
        FROM_TOPRIGHT,
        TOOLTIP
    };

    /// Constructor.
    /// @param anAppearDirection Defines where the origin of the inflying
    ///                          dialog is.
    explicit AnimatedDialog(ResizingGraphicsView* aParentPtr = nullptr,
                            AppearanceDirection anAppearDirection=FROM_TOP);
    ~AnimatedDialog();

    /// set the dialog to delete itself upon disappearance is complete
    void setDeleteOnDisappear(bool isToBeDeleted)
    { theIsToBeDeleted = isToBeDeleted; }

    static void makeAllAnimatedDialogsDisappear();

public slots:
    /// Animates this 'dialog' into the screen.
    /// Will also trigger all other dialogs to go away.
    /// @post Once completed, will emit appeared() signal.
    void appearAnimated();

    /// Disappear using animation.
    /// @post Also deletes the object if member variable
    ///       theIsToBeDeleted is true.
    /// @post Once completed, will emit the disappeared() signal.
    void disappearAnimated();

signals:
    /// This signal is emitted whenever the appear animation is started.
    /// Useful to handle if you need to make sure certain things have updated
    /// before it is shown.
    void startedAppear(void);

    /// This signal is emitted shortly after the appear animation has been
    /// completed.
    void appeared(void);

    /// This signal is emitted whenever the disappear animation is done.
    /// @note: The mechanism behind theIsToBeDeleted is implemented in this
    ///        fashion as well and will run immediately after this signal was
    ///        emitted. If Qt has a guaranteed order of delivery, you're safe.
    ///        Otherwise, I suggest you emit deleteLater() yourself and not
    ///        use the deleteOnDisappear() mechanism.
    void disappeared(void);

private slots:
    /// Handles everything that has to do with having appeared.
    /// This includes emitting the appeared() signal.
    void slot_handleAppeared(void);
    /// Handles everything that has to do with being disappeared.
    /// This includes delete-on-disappear, hiding to save resources and
    /// emitting the disappeared() signal.
    void slot_handleDisappeared(void);

protected:
    /// the Y coordinate where this dialog is to end.
    /// -1 means not set i.e. center of screen
    int theYCoord;

private:
    /// Returns the top-left point of the dialog as a starting point for the
    /// appearance animation (which is also the end point of the disappear).
    QPointF getOutsidePoint() const;

    /// Returns the top-left point of the dialog as a end point for the
    /// appearance animation (which is also the starting point of the disappear).
    QPointF getInsidePoint() const;

    /// Private pointer to our parent - which should at least be a QWidget
    /// but preferable a resizinggraphicsview...
    ResizingGraphicsView* theOurParentPtr;
    QPropertyAnimation theAnimation;

    bool theIsToBeDeleted;

    /// static pointer to currently visible
    static AnimatedDialog* theCurrentlyViewedAnimatedDialog;

    /// Where does the dialog fly in from / fly out to
    AppearanceDirection theAppearanceDirection;
};

#endif // ANIMATEDDIALOG_H
