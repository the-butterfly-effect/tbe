#ifndef LISTVIEWITEMTOOLTIP_H
#define LISTVIEWITEMTOOLTIP_H

#include "animateddialog.h"
#include "ToolboxGroup.h"
#include "resizinggraphicsview.h"
//#include <QWidget>

namespace Ui {
class ListViewItemTooltip;
}

class ListViewItemTooltip : public AnimatedDialog
{
    Q_OBJECT
    
public:
    explicit ListViewItemTooltip(
            ToolboxGroup* aTBGPtr,
            ResizingGraphicsView* parent = 0);
    ~ListViewItemTooltip();
    
public slots:
    void on_buttonRemove_clicked();

private:
    Ui::ListViewItemTooltip *ui;
    ToolboxGroup* theTBGPtr;
};

#endif // LISTVIEWITEMTOOLTIP_H
