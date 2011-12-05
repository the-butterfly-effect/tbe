#include <QPainter>
#include <QPen>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "ImageCache.h"
#include "Overlay.h"
#include "ui_Overlay.h"

#include <cassert>

Overlay::Overlay(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Overlay)
{
    ui->setupUi(this);

    QPixmap myPixmap;
    ImageCache::getPixmap("StatusPlay", QSize(64,64), &myPixmap);
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

QLabel* Overlay::getStatusLabelPtr()
{
    return ui->statusLabel;
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
    move(aSize.width()-size().width()+6,-16);
}
