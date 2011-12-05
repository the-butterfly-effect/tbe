#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>

namespace Ui {
    class Overlay;
}

class QLabel;
class QToolButton;

class Overlay : public QWidget
{
public:
    explicit Overlay(QWidget *parent);
    ~Overlay();

    void parentResize(const QSize& aSize);

    QToolButton* getFFPtr();
    QLabel*      getStatusLabelPtr();
    QToolButton* getPlayPtr();
    QToolButton* getResetPtr();

protected:
//    void paintEvent(QPaintEvent *event);

private:
    Ui::Overlay *ui;

    QWidget* theParentPtr;
};

#endif // OVERLAY_H
