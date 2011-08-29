#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>

namespace Ui {
    class MainWindow;
}

class QGraphicsScene;
class QGraphicsRectWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

	void setupMenu(void);
	void setupView(void);

private:
    Ui::MainWindow *ui;

	QGraphicsScene* theScenePtr;
	QGraphicsRectWidget *theDropDown;
};

#endif // MAINWINDOW_H
