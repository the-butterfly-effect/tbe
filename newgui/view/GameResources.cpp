#include "GameResources.h"
#include "ui_GameResources.h"

GameResources::GameResources(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameResources)
{
    ui->setupUi(this);
}

GameResources::~GameResources()
{
    delete ui;
}

void GameResources::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
