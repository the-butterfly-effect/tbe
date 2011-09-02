#ifndef SIMULATIONCONTROLS_H
#define SIMULATIONCONTROLS_H

#include <QtGui>
#include "tbe_global.h"

class SimState : public QState
{
	Q_OBJECT
public:
	explicit SimState(QState* parent = 0, const QString& aStateName = "")
		: QState(parent), theName(aStateName)
	{ printf("state %s start!\n", ASCII(theName));	}

	virtual ~SimState()
	{}

protected:
	virtual void onEntry ( QEvent * event );

private:
	QString theName;
};



class SimulationControls : public QObject
{
    Q_OBJECT

public:
    explicit SimulationControls(QObject *parent = 0);

	void setup(QMenuBar* aMenuBarPtr);

signals:
	/// this signal is emitted when our slot onFailed is called
	void failed(void);

public slots:
	/// call this slot if the simulation has failed, the internal
	/// state machine will go to the Failed state.
	void onFailed(void)
	{ emit failed(); }

private slots:
	void stateEntered();

private:
	QStateMachine theSimStateMachine;

	QIcon thePlayIcon;
	QIcon thePauseIcon;
	QIcon theStopIcon;
	QIcon theResetIcon;
	QIcon theFwdIcon;
};

#endif // SIMULATIONCONTROLS_H
