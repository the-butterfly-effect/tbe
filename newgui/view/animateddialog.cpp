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

#include "animateddialog.h"
#include <QtCore/QMutex>

AnimatedDialog* AnimatedDialog::theCurrentlyViewedAnimatedDialog = NULL;
static QMutex theAnimatedDialogMutex;

AnimatedDialog::AnimatedDialog(ResizingGraphicsView* aParentPtr) :
    QWidget(aParentPtr),
    theOurParentPtr(aParentPtr),
    theAnimation(NULL, "geometry"),
    theIsToBeDeleted(false)
{
    theHideTimer.setSingleShot(true);
    connect(&theHideTimer, SIGNAL(timeout()), this, SLOT(hide()));
}


void AnimatedDialog::appearAnimated()
{
	theHideTimer.stop();

	// make the currently visible dialog go away
	theAnimatedDialogMutex.lock();
	AnimatedDialog* myADPtr = theCurrentlyViewedAnimatedDialog;
	theCurrentlyViewedAnimatedDialog = this;
	theAnimatedDialogMutex.unlock();
	if (myADPtr!=NULL)
		emit myADPtr->disappearAnimated();

	// setup the animation to appear
	theAnimation.setTargetObject(this);
	theAnimation.setStartValue(QRectF(
									  QPointF((theOurParentPtr->width() - width())/2,
											  -height()), size()));
	theAnimation.setEndValue(QRectF(
									QPointF((theOurParentPtr->width()  - width())/2,
											(theOurParentPtr->height() - height())/2),
									size()));
	theAnimation.setDuration(1500);
	theAnimation.setEasingCurve(QEasingCurve::OutBounce);
	theAnimation.start();
	emit show();
}

void AnimatedDialog::disappearAnimated()
{
	// If we're still currently visible, delist ourselves.
	// Yes, I'm aware there is a potential race condition here.
	theAnimatedDialogMutex.lock();
	if (theCurrentlyViewedAnimatedDialog==this)
		theCurrentlyViewedAnimatedDialog=NULL;
	theAnimatedDialogMutex.unlock();

	// setup disappear animation
	const qreal DURATION=1000;
	theAnimation.setTargetObject(this);
	theAnimation.setStartValue(QRectF(
								   QPointF((theOurParentPtr->width()  - width())/2,
										   (theOurParentPtr->height() - height())/2),
								   size()));
	theAnimation.setEndValue(QRectF(
								 QPointF((theOurParentPtr->width() - width())/2,
										 -height()), size()));
	theAnimation.setDuration(DURATION);
	theAnimation.setEasingCurve(QEasingCurve::OutQuad);
	theAnimation.start();

	// are we also to delete this object after it is hidden?
	if (theIsToBeDeleted)
	{
		connect(&theHideTimer, SIGNAL(timeout()), this, SLOT(deleteLater()));
	}

	// hide (and maybe delete) slightly later than the animation ends
	theHideTimer.start(DURATION*1.05);
}
