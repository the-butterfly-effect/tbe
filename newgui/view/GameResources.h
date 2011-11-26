#ifndef GAMERESOURCES_H
#define GAMERESOURCES_H

#include <QWidget>

namespace Ui {
    class GameResources;
}

class GameResources : public QWidget
{
    Q_OBJECT

public:
    explicit GameResources(QWidget *parent = 0);
    ~GameResources();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GameResources *ui;
};

#endif // GAMERESOURCES_H
