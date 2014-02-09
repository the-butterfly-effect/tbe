#ifndef LISTVIEWITEMTOOLTIP_H
#define LISTVIEWITEMTOOLTIP_H

#include "animateddialog.h"
#include "resizinggraphicsview.h"
//#include <QWidget>

namespace Ui {
class ListViewItemTooltip;
}

class ListViewItemTooltip : public AnimatedDialog
{
    Q_OBJECT
    
public:
    explicit ListViewItemTooltip(ResizingGraphicsView* parent = 0);
    ~ListViewItemTooltip();
    
public slots:
    void slotRemoveTooltip();

private:
    Ui::ListViewItemTooltip *ui;
};

#endif // LISTVIEWITEMTOOLTIP_H
