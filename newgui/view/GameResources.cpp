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
#include "ViewToolboxGroup.h"
#include "ViewWorld.h"

GameResources::GameResources(ResizingGraphicsView* aRSGVPtr) :
    QWidget(aRSGVPtr),
    ui(new Ui::GameResources),
    theLevelPtr(NULL),
    theViewWorldPtr(NULL),
    theParentPtr(aRSGVPtr)
{
    DEBUG1ENTRY;
    ui->setupUi(this);
    setAutoFillBackground (true);
}


GameResources::~GameResources()
{
    delete ui;
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
    // Set the transform of the toolview to be the same as the
    // parent (i.e. resizinggraphicsview).
    // This enforces that objects will have the same size as in the scene.
    ui->theToolView->setTransform(aTransformMatrix, false);

    // And move the dialog to the center of the parent
    QSize myParentSize = theParentPtr->size();
    this->move( (myParentSize.width()-width())/2, (myParentSize.height()-height())/2);
}


void GameResources::setLevelPtr(Level* aLevelPtr)
{
    DEBUG4ENTRY;
    Q_ASSERT(aLevelPtr!=NULL);
    theLevelPtr = aLevelPtr;

    ui->theLevelTitle->setText(theLevelPtr->theLevelName.result());
    //: translators: please do not try to translate the <b>%1</b> part!
    ui->theLevelAuthor->setText(tr("Level by: <b>%1</b>").arg(theLevelPtr->theLevelAuthor));
    ui->theInfoBox->setText(aLevelPtr->theLevelDescription.result());

	theToolboxPtr = new QGraphicsScene(NULL);
	theToolboxPtr->setBackgroundBrush(Qt::blue);
	ui->theToolView->setScene(theToolboxPtr);

	Level::ToolboxGroupList::iterator i = theLevelPtr->theToolboxList.begin();
	int dy = 0;
	while (i!=theLevelPtr->theToolboxList.end())
	{
		ViewToolboxGroup* myVTGPtr = new ViewToolboxGroup(i.value());
		myVTGPtr->moveBy(0,dy);
		theToolboxPtr->addItem(myVTGPtr);
		dy += myVTGPtr->getBigHeight();
		i++;
	}
}

void GameResources::on_theOKButton_clicked()
{
	emit hideMe();
}
