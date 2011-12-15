#include "GameResources.h"
#include "Level.h"
#include "ui_GameResources.h"

#include "tbe_global.h"

#include <QAbstractAnimation>
#include <QAbstractTransition>
#include <QGraphicsView>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QStateMachine>
#include "ViewObject.h"
#include "ViewWorld.h"

const static int theDialogWidth  = 400;
const static int theDialogHeight = 250;


GameResources::GameResources(ResizingGraphicsView* aRSGVPtr) :
    QWidget(aRSGVPtr),
    ui(new Ui::GameResources),
    theLevelPtr(NULL),
    theViewWorldPtr(NULL),
    theParentPtr(aRSGVPtr)
{
    DEBUG1ENTRY;
    ui->setupUi(this);
}


GameResources::~GameResources()
{
    delete ui;
}


void GameResources::addAbstractObjectToToolbox(
        const LocalString& anObjectGroupName,
        AbstractObject* anAOPtr)
{
    DEBUG1ENTRY;
    // is it in an existing group already?
    // no? then create one
    ToolboxGroupList::iterator myI = theToolboxList.find(anObjectGroupName);
    if (myI == theToolboxList.end())
    {
        ToolboxGroup* myTGPtr = new ToolboxGroup(anObjectGroupName);
        myI = theToolboxList.insert(anObjectGroupName, myTGPtr);
    }

    // and let's add the object to it
    // it's not our responsibility to check for double entries
    myI.value()->addObject(anAOPtr);
}


void GameResources::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        // TODO/FIXME: add change of Description and Title to new language...
        break;
    default:
        break;
    }
}


void GameResources::parentResize(const QTransform& aTransformMatrix)
{
    ui->theToolView->setTransform(aTransformMatrix, false);
}


void GameResources::setLevelPtr(Level* aLevelPtr)
{
    DEBUG4ENTRY;
    theLevelPtr = aLevelPtr;

    ui->theLevelTitle->setText(theLevelPtr->theLevelName.result());
    //: translators: please do not try to translate the <b>%1</b> part!
    ui->theLevelAuthor->setText(tr("Level by: <b>%1</b>").arg(theLevelPtr->theLevelAuthor));
    ui->theInfoBox->setText(aLevelPtr->theLevelDescription.result());
}


void GameResources::setup(QMenuBar* aMenuBarPtr)
{
	setAutoFillBackground (true);


	// and the animation mumbo-jumbo
	//
	int myBigWidth = 600; //theParentPtr->width();
	int myBigHeight= theParentPtr->height();
	QState* myUpStatePtr = new QState(&theStateMachine);
	myUpStatePtr->assignProperty(this, "geometry", QRectF(
									 QPointF((myBigWidth-theDialogWidth)/2, -248.0),
									 QSizeF(theDialogWidth,theDialogHeight)));
	QState* myDownStatePtr = new QState(&theStateMachine);
	myDownStatePtr->assignProperty(this, "geometry", QRectF(
									QPointF((myBigWidth-theDialogWidth)/2,100.0),
									QSizeF(theDialogWidth,theDialogHeight)));
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


void GameResources::updateToolbox(void)
{
	DEBUG1ENTRY;

	theToolboxPtr = new QGraphicsScene(NULL);
	theToolboxPtr->setBackgroundBrush(Qt::blue);


	ToolboxGroupList::iterator i = theToolboxList.begin();
	while (i!=theToolboxList.end())
	{
		ViewObject* myVOPtr = i.value()->first()->createViewObject();
		qDebug() << myVOPtr;
		theToolboxPtr->addItem(myVOPtr);
		i++;
	}

	ui->theToolView->setScene(theToolboxPtr);
}
