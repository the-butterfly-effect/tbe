#ifndef DROPDOWNWINDOW_H
#define DROPDOWNWINDOW_H

#include <QtGui>

class DropDownWindow : public QGraphicsWidget
{
public:
	explicit DropDownWindow(QGraphicsWidget* parent = 0);

	void setup(QMenuBar* aMenuBarPtr);

signals:

public slots:
//	void hideYourself(void);
//	void showYourself(void);

private:
	QGraphicsWidget*       textEdit;
	QGraphicsWidget*       pushButton;
	QGraphicsLinearLayout* theLayoutPtr;

	/// used for the animation
	QStateMachine theStateMachine;
};

#endif // DROPDOWNWINDOW_H
