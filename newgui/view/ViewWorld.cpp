
#include <QGraphicsSceneMouseEvent>
#include "ViewWorld.h"
#include "PieMenu.h"

ViewWorld::ViewWorld(QObject *parent) :
    QGraphicsScene(parent)
{
}

ViewWorld::ViewWorld ( qreal x, qreal y, qreal width, qreal height, QObject * parent)
    : QGraphicsScene( x, y, width, height, parent)
{

}



void
ViewWorld::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
    if (itemAt(mouseEvent->scenePos())==NULL)
        PieMenuSingleton::clearPieMenu();
    QGraphicsScene::mousePressEvent(mouseEvent);
}
