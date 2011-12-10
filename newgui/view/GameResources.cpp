#include "GameResources.h"
#include "ui_GameResources.h"

#include "tbe_global.h"

#include <QAbstractAnimation>
#include <QAbstractTransition>
#include <QGraphicsView>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QStateMachine>


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


void GameResources::setLevelPtr(Level* aLevelPtr)
{
    DEBUG4ENTRY;
    theLevelPtr = aLevelPtr;
}


void GameResources::setup(QMenuBar* aMenuBarPtr)
{
	setAutoFillBackground (true);

	// take all palette options from menu bar, so we match
	// except for the background of the window :-)
	QPalette myPal = aMenuBarPtr->palette();
	myPal.setColor(QPalette::Window, Qt::darkRed);
	setPalette(myPal);

	{
//	theScenePtr = new QGraphicsScene(0, -100, 100, 100);
//	theScenePtr->setBackgroundBrush(Qt::white);
//	theScenePtr->addRect(0,0,100,-100);

//	AbstractObject* theAOPtr = new AbstractObject();
//	theAOPtr->setTheHeight(45);
//	theAOPtr->setTheWidth(45);
//	theAOPtr->setOrigCenter(Position(30,30, 0));
//	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
//	theScenePtr->addItem(theVOPtr);
	}


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
