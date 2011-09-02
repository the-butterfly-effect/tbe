#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore>
#include <QtGui>
#include <cassert>


#include "DropDownWindow.h"
#include "AbstractObject.h"
#include "ViewObject.h"

#include "SimulationControls.h"

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
	theAOPtr->setTheHeight(20);
	theAOPtr->setTheWidth(20);
	theAOPtr->setOrigCenter(Position(40,40,1.57));
	ViewObject* theVOPtr = new ViewObject(theAOPtr, "../images/QuarterArc.png");
	theScenePtr->addItem(theVOPtr);

	theScenePtr->addRect(30,-50,20,20);

	SimulationControls myControls;
	myControls.setup(ui->menuBar);
}
