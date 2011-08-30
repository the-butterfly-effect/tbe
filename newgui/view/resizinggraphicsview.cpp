#include "resizinggraphicsview.h"

ResizingGraphicsView::ResizingGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
