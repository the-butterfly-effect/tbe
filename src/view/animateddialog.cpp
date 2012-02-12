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
#include "tbe_global.h"

AnimatedDialog* AnimatedDialog::theCurrentlyViewedAnimatedDialog = NULL;
static QMutex theAnimatedDialogMutex;

AnimatedDialog::AnimatedDialog(ResizingGraphicsView* aParentPtr) :
    QWidget(aParentPtr),
    theOurParentPtr(aParentPtr),
    theAnimation(NULL, "geometry"),
    theIsToBeDeleted(false)
{
    DEBUG1("entry: %s = %p\n", Q_FUNC_INFO, this);
    theAnimation.setTargetObject(this);
    this->hide();
}

AnimatedDialog::~AnimatedDialog()
{
	DEBUG1("exit: %s = %p\n", Q_FUNC_INFO, this);
	// catch any dialogs that disappear unanimated
	theAnimatedDialogMutex.lock();
	if (theCurrentlyViewedAnimatedDialog==this)
		theCurrentlyViewedAnimatedDialog=NULL;
	theAnimatedDialogMutex.unlock();
}

void AnimatedDialog::appearAnimated()
{
	// make the currently visible dialog go away
	theAnimatedDialogMutex.lock();
	AnimatedDialog* myADPtr = theCurrentlyViewedAnimatedDialog;
	theCurrentlyViewedAnimatedDialog = this;
	theAnimatedDialogMutex.unlock();
	if (myADPtr!=NULL)
		emit myADPtr->disappearAnimated();

	// setup the animation to appear
	const qreal DURATION=1500;
	if (theAnimation.state()==QPropertyAnimation::Running)
	{
		theAnimation.stop();
		theAnimation.setStartValue(theAnimation.currentValue());
	}
	else
	{
		theAnimation.setStartValue(QRectF(
										  QPointF((theOurParentPtr->width() - width())/2,
												  -height()), size()));
	}
	theAnimation.setEndValue(QRectF(
									QPointF((theOurParentPtr->width()  - width())/2,
											(theOurParentPtr->height() - height())/2),
									size()));
	theAnimation.setDuration(DURATION);
	theAnimation.setEasingCurve(QEasingCurve::OutBounce);
	theAnimation.start();
	emit show();
	emit startedAppear();
	connect(   &theAnimation, SIGNAL(finished()), this, SLOT(slot_handleAppeared()));
	disconnect(&theAnimation, SIGNAL(finished()), this, SLOT(slot_handleDisappeared()));
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
	if (theAnimation.state()==QPropertyAnimation::Running)
	{
		theAnimation.stop();
		theAnimation.setStartValue(theAnimation.currentValue());
	}
	else
	{
		theAnimation.setStartValue(QRectF(
									   QPointF((theOurParentPtr->width()  - width())/2,
											   (theOurParentPtr->height() - height())/2),
									   size()));
	}
	theAnimation.setEndValue(QRectF(
								 QPointF((theOurParentPtr->width() - width())/2,
										 -height()), size()));
	theAnimation.setDuration(DURATION);
	theAnimation.setEasingCurve(QEasingCurve::OutQuad);
	theAnimation.start();
	disconnect(&theAnimation, SIGNAL(finished()), this, SLOT(slot_handleAppeared()));
	connect(   &theAnimation, SIGNAL(finished()), this, SLOT(slot_handleDisappeared()));
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
	if (theIsToBeDeleted)
	{
		emit deleteLater();
	}
}


void AnimatedDialog::makeAllAnimatedDialogsDisappear()
{
	if (theCurrentlyViewedAnimatedDialog!=NULL)
		emit theCurrentlyViewedAnimatedDialog->disappearAnimated();
}
