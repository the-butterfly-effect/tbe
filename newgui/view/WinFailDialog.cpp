#include "WinFailDialog.h"
#include "ui_WinFailDialog.h"

WinFailDialog::WinFailDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinFailDialog)
{
    ui->setupUi(this);
}

WinFailDialog::~WinFailDialog()
{
    delete ui;
}

void WinFailDialog::changeEvent(QEvent *e)
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
