#ifndef VIEWWORLD_H
#define VIEWWORLD_H

#include <QtGui/QGraphicsScene>

class ViewWorld : public QGraphicsScene
{
    Q_OBJECT
public:
    ViewWorld ( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 );
    explicit ViewWorld(QObject *parent = 0);

    virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );

signals:

public slots:

};

#endif // VIEWWORLD_H
