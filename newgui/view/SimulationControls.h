#ifndef SIMULATIONCONTROLS_H
#define SIMULATIONCONTROLS_H

#include <QObject>
#include <QStateMachine>
#include <QState>

class QMenuBar;

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
	QStateMachine theStateMachine;
};

#endif // SIMULATIONCONTROLS_H
