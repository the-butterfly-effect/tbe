#ifndef TOOLBOXLISTWIDGETITEM_H
#define TOOLBOXLISTWIDGETITEM_H

#include "resizinggraphicsview.h"
#include "ToolboxGroup.h"

#include <QListWidgetItem>
#include <QObject>

class ToolboxListWidgetItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    explicit ToolboxListWidgetItem(ResizingGraphicsView* aRSGVPtr,
                                   ToolboxGroup* aTBGPtr,
                                   QListWidget *parent = 0);

signals:

public slots:
    void slotSelected(QListWidgetItem*);

private:
    ToolboxGroup* theTBGPtr;
    ResizingGraphicsView* theRSGVPtr;
    
};

#endif // TOOLBOXLISTWIDGETITEM_H
