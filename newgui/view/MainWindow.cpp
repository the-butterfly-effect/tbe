#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore>
#include <QtGui>
#include <cassert>


#include "DropDownWindow.h"
#include "AbstractObject.h"
#include "ViewObject.h"

class QGraphicsRectWidget : public QGraphicsWidget
{
public:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
			   QWidget *)
	{
		painter->fillRect(rect(), Qt::blue);
	}
};





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	theScenePtr(NULL),
	theDropDown(NULL)
{
    ui->setupUi(this);

	setupMenu();
	setupView();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::setupMenu(void)
{
}

void MainWindow::setupView()
{
	assert(theScenePtr==NULL);
	theScenePtr = new QGraphicsScene(0, -300, 300, 300);
	theScenePtr->setBackgroundBrush(Qt::blue);
	theScenePtr->addRect(0,0,300,-300);
	ui->graphicsView->setScene(theScenePtr);

	DropDownWindow* theDropDown = new DropDownWindow;
	theScenePtr->addItem(theDropDown);
	theDropDown->setup(ui->menuBar);

	AbstractObject* theAOPtr = new AbstractObject();
	theAOPtr->setTheHeight(25);
	theAOPtr->setTheWidth(25);
	theAOPtr->setOrigCenter(Position(40,40,1.57));
	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr);

	// and prove that rotation doesn't work as it should:
	AbstractObject* theAOPtr2 = new AbstractObject();
	theAOPtr2->setTheHeight(25);
	theAOPtr2->setTheWidth(25);
	theAOPtr2->setOrigCenter(Position(40,40,0));
	ViewObject* theVOPtr2 = new ViewObject(theAOPtr2, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr2);

}
