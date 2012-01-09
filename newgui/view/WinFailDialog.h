#ifndef WINFAILDIALOG_H
#define WINFAILDIALOG_H

#include <QWidget>

namespace Ui {
class WinFailDialog;
}

class WinFailDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit WinFailDialog(QWidget *parent = 0);
    ~WinFailDialog();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::WinFailDialog *ui;
};

#endif // WINFAILDIALOG_H
