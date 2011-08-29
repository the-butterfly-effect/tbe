#ifndef RESIZINGGRAPHICSVIEW_H
#define RESIZINGGRAPHICSVIEW_H

#include <QGraphicsView>

class ResizingGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ResizingGraphicsView(QWidget *parent = 0);

protected:
	void resizeEvent(QResizeEvent *event)
	{
		QGraphicsView::resizeEvent(event);
		fitInView(sceneRect(), Qt::KeepAspectRatio);
	}

signals:

public slots:

};

#endif // RESIZINGGRAPHICSVIEW_H
