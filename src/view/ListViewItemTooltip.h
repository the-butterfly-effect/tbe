#ifndef LISTVIEWITEMTOOLTIP_H
#define LISTVIEWITEMTOOLTIP_H

#include <QWidget>

namespace Ui {
class ListViewItemTooltip;
}

class ListViewItemTooltip : public QWidget
{
    Q_OBJECT
    
public:
    explicit ListViewItemTooltip(QWidget *parent = 0);
    ~ListViewItemTooltip();
    
private:
    Ui::ListViewItemTooltip *ui;
};

#endif // LISTVIEWITEMTOOLTIP_H
