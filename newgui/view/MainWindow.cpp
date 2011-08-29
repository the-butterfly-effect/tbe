#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore>
#include <QtGui>
#include <cassert>


#include "DropDownWindow.h"
#include "BaseObject.h"
#include "DrawObject.h"

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
	theScenePtr = new QGraphicsScene(0, 0, 300, 300);
	theScenePtr->setBackgroundBrush(Qt::black);
	ui->graphicsView->setScene(theScenePtr);

	DropDownWindow* theDropDown = new DropDownWindow;
	theScenePtr->addItem(theDropDown);
	theDropDown->setup(ui->menuBar);

	BaseObject* theBBPtr = new BaseObject();
	DrawObject* theDOPtr = new DrawObject(theBBPtr, "BowlingBall");
	theScenePtr->addItem(theDOPtr);
}
