#include "DropDownWindow.h"

DropDownWindow::DropDownWindow(QGraphicsWidget* parent) :
	QGraphicsWidget(parent)
{
}

void DropDownWindow::setup(QMenuBar* aMenuBarPtr)
{
	setAutoFillBackground (true);

	// take all palette options from menu bar, so we match
	// except for the background of the window :-)
	QPalette myPal = aMenuBarPtr->palette();
	myPal.setColor(QPalette::Window, Qt::darkRed);
	setPalette(myPal);

	textEdit = scene()->addWidget(new QTextEdit("hello hello"));
	pushButton = scene()->addWidget(new QPushButton("test"));

	theLayoutPtr = new QGraphicsLinearLayout;
	theLayoutPtr->addItem(textEdit);
	theLayoutPtr->addItem(pushButton);
	setLayout(theLayoutPtr);
	setZValue(1000);


	// and the animation mumbo-jumbo
	//
	QState* myUpStatePtr = new QState(&theStateMachine);
	myUpStatePtr->assignProperty(this, "geometry", QRectF(QPointF(80, -380), QSizeF(0, 40)));
	QState* myDownStatePtr = new QState(&theStateMachine);
	myDownStatePtr->assignProperty(this, "geometry", QRectF(QPointF(80,-200), QSizeF(0,0)));
	theStateMachine.setInitialState(myUpStatePtr);

	// setup a custom animation
	QPropertyAnimation* myAnimationPtr = new QPropertyAnimation(this, "geometry");
	myAnimationPtr->setDuration(1000);
	myAnimationPtr->setEasingCurve(QEasingCurve::OutBounce);

	// setup a custom animation
	QPropertyAnimation* myUpAnimationPtr = new QPropertyAnimation(this, "geometry");
	myUpAnimationPtr->setDuration(1000);
	myUpAnimationPtr->setEasingCurve(QEasingCurve::OutQuad);

	// and trigger the state changes with the above animation
	QAction* myDownActionPtr = aMenuBarPtr->addAction(tr("&Down"));
	QAbstractTransition* myDownTransitionPtr = myUpStatePtr->addTransition(myDownActionPtr, SIGNAL(triggered()), myDownStatePtr);
	myDownTransitionPtr->addAnimation(myAnimationPtr);

	QAction* myUpActionPtr = aMenuBarPtr->addAction(tr("&Up"));
	QAbstractTransition *myUpTransitionPtr = myDownStatePtr->addTransition(myUpActionPtr, SIGNAL(triggered()), myUpStatePtr);
	myUpTransitionPtr->addAnimation(myUpAnimationPtr);

	// finally, let's get this baby rolling!!!
	theStateMachine.start();
}
