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

#include "animateddialog.h"
#include <QtCore/QMutex>
#include "tbe_global.h"

AnimatedDialog *AnimatedDialog::theCurrentlyViewedAnimatedDialog = nullptr;
static QMutex theAnimatedDialogMutex;

AnimatedDialog::AnimatedDialog(ResizingGraphicsView *aParentPtr,
                               AppearanceDirection anAppearDirection) :
    QWidget(aParentPtr),
    theYCoord(-1),
    theOurParentPtr(aParentPtr),
    theAnimation(nullptr, "geometry"),
    theIsToBeDeleted(false),
    theAppearanceDirection(anAppearDirection)
{
    DEBUG5ENTRY;
    theAnimation.setTargetObject(this);
    this->hide();
}

AnimatedDialog::~AnimatedDialog()
{
    // catch any dialogs that disappear unanimated
    theAnimatedDialogMutex.lock();
    if (theCurrentlyViewedAnimatedDialog == this)
        theCurrentlyViewedAnimatedDialog = nullptr;
    theAnimatedDialogMutex.unlock();
}

void AnimatedDialog::appearAnimated()
{
    // make the currently visible dialog go away
    theAnimatedDialogMutex.lock();
    AnimatedDialog *myADPtr = theCurrentlyViewedAnimatedDialog;
    theCurrentlyViewedAnimatedDialog = this;
    theAnimatedDialogMutex.unlock();
    if (myADPtr != nullptr)
        emit myADPtr->disappearAnimated();

    // setup the animation to appear
    const qreal DURATION = 800;
    emit show();
    if (theAnimation.state() == QPropertyAnimation::Running) {
        theAnimation.stop();
        theAnimation.setStartValue(theAnimation.currentValue());
    } else {
        theAnimation.setStartValue(QRectF(getOutsidePoint(), size()));
    }

    theAnimation.setEndValue(QRectF(getInsidePoint(), size()));
    switch (theAppearanceDirection) {
    case FROM_TOP:
    case FROM_TOPRIGHT:
    default:
        theAnimation.setDuration(DURATION);
        theAnimation.setEasingCurve(QEasingCurve::OutQuad);
        break;
    case TOOLTIP:
        theAnimation.setDuration(DURATION / 4);
        theAnimation.setEasingCurve(QEasingCurve::InQuad);
        break;
    }
    theAnimation.start();
    emit startedAppear();
    connect(   &theAnimation, SIGNAL(finished()), this, SLOT(slot_handleAppeared()));
    disconnect(&theAnimation, SIGNAL(finished()), this, SLOT(slot_handleDisappeared()));
}

void AnimatedDialog::disappearAnimated()
{
    // If we're still currently visible, delist ourselves.
    // Yes, I'm aware there is a potential race condition here.
    theAnimatedDialogMutex.lock();
    if (theCurrentlyViewedAnimatedDialog == this)
        theCurrentlyViewedAnimatedDialog = nullptr;
    theAnimatedDialogMutex.unlock();

    // setup disappear animation
    const qreal DURATION = 1000;
    if (theAnimation.state() == QPropertyAnimation::Running) {
        theAnimation.stop();
        theAnimation.setStartValue(theAnimation.currentValue());
    } else {
        theAnimation.setStartValue(QRectF(getInsidePoint(), size()));
    }
    theAnimation.setEndValue(QRectF(getOutsidePoint(), size()));
    switch (theAppearanceDirection) {
    default:
        theAnimation.setDuration(DURATION);
        break;
    case TOOLTIP:
        theAnimation.setDuration(DURATION / 4);
        break;
    }
    theAnimation.setEasingCurve(QEasingCurve::OutQuad);
    theAnimation.start();
    disconnect(&theAnimation, SIGNAL(finished()), this, SLOT(slot_handleAppeared()));
    connect(   &theAnimation, SIGNAL(finished()), this, SLOT(slot_handleDisappeared()));
}


QPointF AnimatedDialog::getInsidePoint() const
{
    switch (theAppearanceDirection) {
    case FROM_TOP:
    case FROM_TOPRIGHT:
        return QPointF((theOurParentPtr->width()  - width()) / 2,
                       (theOurParentPtr->height() - height()) / 2);
        break;
    case TOOLTIP:
        return QPointF(theOurParentPtr->width() - width(),
                       theYCoord);
        break;
    }
    return QPointF(0, 0);
}

QPointF AnimatedDialog::getOutsidePoint() const
{
    switch (theAppearanceDirection) {
    case FROM_TOP:
        return QPointF((theOurParentPtr->width() - width()) / 2,
                       -height());
        break;
    case FROM_TOPRIGHT:
        return QPointF(theOurParentPtr->width() + width() / 2,
                       -height());
        break;
    case TOOLTIP:
        return QPointF(theOurParentPtr->width(),
                       theYCoord);
        break;
    }
    return QPointF(0, 0);
}


void AnimatedDialog::slot_handleAppeared()
{
    // signal listeners that showing is complete
    emit appeared();
}

void AnimatedDialog::slot_handleDisappeared()
{
    // prevent dialog from taking resources
    emit hide();

    // signal listeners that we're gone
    emit disappeared();

    // are we also to delete this object after it is hidden?
    if (theIsToBeDeleted) {
        emit deleteLater();
    }
}


void AnimatedDialog::makeAllAnimatedDialogsDisappear()
{
    if (theCurrentlyViewedAnimatedDialog != nullptr)
        emit theCurrentlyViewedAnimatedDialog->disappearAnimated();
}
