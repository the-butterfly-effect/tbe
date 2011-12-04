#include <QPainter>
#include <QPen>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

#include "Overlay.h"
#include "ui_Overlay.h"

#include <cassert>

Overlay::Overlay(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Overlay)
{
    ui->setupUi(this);
    QPixmap myPixmap("/home/kaa-ching/Programming/tbe/trunk/images/icons/statusPlay.png");
    assert(myPixmap.isNull()==false);
    ui->statusLabel->setPixmap(myPixmap);
}

Overlay::~Overlay()
{
    delete ui;
}

QToolButton* Overlay::getFFPtr()
{
    return ui->pauseButton;
}

QToolButton* Overlay::getPlayPtr()
{
    return ui->playButton;
}
QToolButton* Overlay::getResetPtr()
{
    return ui->resetButton;
}


void Overlay::parentResize(const QSize& aSize)
{
    // TODO/FIXME: magic numbers here
    // I bet these have to be different for Windows and MacOSX :-(
    move(aSize.width()-size().width()+2,-6);
}
